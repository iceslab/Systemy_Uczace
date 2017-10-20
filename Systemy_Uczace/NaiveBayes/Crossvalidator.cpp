#include "Crossvalidator.h"


Crossvalidator::Crossvalidator(source::DataSource dl) : Crossvalidator(dl.getDataMatrix())
{
    // Nothing to do
}

Crossvalidator::Crossvalidator(source::dataMatrixT matrix)
{
    this->matrix = matrix;
}

Crossvalidator::~Crossvalidator()
{
}

crossvalidatorPairT Crossvalidator::getData()
{
    // Simple implementaion to get things working
    const size_t testDataSize = std::max(matrix.size() / 10LLU, 1LLU);

    return crossvalidatorPairT(source::testDataT(matrix.begin(), matrix.begin() + testDataSize),
                               source::trainingDataT(matrix.begin() + testDataSize, matrix.end()));
}
