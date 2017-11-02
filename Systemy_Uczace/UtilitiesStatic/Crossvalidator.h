#pragma once
#include "DataSource.h"

typedef std::pair<source::testDataT, source::trainingDataT> crossvalidatorPairT;
namespace crossvalidator
{
    class Crossvalidator
    {
    public:
        Crossvalidator(const source::DataSource & dl);
        Crossvalidator(const source::dataMatrixT & matrix);
        ~Crossvalidator() = default;

        crossvalidatorPairT getNextData();
        bool hasNext();
    private:
        source::dataMatrixT matrix;
        static const size_t parts;

        size_t currentPart;
    };
}
