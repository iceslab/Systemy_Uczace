#include "Crossvalidator.h"

namespace crossvalidator
{
    const size_t Crossvalidator::parts = 10;

    Crossvalidator::Crossvalidator(const source::DataSource & dl) : 
        Crossvalidator(dl.getDataMatrix())
    {
        // Nothing to do
    }

    Crossvalidator::Crossvalidator(const source::dataMatrixT & matrix) :
        matrix(matrix), currentPart(0U)
    {
        // In C++17 std::random_shuffle is removed
        std::random_device rd;
        std::default_random_engine re(rd());
        std::shuffle(this->matrix.begin(), this->matrix.end(), re);
    }

    crossvalidatorPairT Crossvalidator::getNextData()
    {
        if (currentPart >= parts)
            return crossvalidatorPairT();

        const size_t testDataSize = std::max(matrix.size() / parts, 1U);

        auto testDataStart = matrix.begin() + testDataSize * currentPart;
        auto testDataEnd = testDataStart + testDataSize;
        source::testDataT testData(testDataStart, testDataEnd);

        auto testDataFirstStart = matrix.begin();
        auto testDataFirstEnd = testDataStart;
        auto testDataSecondStart = testDataEnd;
        auto testDataSecondEnd = matrix.end();

        source::trainingDataT trainingDataFirst(testDataFirstStart, testDataFirstEnd);
        source::trainingDataT trainingDataSecond(testDataSecondStart, testDataSecondEnd);


        source::trainingDataT trainingData(trainingDataFirst);
        trainingData.insert(trainingData.begin(),
                            trainingDataSecond.begin(),
                            trainingDataSecond.end());
        currentPart++;
        return crossvalidatorPairT(testData, trainingData);
    }
    bool Crossvalidator::hasNext()
    {
        return currentPart < parts;
    }
}