#include "stdafx.h"
#include "Algorithm.h"

namespace abstracts
{
    Algorithm::Algorithm(const source::dataDescriptionT & description,
                         const source::trainingDataT & trainingData) :
        descriptions(description), trainingData(trainingData)
    {
        const auto classNames = description.getClassNames();

        for (size_t i = 0; i < classNames.size(); i++)
        {
            indexToClassNameMap.emplace(i, classNames[i]);
            classNameToIndexMap.emplace(classNames[i], i);
        }
    }

    const source::dataDescriptionT & Algorithm::getDescriptions() const
    {
        return descriptions;
    }

    const source::trainingDataT & Algorithm::getTrainingData() const
    {
        return trainingData;
    }

    size_t Algorithm::classNameToIndex(std::string name) const
    {
        const auto it = classNameToIndexMap.find(name);
        ASSERT(it != classNameToIndexMap.end());
        return it->second;
    }

    std::string Algorithm::indexToClassName(size_t index) const
    {
        const auto it = indexToClassNameMap.find(index);
        ASSERT(it != indexToClassNameMap.end());
        return it->second;
    }
};


