#pragma once
#include "Algorithm.h"
#include "asserts.h"
#include "NormalDistribution.h"

namespace algorithm
{
    class KNNAlgorithm : public abstracts::Algorithm
    {
    public:
        KNNAlgorithm(const source::dataDescriptionT & description,
                     const source::trainingDataT & trainingData);
        ~KNNAlgorithm() = default;

        void normalize();
        void standardize();
    };
}
