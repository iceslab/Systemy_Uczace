#pragma once
#include "Algorithm.h"
#include "asserts.h"
#include <unordered_map>

namespace algorithm
{
    typedef std::vector<double> classProbabilitiesT;
    typedef std::vector<classProbabilitiesT> elementProbabilitiesT;
    typedef std::vector<elementProbabilitiesT> attributesProbabilitiesT;

    class NaiveBayesAlgorithm :
        public abstracts::Algorithm
    {
    public:
        NaiveBayesAlgorithm(const source::dataDescriptionT & description,
                            const source::trainingDataT & trainingData);
        ~NaiveBayesAlgorithm() = default;

        classProbabilitiesT getClassProbability() const;
        attributesProbabilitiesT getAttributesProbability() const;

    private:
        elementProbabilitiesT
            getElementProbability(const source::dataDescriptionElementT &description,
                                  const source::trainingColumnT &trainingData,
                                  const source::dataDescriptionElementT &classDescription,
                                  const source::trainingColumnT &classData) const;
        template <typename T>
        elementProbabilitiesT
            categoryProbability(const source::dataDescriptionElementT &description,
                                const source::trainingColumnT &trainingData,
                                const source::dataDescriptionElementT &classDescription,
                                const source::trainingColumnT &classData) const;
    };
}
