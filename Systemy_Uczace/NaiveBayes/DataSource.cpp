#include "DataSource.h"

namespace source
{

    DataSource::DataSource(std::string filePath) : DataSource(std::ifstream(filePath))
    {
        // Nothing to do
    }

    DataSource::DataSource(std::ifstream fileStream)
    {
        // If file is not open throw an exception
        if (!fileStream.is_open())
        {
            throw std::ios_base::failure("Couldn't open file");
        }

        // File format:
        // First several lines describe categories of later data vectors
        // Categories should have this pattern (values in <> are required, in [] are optional):
        // <type_name> <category_name> [name_1],[name_2],...,[name_n]
        // Example:
        // category family RICH, POOR
        // integer wealth 100000, 200000
        // real distance_from_city 22.3 10.0
        // 
        // Data vectors should be separated by whitespace, semicolon or colon.
        // Missing values are not oficially supported.

        // Read categories description
        loadDescription(fileStream);

        // Read data
        loadMatrix(fileStream);

    }

    const DataDescription& DataSource::getDataDescription() const
    {
        return description;
    }

    const dataMatrixT& DataSource::getDataMatrix() const
    {
        return matrix;
    }

    DataDescription& DataSource::getDataDescription()
    {
        return description;
    }

    dataMatrixT& DataSource::getDataMatrix()
    {
        return matrix;
    }

    dataV DataSource::variantFromString(source::DataTypeE type, std::string value)
    {
        dataV var;
        switch (type)
        {
            case CATEGORY:
                var = value;
                break;
            case INTEGER:
                var = std::stoi(value);
                break;
            case REAL:
                var = std::stod(value);
                break;
            case INTEGER_DISCRETE:
            case REAL_DISCRETE:
            case UNDEFINED:
            default:
                FATAL_ERROR_VERBOSE("Illegal type in union costructor: %d", type);
                break;
        }

        return var;
    }

    size_t DataSource::getElementIndex(source::DataTypeE type,
                                       const source::dataV & data,
                                       const source::dataDescriptionElementT & description)
    {
        switch (type)
        {
            case source::CATEGORY:
                return getElementIndex<std::string>(data, description);
            case source::INTEGER_DISCRETE:
                return getElementIndex<std::pair<int, int>>(data, description);
            case source::REAL_DISCRETE:
                return getElementIndex<std::pair<double, double>>(data, description);
            case source::INTEGER:
            case source::REAL:
                DEBUG_PRINTLN("Function %s() does not support values INTEGER or REAL. "
                              "Returning max of size_t.",
                              __FUNCTION__);
                break;
            default:
                FATAL_ERROR_VERBOSE("Function %s() does not support value provided (%d)",
                                    __FUNCTION__,
                                    type);
                break;
        }
        return std::numeric_limits<size_t>::max();
    }

    template<typename T>
    size_t DataSource::getElementIndex(const source::dataV & data,
                                       const source::dataDescriptionElementT & description)
    {
        auto desiredClassName = std::get<T>(data);
        auto classes = std::get<2>(description);
        auto it = std::find_if(
            classes.begin(),
            classes.end(),
            [desiredClassName](source::descriptionV el)->bool
        {
            return std::get<T>(el) == desiredClassName;
        });

        ASSERT(it != classes.end());
        const auto classIndex = it - classes.begin();
        return classIndex;
    }

    void DataSource::loadDescription(std::ifstream &fileStream)
    {
        while (true)
        {
            std::string token;

            // Get non empty line
            if (!getline(fileStream, token) || token.empty())
                break;

            std::istringstream ss(token);
            std::vector<std::string> record(categoryCount);

            // Read category description
            for (auto i = 0; i < categoryCount && ss; i++)
            {
                std::string token;
                if (!getline(ss, token, ' '))
                    break;
                record[i] = token;
            }

            // Deduce data type
            DataTypeE type = UNDEFINED;

            if (record[0] == categoryStr)
                type = CATEGORY;
            else if (record[0] == integerStr)
                type = INTEGER;
            else if (record[0] == realStr)
                type = REAL;

            // Emplace description type (type, name, vector of allowed values)
            description.emplace_back(std::make_tuple(type, record[1], std::vector<descriptionV>()));

            while (true)
            {
                std::string s;
                if (!getline(ss, s, ','))
                    break;
                std::get<2>(description.back()).push_back(s);
            }
        }
    }

    void DataSource::loadMatrix(std::ifstream &fileStream)
    {
        ASSERT(!description.empty());
        const auto attributesCount = description.size();
        while (true)
        {
            std::string token;

            // Get non empty line
            if (!getline(fileStream, token) || token.empty())
                break;

            DataVector vec(attributesCount);

            size_t notPos = 0;
            size_t pos = 0;
            for (size_t i = 0; i < attributesCount; i++)
            {
                auto type = std::get<0>(description[i]);
                notPos = token.find_first_not_of(delimitersStr, pos);
                pos = token.find_first_of(delimitersStr, notPos);

                if (notPos != std::string::npos)
                {
                    std::string substring;
                    if (pos != std::string::npos)
                    {
                        substring = token.substr(notPos, pos - notPos);
                    }
                    else
                    {
                        substring = token.substr(notPos);
                    }

                    vec[i] = variantFromString(type, substring);
                }
                else
                {
                    break;
                }
            }

            matrix.emplace_back(vec);
        }
    }
}