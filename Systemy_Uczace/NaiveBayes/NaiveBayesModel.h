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
        NaiveBayesModel(algorithm::attributesProbabilitiesT p_xc,
                        algorithm::classProbabilitiesT p_c);
        ~NaiveBayesModel();

        loader::testDataT classify(loader::testDataT testData);
    };
}
