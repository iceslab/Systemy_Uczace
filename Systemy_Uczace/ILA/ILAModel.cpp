#include "ILAModel.h"

namespace model
{
    ILAModel::ILAModel(const source::testDataT & testData, 
                       const algorithm::ILAAlgorithm & algorithm,
                       bool performSimpleClassification) :
        ILAModel(testData, algorithm.getRules(), performSimpleClassification)
    {
        // Nothing to do
    }

    ILAModel::ILAModel(const source::testDataT & testData, 
                       const algorithm::rulesVectorT & rules,
                       bool performSimpleClassification) :
        testData(testData), rules(rules), performSimpleClassification(performSimpleClassification)
    {
        // Nothing to do
    }

    source::testDataT ILAModel::classify()
    {
        source::testDataT retVal(testData);

        for (auto& row : retVal)
        {
            std::string className;

            bool classifyResult = false;
            if (performSimpleClassification)
            {
                classifyResult = algorithm::Rule::classifySimple(rules, row, className);
            }
            else
            {
                classifyResult = algorithm::Rule::classifyVote(rules, row, className);
            }

            if (classifyResult == false)
            {
                algorithm::Rule::classifyOccurences(testData, className);
            }
            row.back() = className;
        }

        return retVal;
    }
}
