#include "Bagger.h"

namespace ensembles
{
    Bagger::Bagger(const source::DataSource & dl,
                   classifier::VotingTypeE votingType,
                   size_t classifiersCount,
                   size_t numberOfExamples) :
        Bagger(dl.getDataMatrix(), dl.getDataDescription(), votingType, classifiersCount, numberOfExamples)
    {
        // Nothing to do
    }

    Bagger::Bagger(const source::dataMatrixT & matrix,
                   const source::dataDescriptionT & description,
                   classifier::VotingTypeE votingType,
                   size_t classifiersCount,
                   size_t numberOfExamples) :
        matrix(matrix),
        description(description),
        votingType(votingType),
        classifiersCount(classifiersCount),
        drawExamplesSize(numberOfExamples)
    {
        if (numberOfExamples == 0 || numberOfExamples >= matrix.size())
        {
            drawExamplesSize = static_cast<size_t>(std::floor(0.9 * static_cast<double>(matrix.size())));
        }
    }

    source::testDataT Bagger::classify()
    {
        classifier::partialResultsT partialResults;
        partialResults.reserve(classifiersCount);
        algorithm::weightsMatrixT weightsMatrix;
        weightsMatrix.reserve(classifiersCount);
        algorithm::weightsVectorT weights;
        const auto trainingData = drawWeights(weights).second;
        testData = trainingData;
        for (size_t i = 0; i < classifiersCount; i++)
        {
            weightsMatrix.push_back(weights);
            algorithm::NaiveBayesAlgorithm nba(description, trainingData);
            nba.setDataWeights(weights);

            model::NaiveBayesModel nbm(testData, nba);
            partialResults.push_back(nbm.classify());
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

    source::testDataT Bagger::getTestData() const
    {
        return testData;
    }

    source::testDataPairT Bagger::drawWeights(algorithm::weightsVectorT & weights) const
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
}
