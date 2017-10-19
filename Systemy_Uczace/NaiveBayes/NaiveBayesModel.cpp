#include "NaiveBayesModel.h"

namespace model
{
    NaiveBayesModel::NaiveBayesModel(source::testDataT & testData,
                                     algorithm::NaiveBayesAlgorithm algorithm) :
        NaiveBayesModel(testData,
                        algorithm.getAttributesProbability(),
                        algorithm.getClassProbability())
    {

    }
    NaiveBayesModel::NaiveBayesModel(source::testDataT & testData,
                                     algorithm::attributesProbabilitiesT p_xc,
                                     algorithm::classProbabilitiesT p_c)
    {

    }

    NaiveBayesModel::~NaiveBayesModel()
    {

    }
}