#include "Bagger.h"

namespace ensembles
{
    Bagger::Bagger(const source::DataSource & dl, size_t numberOfExamples) :
        Bagger(dl.getDataMatrix(), numberOfExamples)
    {
        // Nothing to do
    }

    Bagger::Bagger(const source::dataMatrixT & matrix, size_t numberOfExamples) :
        matrix(matrix),
        drawExamplesSize(numberOfExamples)
    {
        if (numberOfExamples == 0 || numberOfExamples >= matrix.size())
        {
            drawExamplesSize = static_cast<size_t>(std::floor(0.9 * static_cast<double>(matrix.size())));
        }
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
            ++weights[drawnNumber];
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
