#include "ILAModel.h"

namespace model
{
    ILAModel::ILAModel(const source::testDataT & testData, 
                       const algorithm::ILAAlgorithm & algorithm) :
        ILAModel(testData, algorithm.getRules())
    {
        // Nothing to do
    }

    ILAModel::ILAModel(const source::testDataT & testData, 
                       const algorithm::rulesVectorT & rules) :
        testData(testData), rules(rules)
    {
        // Nothing to do
    }

    source::testDataT ILAModel::classify()
    {
        source::testDataT retVal(testData);

        for (auto& row : retVal)
        {
            std::string className;
            auto classifyResult = algorithm::Rule::classify(rules, row, className);
            ASSERT_VERBOSE(classifyResult == true, "Data row was not classified");
            row.back() = className;
        }

        return retVal;
    }
}
