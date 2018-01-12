#pragma once
#include "DataSource.h"
#include "NaiveBayesAlgorithm.h"
#include <random>

namespace ensembles
{
    class Bagger
    {
    public:
        Bagger(const source::DataSource & dl, size_t numberOfExamples = 0);
        Bagger(const source::dataMatrixT & matrix, size_t numberOfExamples = 0);
        ~Bagger() = default;

        source::testDataPairT drawWeights(algorithm::weightsVectorT & weights) const;
    private:
        source::dataMatrixT matrix;
        size_t drawExamplesSize;
    };
}
