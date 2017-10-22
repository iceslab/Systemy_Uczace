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
};