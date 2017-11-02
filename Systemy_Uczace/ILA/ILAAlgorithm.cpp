#include "ILAAlgorithm.h"

namespace algorithm
{
    ILAAlgorithm::ILAAlgorithm(const source::dataDescriptionT & description, 
                               const source::trainingDataT & trainingData) :
        abstracts::Algorithm(descriptions, trainingData)
    {
        // Nothing to do
    }
}
