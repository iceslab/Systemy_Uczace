#pragma once
#include "Model.h"
#include "NaiveBayesAlgorithm.h"
#include "DataVector.h"


class NaiveBayesModel :
    public interfaces::Model
{
public:
    NaiveBayesModel(NaiveBayesAlgorithm::attributesProbabilitiesT p_xc,
                    NaiveBayesAlgorithm::classProbabilitiesT p_c);
    ~NaiveBayesModel();

    loader::testDataT classify(loader::testDataT testData);
};

