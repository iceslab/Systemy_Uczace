#include "stdafx.h"
#include "DataVector.h"

namespace source
{
    const dataV& DataVector::getClass() const
    {
        return back();
    }
    
    std::string DataVector::getClassName() const
    {
        const auto& clazz = getClass();
        return std::get<std::string>(clazz);
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
};