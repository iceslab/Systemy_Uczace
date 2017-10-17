#include "Algorithm.h"

namespace interfaces
{
    Algorithm::Algorithm(const loader::dataDescriptionT & description,
                         const loader::trainingDataT & trainingData) : trainingData(trainingData)
    {
    }

    Algorithm::~Algorithm()
    {
    }
};


