#pragma once
#include "DataSource.h"
#include <algorithm>

typedef std::pair<source::testDataT, source::trainingDataT> crossvalidatorPairT;

class Crossvalidator
{
public:
    Crossvalidator(source::DataSource dl);
    Crossvalidator(source::dataMatrixT matrix);
    ~Crossvalidator();

    crossvalidatorPairT getData();
private:
    source::dataMatrixT matrix;
};

