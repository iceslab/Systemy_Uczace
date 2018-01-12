#include "Booster.h"

namespace ensembles
{
    Booster::Booster(const source::DataSource & dl, size_t numberOfExamples) :
        Booster(dl.getDataMatrix(), numberOfExamples)
    {
        // Nothing to do
    }

    Booster::Booster(const source::dataMatrixT & matrix, size_t numberOfExamples) :
        matrix(matrix),
        drawExamplesSize(numberOfExamples)
    {
        if (numberOfExamples == 0 || numberOfExamples >= matrix.size())
        {
            drawExamplesSize = static_cast<size_t>(std::floor(0.9 * static_cast<double>(matrix.size())));
        }
    }

}