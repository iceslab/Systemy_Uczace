#pragma once
#include "Algorithm.h"
#include "asserts.h"
#include <unordered_map>
#include "NormalDistribution.h"

namespace algorithm
{
    using distribution::NormalDistribution;
    typedef std::vector<double> classProbabilitiesT;
    typedef std::vector<classProbabilitiesT> elementProbabilitiesT;
    typedef std::vector<elementProbabilitiesT> attributesProbabilitiesT;
    typedef std::vector<NormalDistribution> distributionsElementT;
    typedef std::vector<distributionsElementT> distributionsT;

    class NaiveBayesAlgorithm :
        public abstracts::Algorithm
    {
    public:
        NaiveBayesAlgorithm(const source::dataDescriptionT & description,
                            const source::trainingDataT & trainingData);
        ~NaiveBayesAlgorithm() = default;

        const classProbabilitiesT& getClassProbability() const;
        const attributesProbabilitiesT& getAttributesProbability() const;
        const distributionsT& getDistributions() const;
    private:
        mutable bool modelBuilt;
        mutable classProbabilitiesT p_c;
        mutable attributesProbabilitiesT p_xc;
        mutable distributionsT distributions;

        void calculateModel() const;
        void calculateClassProbability() const;
        void calculateAttributesProbability() const;

        elementProbabilitiesT
            getDiscreteElementProbability(const source::dataDescriptionElementT &description,
                                          const source::trainingColumnT &trainingData,
                                          const source::dataDescriptionElementT &classDescription,
                                          const source::trainingColumnT &classData) const;
        distributionsElementT
            getContinuousElementProbability(const source::dataDescriptionElementT &description,
                                            const source::trainingColumnT &trainingData,
                                            const source::dataDescriptionElementT &classDescription,
                                            const source::trainingColumnT &classData) const;
        template <typename T>
        elementProbabilitiesT
            categoryProbability(const source::dataDescriptionElementT &description,
                                const source::trainingColumnT &trainingData,
                                const source::dataDescriptionElementT &classDescription,
                                const source::trainingColumnT &classData) const;
        template <typename T>
        distributionsElementT
            numberProbability(const source::dataDescriptionElementT &description,
                              const source::trainingColumnT &trainingData,
                              const source::dataDescriptionElementT &classDescription,
                              const source::trainingColumnT &classData) const;
    };
}
