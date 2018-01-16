#pragma once
#include "DataSource.h"
#include "NaiveBayesAlgorithm.h"
#include "NaiveBayesModel.h"
#include "VoteClassifier.h"
#include <random>

namespace ensembles
{
    class Booster
    {
    public:
        Booster(const source::DataSource & dl,
                classifier::VotingTypeE votingType,
                size_t classifiersCount,
                double deltaWeight,
                size_t numberOfExamples = 0);
        Booster(const source::dataMatrixT & matrix,
                const source::dataDescriptionT & description,
                classifier::VotingTypeE votingType,
                size_t classifiersCount,
                double deltaWeight,
                size_t numberOfExamples = 0);
        ~Booster() = default;

        source::testDataT classify();
        source::testDataT getTestData() const;
    private:
        const source::dataMatrixT matrix;
        const source::dataDescriptionT description;
        classifier::VotingTypeE votingType;
        size_t classifiersCount;
        double deltaWeight;
        size_t drawExamplesSize;
        source::testDataT testData;

        source::testDataPairT drawWeights(algorithm::weightsVectorT & weights) const;
        algorithm::weightsVectorT modifyWeights(const classifier::classificationResultVectorT & correct,
                                                algorithm::weightsVectorT & weights) const;
    };
}
