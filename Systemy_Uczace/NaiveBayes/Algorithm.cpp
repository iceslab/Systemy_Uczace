#include "Algorithm.h"

namespace abstracts
{
    Algorithm::Algorithm(const source::dataDescriptionT & description,
                         const source::trainingDataT & trainingData) :
        descriptions(descriptions), trainingData(trainingData)
    {
    }

    Algorithm::~Algorithm()
    {
    }
    const source::dataDescriptionT & Algorithm::getDescriptions() const
    {
        return descriptions;
    }
};


