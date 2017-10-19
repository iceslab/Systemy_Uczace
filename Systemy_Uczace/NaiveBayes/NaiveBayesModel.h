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
                        const source::dataDescriptionT & descriptions);
        ~NaiveBayesModel() = default;

        source::testDataT classify();

    private:
        source::dataVectorT classify(const source::dataVectorT & data);
        const source::testDataT testData;
        const algorithm::attributesProbabilitiesT p_xc;
        const algorithm::classProbabilitiesT p_c;
        const source::dataDescriptionT descriptions;

        size_t getElementIndex(source::DataTypeE type,
                               const source::dataV & data,
                               const source::dataDescriptionElementT & description);

        template<typename T>
        size_t getElementIndex(const source::dataV & data,
                               const source::dataDescriptionElementT & description);
    };

}
