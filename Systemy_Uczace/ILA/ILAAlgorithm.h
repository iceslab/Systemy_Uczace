#pragma once
#pragma comment(lib, "UtilitiesStatic")
#include "Algorithm.h"
#include "asserts.h"
#include "NormalDistribution.h"

namespace algorithm
{
    class ILAAlgorithm : public abstracts::Algorithm
    {
    public:
        ILAAlgorithm(const source::dataDescriptionT & description,
                     const source::trainingDataT & trainingData);
        ~ILAAlgorithm() = default;
    };
}
