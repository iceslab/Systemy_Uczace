#pragma once
#include "DataSource.h"

namespace crossvalidator
{
    class Crossvalidator
    {
    public:
        Crossvalidator(const source::DataSource & dl);
        Crossvalidator(const source::dataMatrixT & matrix);
        ~Crossvalidator() = default;

        source::testDataPairT getNextData();
        bool hasNext();
    private:
        source::dataMatrixT matrix;
        static const size_t parts;

        size_t currentPart;
    };
}
