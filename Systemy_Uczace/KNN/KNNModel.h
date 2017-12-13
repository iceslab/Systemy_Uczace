#pragma once
#include "Model.h"
#include "asserts.h"
#include "KNNAlgorithm.h"

namespace model
{
    enum VotingMethodE
    {
        MAJORITY,
        WEIGHTED
    };

    typedef std::pair<source::dataVectorT, double> rowDistancePairT;

    class KNNModel : abstracts::Model
    {
    public:
        KNNModel(const source::testDataT & testData,
                 const algorithm::KNNAlgorithm & algorithm,
                 const source::distanceFn & distanceFunction,
                 const VotingMethodE votingType,
                 const size_t k);
        KNNModel(const source::testDataT & testData,
                 const source::trainingDataT & trainingData,
                 const source::distanceFn & distanceFunction,
                 const VotingMethodE votingType,
                 const size_t k);
        ~KNNModel() = default;

        virtual source::testDataT classify() override;

    private:
        typedef source::dataVectorT(KNNModel::*votingFn)(const source::dataVectorT &, const source::dataMatrixT &) const;
        const source::testDataT testData;
        const source::trainingDataT trainingData;
        const source::distanceFn distanceFunction;
        votingFn votingFunction;
        const size_t k;

        source::dataMatrixT closestNeighbours(const source::dataVectorT & row) const;

        source::dataVectorT majorityVoting(const source::dataVectorT & row,
                                           const source::dataMatrixT & neighbours) const;
        source::dataVectorT weightedVoting(const source::dataVectorT & row,
                                           const source::dataMatrixT & neighbours) const;
    };
}
