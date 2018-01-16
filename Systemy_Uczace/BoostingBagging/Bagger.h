#pragma once
#include "DataSource.h"
#include "NaiveBayesAlgorithm.h"
#include "NaiveBayesModel.h"
#include "VoteClassifier.h"
#include <random>

namespace ensembles
{
    class Bagger
    {
    public:
        Bagger(const source::DataSource & dl,
               classifier::VotingTypeE votingType,
               size_t classifiersCount,
               size_t numberOfExamples = 0);
        Bagger(const source::dataMatrixT & matrix,
               const source::dataDescriptionT & description,
               classifier::VotingTypeE votingType,
               size_t classifiersCount,
               size_t numberOfExamples = 0);
        ~Bagger() = default;

        source::testDataT classify();
        source::testDataT getTestData() const;
    private:
        const source::dataMatrixT matrix;
        const source::dataDescriptionT description;
        classifier::VotingTypeE votingType;
        size_t classifiersCount;
        size_t drawExamplesSize;
        source::testDataT testData;

        source::testDataPairT drawWeights(algorithm::weightsVectorT & weights) const;
    };
}
