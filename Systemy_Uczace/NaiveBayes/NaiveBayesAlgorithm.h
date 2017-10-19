#pragma once
#include "Algorithm.h"
#include "asserts.h"

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
        ~NaiveBayesAlgorithm();

        void produceModel() override;

        classProbabilitiesT getClassProbability();
        attributesProbabilitiesT getAttributesProbability();

    private:
        elementProbabilitiesT
            getElementProbability(const source::dataDescriptionElementT &description,
                                  const source::trainingColumnT &trainingData,
                                  const source::dataDescriptionElementT &classDescription,
                                  const source::trainingColumnT &classData);
        elementProbabilitiesT
            categoryProbability(const source::dataDescriptionElementT &description,
                                const source::trainingColumnT &trainingData,
                                const source::dataDescriptionElementT &classDescription,
                                const source::trainingColumnT &classData);
        elementProbabilitiesT
            numberProbability(const source::dataDescriptionElementT &description,
                              const source::trainingColumnT &trainingData,
                              const source::dataDescriptionElementT &classDescription,
                              const source::trainingColumnT &classData);
    };
}
