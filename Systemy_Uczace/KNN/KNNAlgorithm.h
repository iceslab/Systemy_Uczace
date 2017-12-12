#pragma once
#include "Algorithm.h"
#include "asserts.h"

namespace algorithm
{
    class KNNAlgorithm
    {
    public:
        KNNAlgorithm(const source::dataDescriptionT & description,
                     const source::trainingDataT & trainingData);
        ~KNNAlgorithm() = default;
    };
}
