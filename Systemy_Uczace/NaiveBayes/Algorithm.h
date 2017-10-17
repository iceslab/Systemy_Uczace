#pragma once
#include "DataLoader.h"
#include "Model.h"
#include <memory>

namespace interfaces
{
    class Algorithm
    {
    public:
        Algorithm(const loader::dataDescriptionT & description, 
                  const loader::trainingDataT & trainingData);
        ~Algorithm();

        virtual std::unique_ptr<Model> produceModel() = 0;

    protected:
        loader::trainingDataT trainingData;
    };
};


