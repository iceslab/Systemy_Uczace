#pragma once
#include "DataVector.h"

namespace abstracts
{
    class Model
    {
    public:
        Model() = default;
        ~Model() = default;

        virtual source::testDataT classify() = 0;
    };
};
