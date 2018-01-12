#pragma once
#include "DataSource.h"
#include "NaiveBayesAlgorithm.h"

namespace ensembles
{
    class Booster
    {
    public:
        Booster(const source::DataSource & dl, size_t numberOfExamples = 0);
        Booster(const source::dataMatrixT & matrix, size_t numberOfExamples = 0);
        ~Booster() = default;
    private:
        source::dataMatrixT matrix;
        size_t drawExamplesSize;
    };
}
