#include "Algorithm.h"

namespace abstracts
{
    Algorithm::Algorithm(const loader::dataDescriptionT & description,
                         const loader::trainingDataT & trainingData) :
        descriptions(descriptions), trainingData(trainingData)
    {
    }

    Algorithm::~Algorithm()
    {
    }
};


