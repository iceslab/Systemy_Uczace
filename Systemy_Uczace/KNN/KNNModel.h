#pragma once
#include "Model.h"
#include "asserts.h"
#include "KNNAlgorithm.h"

namespace model
{
    class KNNModel
    {
    public:
        KNNModel(const source::testDataT & testData,
                 const algorithm::KNNAlgorithm & algorithm);
        ~KNNModel() = default;
    };
}
