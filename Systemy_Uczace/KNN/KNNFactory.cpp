#include "KNNFactory.h"


namespace model
{
    std::unique_ptr<KNNModel> KNNFactory::getKNNModel(const source::testDataT & testData,
                                                      const algorithm::KNNAlgorithm & algorithm,
                                                      const DistanceFunctionE functionType,
                                                      const VotingMethodE votingType,
                                                      const size_t k,
                                                      const double power)
    {
        using namespace std::placeholders;

        if (functionType == MINKOWSKI)
        {
            ASSERT_VERBOSE(power > 0.0, "Power must be greater than 0. Passed %f", power);
        }

        source::distanceFn function;

        switch (functionType)
        {
        case EUCLIDEAN:
            function = &source::DataVector::euclideanDistance;
            break;
        case MANHATTAN:
            function = &source::DataVector::manhattanDistance;
            break;
        case MINKOWSKI:
            function = std::bind(&source::DataVector::minkowskiDistance, _1, _2, power);
            break;
        default:
            FATAL_ERROR_VERBOSE("Invalid enum value: %d", functionType);
            break;
        }

        return std::make_unique<KNNModel>(testData, algorithm, function, votingType, k);
    }
}
