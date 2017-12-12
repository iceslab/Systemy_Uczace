#include "stdafx.h"
#include "DataVector.h"

namespace source
{
    const double DataVector::invalidNumericValue = std::numeric_limits<double>::lowest();

    const dataV& DataVector::getClass() const
    {
        return back();
    }

    std::string DataVector::getClassName() const
    {
        const auto& clazz = getClass();
        return std::get<std::string>(clazz);
    }

    std::string DataVector::toString() const
    {
        std::stringstream ss;
        for (const auto& el : *this)
        {
            ss << toString(el);
            ss << " ";
        }

        return ss.str();
    }

    dataColumnRefT DataVector::getAttributeColumn(std::vector<DataVector>& dc, size_t attributeIndex)
    {
        dataColumnRefT retVal;
        retVal.reserve(dc.size());

        for (auto& el : dc)
        {
            retVal.emplace_back(el[attributeIndex]);
        }

        return retVal;
    }

    dataColumnRefT DataVector::getClassColumn(std::vector<DataVector>& dc)
    {
        ASSERT(!dc.empty());
        return getAttributeColumn(dc, dc.front().size() - 1);
    }

    classAsStringColumnT DataVector::getClassColumnAsString(const std::vector<DataVector>& dc)
    {
        classAsStringColumnT retVal;
        retVal.reserve(dc.size());

        for (auto& el : dc)
        {
            retVal.emplace_back(std::get<std::string>(el.back()));
        }

        return retVal;
    }

    std::string DataVector::toString(const dataV & data)
    {
        std::stringstream ss;
        switch (data.index())
        {
            // std::string
            case 0:
                ss << std::get<std::string>(data);
                break;
                // int
            case 1:
                ss << std::get<int>(data);
                break;
                // double
            case 2:
                ss << std::get<double>(data);
                break;
                // std::pair<int, int>
            case 3:
                ss << "["
                    << std::get<std::pair<int, int>>(data).first
                    << ", "
                    << std::get<std::pair<int, int>>(data).second
                    << "]";
                break;
                // std::pair<double, double>
            case 4:
                ss << "["
                    << std::get<std::pair<double, double>>(data).first
                    << ", "
                    << std::get<std::pair<double, double>>(data).second
                    << "]";
                break;
        }
        return ss.str();
    }
    double DataVector::getNumericValue(const dataV & element)
    {
        auto retVal = invalidNumericValue;
        const auto idx = element.index();
        if (idx == 1)
        {
            retVal = std::get<1>(element);
        }
        else if (idx == 2)
        {
            retVal = std::get<2>(element);
        }
        else
        {
            DEBUG_PRINTLN("Cannot get numeric value. Element is not numeric.");
        }

        return retVal;
    }

    double DataVector::vectorLength(const DataVector & vector)
    {
        auto retVal = 0.0;
        for (const auto& el : vector)
        {
            auto currentValue = source::DataVector::getNumericValue(el);
            if (currentValue == source::DataVector::invalidNumericValue)
            {
                DEBUG_PRINTLN("Cannot calculate vector length. Vector is not numeric.");
                retVal = 0.0;
                break;
            }

            retVal += currentValue * currentValue;
        }
        return std::sqrt(retVal);
    }

    void DataVector::normalize(DataVector & vector)
    {
        const auto length = vectorLength(vector);
        if (length != 0.0)
        {
            for (auto &el : vector)
            {
                auto currentValue = getNumericValue(el);
                if (currentValue == invalidNumericValue)
                {
                    DEBUG_PRINTLN("Cannot normalize non numeric element. Skipping...");
                }
                else
                {
                    el = source::dataV(currentValue / length);
                }
            }
        }
        else
        {
            DEBUG_PRINTLN("Cannot normalize vector. Length is 0");
        }
    }
};