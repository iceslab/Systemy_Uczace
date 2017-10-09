#include "DataLoader.h"

namespace loader
{

    DataLoader::DataLoader(std::string filePath) : DataLoader(std::ifstream(filePath))
    {
        // Nothing to do
    }

    DataLoader::DataLoader(std::ifstream fileStream)
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
        while (true)
        {
            std::string s;
            
            // Get non empty line
            if (!getline(fileStream, s) || s.empty())
                break;

            std::istringstream ss(s);
            std::vector<std::string> record(categoryCount);

            // Read category description
            for (auto i = 0; i < categoryCount && ss; i++)
            {
                std::string s;
                if (!getline(ss, s, ' ')) 
                    break;
                record[i] = s;
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
            description.emplace_back(std::make_tuple(type, record[1], std::vector<std::string>()));

            while (true)
            {
                std::string s;
                if (!getline(ss, s, ','))
                    break;
                std::get<2>(description.back()).push_back(s);
            }
        }

        // Read data
        while (true)
        {
            std::string s;

            // Get non empty line
            if (!getline(fileStream, s) || s.empty())
                break;

            std::vector<std::string> vec;

            size_t notPos = 0;
            size_t pos = 0;
            while (true)
            {
                notPos = s.find_first_not_of(delimitersStr, pos);
                pos = s.find_first_of(delimitersStr, notPos);

                if (notPos != std::string::npos)
                {
                    if (pos != std::string::npos)
                    {
                        vec.push_back(s.substr(notPos, pos));
                    }
                    else
                    {
                        vec.push_back(s.substr(notPos));
                    }
                }
                else
                {
                    break;
                }
            }
            
            matrix.emplace_back(vec);
        }
    }

    DataLoader::~DataLoader()
    {
    }

    const dataDescriptionT& DataLoader::getDataDescription() const
    {
        return description;
    }

    const dataMatrixT& DataLoader::getDataMatrix() const
    {
        return matrix;
    }
}