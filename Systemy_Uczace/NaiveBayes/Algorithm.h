#pragma once
#include "DataLoader.h"
#include "Model.h"
#include <memory>

namespace abstracts
{
    class Algorithm
    {
    public:
        Algorithm(const loader::dataDescriptionT & description, 
                  const loader::trainingDataT & trainingData);
        ~Algorithm();

        virtual void produceModel() = 0;

    protected:
        loader::dataDescriptionT descriptions;
        loader::trainingDataT trainingData;
    };
}


