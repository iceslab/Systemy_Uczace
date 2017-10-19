#include "NaiveBayesModel.h"

namespace model
{
    NaiveBayesModel::NaiveBayesModel(const source::testDataT & testData,
                                     const algorithm::NaiveBayesAlgorithm & algorithm) :
        p_xc(p_xc), p_c(p_c)
    {
        // Nothing to do
    }

    NaiveBayesModel::NaiveBayesModel(const source::testDataT & testData,
                                     const algorithm::attributesProbabilitiesT & p_xc,
                                     const algorithm::classProbabilitiesT & p_c) :
        testData(testData), p_xc(p_xc), p_c(p_c)
    {
        // Nothing to do
    }

    NaiveBayesModel::~NaiveBayesModel()
    {
        // Nothing to do
    }

    source::testDataT NaiveBayesModel::classify()
    {
        // P(C | X) = (P(X | C) * P(C)) / P(X)

        source::testDataT retVal;

        for (auto& row : testData)
        {
            retVal.emplace_back(classify(row));
        }

        return source::testDataT();
    }

    source::dataVectorT NaiveBayesModel::classify(const source::dataVectorT & data)
    {
        // P(C | X) = (P(X | C) * P(C)) / P(X)
        // P(X | C) =
        return source::dataVectorT();
    }
}