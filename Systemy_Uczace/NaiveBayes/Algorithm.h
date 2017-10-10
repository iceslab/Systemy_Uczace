#pragma once
#include "DataLoader.h"
#include "Model.h"

namespace interfaces
{
    using loader::DataLoader;

    class Algorithm
    {
    public:
        Algorithm(const loader::DataLoader &loader);
        ~Algorithm();

        virtual Model& produceModel();

    private:
        loader::DataLoader loader;
    };
};


