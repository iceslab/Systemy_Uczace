#pragma once
#include "Model.h"
#include "DataVector.h"
#include "NaiveBayesAlgorithm.h"

namespace model
{
    class NaiveBayesModel :
        public abstracts::Model
    {
    public:
        NaiveBayesModel(const source::testDataT & testData,
                        const algorithm::NaiveBayesAlgorithm & algorithm);
        NaiveBayesModel(const source::testDataT & testData,
                        const algorithm::attributesProbabilitiesT & p_xc,
                        const algorithm::classProbabilitiesT & p_c,
                        const source::dataDescriptionT & descriptions,
                        const algorithm::distributionsT & distributions);
        ~NaiveBayesModel() = default;

        source::testDataT classify();

    private:
        source::dataVectorT classify(const source::dataVectorT & data);
        const source::testDataT testData;
        const algorithm::attributesProbabilitiesT p_xc;
        const algorithm::classProbabilitiesT p_c;
        const source::dataDescriptionT descriptions;
        const algorithm::distributionsT distributions;

        double getAttributeProbability(size_t attributeIndex,
                                       size_t elementIndex,
                                       size_t classIndex,
                                       const source::dataVectorT & data);
    };

}
