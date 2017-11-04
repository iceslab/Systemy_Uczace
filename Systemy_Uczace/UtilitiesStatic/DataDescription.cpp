#include "stdafx.h"
#include "DataDescription.h"

namespace source
{
    size_t DataDescription::getLongestClassNameLength() const
    {
        auto maxValue = std::numeric_limits<size_t>::min();
        const auto& classesDescriptions = std::get<2>(this->back());
        for (const auto& element : classesDescriptions)
        {
            const auto& classNameLength = std::get<std::string>(element).size();
            if (classNameLength > maxValue)
            {
                maxValue = classNameLength;
            }
        }

        return maxValue;
    }

    dataDescriptionElementT DataDescription::getClassDescriptions() const
    {
        return back();
    }

    classNamesT DataDescription::getClassNames() const
    {
        return getClassNames(getClassDescriptions());
    }

    classNamesT DataDescription::getClassNames(const dataDescriptionElementT & description)
    {
        const auto descriptionVector = std::get<descriptionVectorT>(description);
        classNamesT retVal;
        retVal.reserve(descriptionVector.size());

        for (const auto& el : descriptionVector)
        {
            retVal.emplace_back(std::get<std::string>(el));
        }

        return retVal;
    }
    
    size_t DataDescription::getClassNamesCount() const
    {
        return std::get<descriptionVectorT>(getClassDescriptions()).size();
    }
};