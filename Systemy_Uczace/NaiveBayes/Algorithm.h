#pragma once
#include "DataLoader.h"
#include "Model.h"
#include <memory>

namespace interfaces
{
    using loader::DataLoader;

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


