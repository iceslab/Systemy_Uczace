#pragma once
#include "Model.h"
#include "asserts.h"
#include "KNNAlgorithm.h"

namespace model
{
    class KNNModel : abstracts::Model
    {
    public:
        KNNModel(const source::testDataT & testData,
                 const algorithm::KNNAlgorithm & algorithm);
        ~KNNModel() = default;

        virtual source::testDataT classify() override;
    };
}
