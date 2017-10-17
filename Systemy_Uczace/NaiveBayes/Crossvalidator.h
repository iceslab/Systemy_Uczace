#pragma once
//#include <tuple>
#include "DataLoader.h"

typedef std::pair<loader::testDataT, loader::trainingDataT> crossvalidatorPairT;

class Crossvalidator
{
public:
    Crossvalidator(loader::DataLoader dl);
    Crossvalidator(loader::dataMatrixT matrix);
    ~Crossvalidator();

    crossvalidatorPairT getData();
private:
    loader::dataMatrixT matrix;
};

