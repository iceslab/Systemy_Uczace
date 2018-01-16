#include "Booster.h"

namespace ensembles
{
    Booster::Booster(const source::DataSource & dl,
                     classifier::VotingTypeE votingType,
                     size_t classifiersCount,
                     double deltaWeight,
                     size_t numberOfExamples) :
        Booster(dl.getDataMatrix(),
                dl.getDataDescription(),
                votingType,
                classifiersCount,
                deltaWeight,
                numberOfExamples)
    {
        // Nothing to do
    }

    Booster::Booster(const source::dataMatrixT & matrix,
                     const source::dataDescriptionT & description,
                     classifier::VotingTypeE votingType,
                     size_t classifiersCount,
                     double deltaWeight,
                     size_t numberOfExamples) :
        matrix(matrix),
        description(description),
        votingType(votingType),
        classifiersCount(classifiersCount),
        deltaWeight(deltaWeight),
        drawExamplesSize(numberOfExamples)
    {
        if (numberOfExamples == 0 || numberOfExamples >= matrix.size())
        {
            drawExamplesSize = static_cast<size_t>(std::floor(0.9 * static_cast<double>(matrix.size())));
        }
    }

    source::testDataT Booster::classify()
    {
        classifier::partialResultsT partialResults;
        partialResults.reserve(classifiersCount);
        algorithm::weightsMatrixT weightsMatrix;
        weightsMatrix.reserve(classifiersCount);

        algorithm::weightsVectorT nextWeights;
        const auto trainingData = drawWeights(nextWeights).second;
        testData = trainingData;
        for (size_t i = 0; i < classifiersCount; i++)
        {
            weightsMatrix.push_back(nextWeights);
            algorithm::NaiveBayesAlgorithm nba(description, trainingData);
            nba.setDataWeights(nextWeights);

            model::NaiveBayesModel nbm(testData, nba);
            const auto partialResult = nbm.classify();
            partialResults.push_back(partialResult);
            const auto correct = classifier::VoteClassifier::checkClassification(trainingData, partialResult);
            nextWeights = modifyWeights(correct, nextWeights);
        }


        source::testDataT retVal;
        switch (votingType)
        {
        case classifier::WEIGHTED:
            retVal = classifier::VoteClassifier::weightedVoting(partialResults, weightsMatrix);
            break;
        case classifier::NORMAL:
            retVal = classifier::VoteClassifier::normalVoting(partialResults);
            break;
        default:
            FATAL_ERROR_VERBOSE("Invalid voting type");
            break;
        }

        return retVal;
    }

    source::testDataT Booster::getTestData() const
    {
        return testData;
    }

    source::testDataPairT Booster::drawWeights(algorithm::weightsVectorT & weights) const
    {
        std::random_device rd;
        std::default_random_engine re(rd());
        std::uniform_int_distribution<size_t> distribution(0, matrix.size() - 1);

        source::testDataT testData;
        source::trainingDataT trainingData;
        algorithm::weightsVectorT tmpWeights(matrix.size(), 0.0);
        weights.clear();
        weights.reserve(drawExamplesSize);
        testData.reserve(matrix.size() - drawExamplesSize);
        trainingData.reserve(drawExamplesSize);

        for (size_t i = 0; i < drawExamplesSize; i++)
        {
            const auto& drawnNumber = distribution(re);
            ++tmpWeights[drawnNumber];
        }

        for (size_t i = 0; i < matrix.size(); i++)
        {
            if (tmpWeights[i] > 0.0)
            {
                trainingData.push_back(matrix[i]);
                weights.push_back(tmpWeights[i]);
            }
            else
            {
                testData.push_back(matrix[i]);
            }
        }

        return std::make_pair(testData, trainingData);
    }

    algorithm::weightsVectorT Booster::modifyWeights(const classifier::classificationResultVectorT & correct,
                                                     algorithm::weightsVectorT & weights) const
    {
        auto retVal = weights;
        for (size_t i = 0; i < correct.size(); i++)
        {
            if (correct[i] == true)
            {
                retVal[i] -= deltaWeight;
                if (retVal[i] < 0.0)
                {
                    retVal[i] = 0.0;
                }
            }
            else
            {
                retVal[i] += deltaWeight;
            }
        }

        return retVal;
    }

}