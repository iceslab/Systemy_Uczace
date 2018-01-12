#pragma once
#include "DataSource.h"
#include "NaiveBayesAlgorithm.h"

namespace classifier
{
    enum VotingTypeE
    {
        NORMAL,
        WEIGHTED
    };

    typedef std::vector<bool> classificationResultVectorT;
    typedef std::vector<source::testDataT> partialResultsT;

    class VoteClassifier
    {
    public:
        VoteClassifier() = delete;
        ~VoteClassifier() = delete;

        static source::testDataT weightedVoting(const partialResultsT & classifiedExamples,
                                                  const algorithm::weightsMatrixT & weights);
        static source::testDataT normalVoting(const partialResultsT & classifiedExamples);
        static source::dataVectorT weightedVoting(const source::dataMatrixT & classifiedExamples,
                                                  const algorithm::weightsVectorT & weights);
        static source::dataVectorT normalVoting(const source::dataMatrixT & classifiedExamples);
        static classificationResultVectorT checkClassification(
            const source::dataMatrixT & originalExamples,
            const source::dataMatrixT & classifiedExamples);
    };
}
