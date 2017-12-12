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

    void KNNAlgorithm::normalize()
    {
        for (auto& el : trainingData)
        {
            source::DataVector::normalize(el);
        }
    }

    void KNNAlgorithm::standardize()
    {
        const auto attributesSize = trainingData.size() - 1;
        for (size_t columnIdx = 0; columnIdx < attributesSize; columnIdx++)
        {
            auto column = source::DataVector::getAttributeColumn(trainingData, columnIdx);
            const auto mean = distribution::NormalDistribution::calculateMean(column);
            const auto stddev = distribution::NormalDistribution::calculateStddev(column);

            for (auto& el : column)
            {
                const auto val = source::DataVector::getNumericValue(el.get());
                el.get() = (val - mean) / stddev;
            }
        }
    }


}
