#pragma once
#include "DataSource.h"
#include "NaiveBayesAlgorithm.h"

namespace classifier
{
    typedef std::vector<bool> classificationResultVectorT;
    class VoteClassifier
    {
    public:
        VoteClassifier() = delete;
        ~VoteClassifier() = delete;

        static source::dataVectorT weightedVoting(const source::dataMatrixT & classifiedExamples,
                                                  const algorithm::weightsVectorT & weights);
        static source::dataVectorT normalVoting(const source::dataMatrixT & classifiedExamples);
        static classificationResultVectorT checkClassification(
            const source::dataMatrixT & originalExamples,
            const source::dataMatrixT & classifiedExamples);
    };
}
