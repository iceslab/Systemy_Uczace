#pragma once
#include "KNNModel.h"
#include <memory>

namespace model
{
    enum DistanceFunctionE
    {
        EUCLIDEAN,
        MANHATTAN,
        MINKOWSKI
    };

    class KNNFactory
    {
    public:
        KNNFactory() = delete;
        ~KNNFactory() = delete;

        static std::unique_ptr<KNNModel> getKNNModel(const source::testDataT & testData,
                                                     const algorithm::KNNAlgorithm & algorithm,
                                                     const DistanceFunctionE functionType,
                                                     const double power = 1.0);
    };
}
