#pragma once
#include "DataLoader.h"
#include "Model.h"
#include <memory>

namespace interfaces
{
    class Algorithm
    {
    public:
        Algorithm(const loader::DataLoader &loader);
        ~Algorithm();

        virtual std::unique_ptr<Model> produceModel() = 0;

    protected:
        loader::DataLoader loader;
    };
};


