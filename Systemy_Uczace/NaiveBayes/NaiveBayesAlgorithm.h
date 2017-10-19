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
        NaiveBayesAlgorithm(const loader::dataDescriptionT & description,
                            const loader::trainingDataT & trainingData);
        ~NaiveBayesAlgorithm();

        void produceModel() override;
    private:


        classProbabilitiesT getClassProbability(const loader::dataDescriptionT &descriptions,
                                                const loader::trainingDataT &trainingData);
        attributesProbabilitiesT getAttributesProbability(const loader::dataDescriptionT &descriptions,
                                                          const loader::trainingDataT &trainingData);

        elementProbabilitiesT getElementProbability(const loader::dataDescriptionElementT &description,
                                                    const loader::trainingColumnT &trainingData,
                                                    const loader::dataDescriptionElementT &classDescription,
                                                    const loader::trainingColumnT &classData);
        elementProbabilitiesT categoryProbability(const loader::dataDescriptionElementT &description,
                                                  const loader::trainingColumnT &trainingData,
                                                  const loader::dataDescriptionElementT &classDescription,
                                                  const loader::trainingColumnT &classData);
        elementProbabilitiesT numberProbability(const loader::dataDescriptionElementT &description,
                                                const loader::trainingColumnT &trainingData,
                                                const loader::dataDescriptionElementT &classDescription,
                                                const loader::trainingColumnT &classData);
        classProbabilitiesT classProbability(const loader::dataDescriptionElementT &description,
                                             const loader::trainingColumnT &trainingData);
    };
}
