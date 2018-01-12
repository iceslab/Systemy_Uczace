#include "VoteClassifier.h"

namespace classifier
{
    source::testDataT VoteClassifier::weightedVoting(const partialResultsT & classifiedExamples,
                                                     const algorithm::weightsMatrixT & weights)
    {
        ASSERT(classifiedExamples.front().size() == weights.front().size());
        const auto & examplesSize = classifiedExamples.front().size();

        ASSERT(classifiedExamples.size() == weights.size());
        const auto& classifiersCount = classifiedExamples.size();

        source::testDataT retVal;
        retVal.reserve(examplesSize);

        for (size_t i = 0; i < examplesSize; i++)
        {

            source::dataMatrixT examples;
            examples.reserve(classifiersCount);
            algorithm::weightsVectorT weightsVector;
            weightsVector.reserve(classifiersCount);

            for (size_t j = 0; j < classifiersCount; j++)
            {
                examples.emplace_back(classifiedExamples[j][i]);
                weightsVector.emplace_back(weights[j][i]);
            }

            retVal.emplace_back(weightedVoting(examples, weightsVector));
        }

        return retVal;
    }

    source::testDataT VoteClassifier::normalVoting(const partialResultsT & classifiedExamples)
    {
        algorithm::weightsMatrixT weights(classifiedExamples.size(),
                                          algorithm::weightsVectorT(classifiedExamples.front().size(),
                                                                    1.0));
        return weightedVoting(classifiedExamples, weights);
    }

    source::dataVectorT VoteClassifier::weightedVoting(const source::dataMatrixT & classifiedExamples,
                                                       const algorithm::weightsVectorT & weights)
    {
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
        algorithm::weightsVectorT weights(classifiedExamples.size(), 1.0);
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
