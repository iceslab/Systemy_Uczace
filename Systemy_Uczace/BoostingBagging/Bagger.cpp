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
        if (numberOfExamples == 0)
        {
            drawExamplesSize = static_cast<size_t>(std::floor(0.9 * static_cast<double>(matrix.size())));
        }
    }

    algorithm::weightsVectorT Bagger::drawWeights() const
    {
        std::random_device rd;
        std::default_random_engine re(rd());
        std::uniform_int_distribution<size_t> distribution(0, matrix.size() - 1);
        algorithm::weightsVectorT retVal(matrix.size(), 0.0);

        for (size_t i = 0; i < drawExamplesSize; i++)
        {
            const auto& drawnNumber = distribution(re);
            ++retVal[drawnNumber];
        }

        return retVal;
    }
}
