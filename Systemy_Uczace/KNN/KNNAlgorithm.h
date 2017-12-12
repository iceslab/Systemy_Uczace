#pragma once
#include "Algorithm.h"
#include "asserts.h"

namespace algorithm
{
    class KNNAlgorithm : public abstracts::Algorithm
    {
    public:
        KNNAlgorithm(const source::dataDescriptionT & description,
                     const source::trainingDataT & trainingData);
        ~KNNAlgorithm() = default;

        void normalize();


        static double vectorLength(const source::dataVectorT & vector);
        static void normalize(source::dataVectorT & vector);
    };
}
