#include "VoteClassifier.h"

namespace classifier
{
    source::dataVectorT VoteClassifier::weightedVoting(const source::dataMatrixT & classifiedExamples,
                                                       const algorithm::weightsVectorT & weights)
    {
        ASSERT(weights.size() == classifiedExamples.size());

        std::unordered_map<std::string, double> classNamesMap;
        const auto classNames = source::DataVector::getClassColumnAsString(classifiedExamples);

        for (size_t idx = 0; idx < classifiedExamples.size(); idx++)
        {
            const auto& name = classNames[idx];
            const auto& weight = weights[idx];
            auto result = classNamesMap.emplace(name, 1.0);
            if (!result.second)
            {
                result.first->second += weight;
            }
        }

        const auto it = std::max_element(classNamesMap.begin(), classNamesMap.end());
        ASSERT(it != classNamesMap.end());

        auto retVal(classifiedExamples.front());
        retVal.back() = it->first;
        return retVal;
    }

    source::dataVectorT VoteClassifier::normalVoting(const source::dataMatrixT & classifiedExamples)
    {
        const auto weights = algorithm::weightsVectorT(classifiedExamples.size(), 1.0);
        return weightedVoting(classifiedExamples, weights);
    }

    classificationResultVectorT VoteClassifier::checkClassification(
        const source::dataMatrixT & originalExamples,
        const source::dataMatrixT & classifiedExamples)
    {
        ASSERT(originalExamples.size() == classifiedExamples.size());
        const auto examplesSize = originalExamples.size();
        classificationResultVectorT retVal(examplesSize, false);

        for (size_t i = 0; i < examplesSize; i++)
        {
            const auto& originalClass = originalExamples[i].back();
            const auto& classifiedClass = classifiedExamples[i].back();
            retVal[i] = originalClass == classifiedClass;
        }

        return retVal;
    }
}
