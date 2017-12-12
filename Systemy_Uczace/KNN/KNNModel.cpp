#include "KNNModel.h"

namespace model
{
    KNNModel::KNNModel(const source::testDataT & testData,
                       const algorithm::KNNAlgorithm & algorithm,
                       const source::distanceFn & distanceFunction) :
        distanceFunction(distanceFunction)
    {
        // Nothing to do
    }

    source::testDataT KNNModel::classify()
    {
        return source::testDataT();
    }

    source::dataVectorT KNNModel::majorityVoting(const source::dataVectorT & row,
                                                 const source::dataMatrixT & neighbours)
    {
        std::unordered_map<std::string, size_t> classNamesMap;
        const auto classNames = source::DataVector::getClassColumnAsString(neighbours);

        for (const auto& name : classNames)
        {
            auto result = classNamesMap.emplace(name, 1);
            if (!result.second)
            {
                result.first->second++;
            }

        }

        const auto it = std::max_element(classNamesMap.begin(), classNamesMap.end());
        ASSERT(it != classNamesMap.end());

        auto retVal(row);
        retVal.back() = it->first;
        return retVal;
    }

    source::dataVectorT KNNModel::weightedVoting(const source::dataVectorT & row,
                                                 const source::dataMatrixT & neighbours)
    {
        std::unordered_map<std::string, double> classNamesMap;
        const auto classNames = source::DataVector::getClassColumnAsString(neighbours);

        for (size_t i = 0; i < neighbours.size(); i++)
        {
            const auto& name = classNames[i];
            const auto& neighbour = neighbours[i];
            const auto value = distanceFunction(row, neighbour);

            auto result = classNamesMap.emplace(name, value);
            if (!result.second)
            {
                result.first->second += value;
            }

        }

        const auto it = std::max_element(classNamesMap.begin(), classNamesMap.end());
        ASSERT(it != classNamesMap.end());

        auto retVal(row);
        retVal.back() = it->first;
        return retVal;
    }


}
