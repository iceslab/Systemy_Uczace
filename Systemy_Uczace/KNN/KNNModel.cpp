#include "KNNModel.h"

namespace model
{
    KNNModel::KNNModel(const source::testDataT & testData,
                       const algorithm::KNNAlgorithm & algorithm,
                       const source::distanceFn & distanceFunction,
                       const VotingMethodE votingType,
                       const size_t k) :
        KNNModel(testData,
                 algorithm.getTrainingData(),
                 distanceFunction,
                 votingType,
                 k)

    {
        // Nothing to do
    }

    KNNModel::KNNModel(const source::testDataT & testData,
                       const source::trainingDataT & trainingData,
                       const source::distanceFn & distanceFunction,
                       const VotingMethodE votingType,
                       const size_t k) :
        testData(testData),
        trainingData(trainingData),
        distanceFunction(distanceFunction),
        k(k)
    {
        ASSERT(k >= 1);
        switch (votingType)
        {
        case MAJORITY:
            votingFunction = &KNNModel::majorityVoting;
            break;
        case WEIGHTED:
            votingFunction = &KNNModel::weightedVoting;
            break;
        default:
            FATAL_ERROR_VERBOSE("Invalid voting type: %d", votingType);
            break;
        }
    }

    source::testDataT KNNModel::classify()
    {
        source::testDataT retVal;
        retVal.reserve(testData.size());

        for (const auto& testRow : testData)
        {
            const auto neighbours = closestNeighbours(testRow);
            const auto votingResult = (this->*votingFunction)(testRow, neighbours);
            retVal.emplace_back(votingResult);
        }

        return retVal;
    }

    source::dataMatrixT KNNModel::closestNeighbours(const source::dataVectorT & row) const
    {
        std::vector<rowDistancePairT> rowDistancePairs;
        rowDistancePairs.reserve(trainingData.size());

        for (const auto& el : trainingData)
        {
            const auto distance = distanceFunction(row, el);
            rowDistancePairs.emplace_back(el, distance);
        }


        std::sort(rowDistancePairs.begin(),
                  rowDistancePairs.end(),
                  [](const rowDistancePairT & p1, const rowDistancePairT & p2)->bool
        {
            return p1.second < p2.second;
        });

        ASSERT(rowDistancePairs.size() >= k);
        source::dataMatrixT retVal;
        retVal.reserve(k);

        for (size_t i = 0; i < k; i++)
        {
            retVal.emplace_back(rowDistancePairs[i].first);
        }

        return retVal;
    }

    source::dataVectorT KNNModel::majorityVoting(const source::dataVectorT & row,
                                                 const source::dataMatrixT & neighbours) const
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
                                                 const source::dataMatrixT & neighbours) const
    {
        std::unordered_map<std::string, double> classNamesMap;
        const auto classNames = source::DataVector::getClassColumnAsString(neighbours);

        for (size_t i = 0; i < neighbours.size(); i++)
        {
            const auto& name = classNames[i];
            const auto& neighbour = neighbours[i];
            auto value = distanceFunction(row, neighbour);
            if (value == 0.0)
            {
                value = std::numeric_limits<decltype(value)>::max();
            }
            else
            {
                value = 1.0 / value;
            }

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
