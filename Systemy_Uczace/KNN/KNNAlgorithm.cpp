#include "KNNAlgorithm.h"

namespace algorithm
{
    KNNAlgorithm::KNNAlgorithm(const source::dataDescriptionT & description,
                               const source::trainingDataT & trainingData) :
        abstracts::Algorithm(description, trainingData)
    {
        for (const auto& row : trainingData)
        {
            for (const auto& el : row)
            {
                ASSERT_VERBOSE(el.index() == 1 || el.index() == 2, "Data type should be int or double got: %zu", el.index());
            }
        }
    }
}
