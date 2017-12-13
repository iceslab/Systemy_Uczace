#pragma once
#include "DataSource.h"
#include "Model.h"

namespace abstracts
{
    typedef std::unordered_map<std::string, size_t> classNameToIndexMapT;
    typedef std::unordered_map<size_t, std::string> indexToClassNameMapT;

    class Algorithm
    {
    public:
        Algorithm(const source::dataDescriptionT & description, 
                  const source::trainingDataT & trainingData);
        ~Algorithm() = default;

        const source::dataDescriptionT& getDescriptions() const;
        const source::trainingDataT& getTrainingData() const;

    protected:
        source::dataDescriptionT descriptions;
        source::trainingDataT trainingData;

        size_t classNameToIndex(std::string name) const;
        std::string indexToClassName(size_t index) const;

    private:
        indexToClassNameMapT indexToClassNameMap;
        classNameToIndexMapT classNameToIndexMap;
    };
}


