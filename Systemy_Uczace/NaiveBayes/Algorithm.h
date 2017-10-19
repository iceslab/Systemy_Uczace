#pragma once
#include "DataSource.h"
#include "Model.h"
#include <memory>
#include <algorithm>

namespace abstracts
{
    class Algorithm
    {
    public:
        Algorithm(const source::dataDescriptionT & description, 
                  const source::trainingDataT & trainingData);
        ~Algorithm();

        virtual void produceModel() = 0;

        const source::dataDescriptionT& getDescriptions() const;

    protected:
        source::dataDescriptionT descriptions;
        source::trainingDataT trainingData;
    };
}


