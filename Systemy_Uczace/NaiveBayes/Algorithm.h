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

        const source::dataDescriptionT& getDescriptions() const;

    protected:
        source::dataDescriptionT descriptions;
        source::trainingDataT trainingData;
    };
}


