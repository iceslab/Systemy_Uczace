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
                 const algorithm::KNNAlgorithm & algorithm,
                 const source::distanceFn & distanceFunction);
        ~KNNModel() = default;

        virtual source::testDataT classify() override;

    private:
        const source::distanceFn distanceFunction;

        source::dataVectorT majorityVoting(const source::dataVectorT & row,
                                           const source::dataMatrixT & neighbours);
        source::dataVectorT weightedVoting(const source::dataVectorT & row,
                                           const source::dataMatrixT & neighbours);
    };
}
