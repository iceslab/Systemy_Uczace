#include "KNNAlgorithm.h"

namespace algorithm
{
    KNNAlgorithm::KNNAlgorithm(const source::dataDescriptionT & description,
                               const source::trainingDataT & trainingData) :
        abstracts::Algorithm(description, trainingData)
    {
        // Nothing to do
    }

    void KNNAlgorithm::normalize()
    {
        for (const auto& row : trainingData)
        {
            for (const auto& el : row)
            {
                ASSERT_VERBOSE(el.index() == 1 || el.index() == 2, "Data type should be int or double got: %zu", el.index());
            }
        }

        for (auto& el : trainingData)
        {
            normalize(el);
        }
    }

    double KNNAlgorithm::vectorLength(const source::dataVectorT & vector)
    {
        auto retVal = 0.0;
        for (const auto& el : vector)
        {
            auto currentValue = source::DataVector::getNumericValue(el);
            if (currentValue == source::DataVector::invalidNumericValue)
            {
                DEBUG_PRINTLN("Cannot calculate vector length. Vector is not numeric.");
                retVal = 0.0;
                break;
            }

            retVal += currentValue * currentValue;
        }
        return std::sqrt(retVal);
    }

    void KNNAlgorithm::normalize(source::dataVectorT & vector)
    {
        const auto length = vectorLength(vector);
        if (length != 0.0)
        {
            for (auto &el : vector)
            {
                auto currentValue = source::DataVector::getNumericValue(el);
                if (currentValue == source::DataVector::invalidNumericValue)
                {
                    DEBUG_PRINTLN("Cannot normalize non numeric element. Skipping...");
                }
                else
                {
                    el = source::dataV(currentValue / length);
                }
            }
        }
        else
        {
            DEBUG_PRINTLN("Cannot normalize vector. Length is 0");
        }
    }
}
