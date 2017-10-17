#include "Crossvalidator.h"


Crossvalidator::Crossvalidator(loader::DataLoader dl) : Crossvalidator(dl.getDataMatrix())
{
    // Nothing to do
}

Crossvalidator::Crossvalidator(loader::dataMatrixT matrix)
{
    this->matrix = matrix;
}

Crossvalidator::~Crossvalidator()
{
}

crossvalidatorPairT Crossvalidator::getData()
{
    // Simple implementaion to get things working
    const size_t testDataSize = matrix.size() / 10U;
    return crossvalidatorPairT(loader::testDataT(matrix.begin(), matrix.begin() + testDataSize),
                               loader::trainingDataT(matrix.begin() + testDataSize, matrix.end()));
}
