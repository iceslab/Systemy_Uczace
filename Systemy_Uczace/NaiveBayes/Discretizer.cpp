#include "Discretizer.h"

namespace discretizer
{
    Discretizer::Discretizer(source::DataSource & source,
                             size_t buckets) :
        Discretizer(source.getDataDescription(),
                    source.getDataMatrix(),
                    buckets)
    {
        // Nothing to do
    }

    Discretizer::Discretizer(source::dataDescriptionT & descriptions,
                             source::dataMatrixT & matrix,
                             size_t buckets) :
        descriptions(descriptions), matrix(matrix), buckets(buckets)
    {
        // Nothing to do
    }

    void Discretizer::discretize()
    {
        const auto rowsCount = matrix.size();
        const auto columnsCount = descriptions.size() - 1; // Last column ommited
        const auto classDescription = descriptions.back();
        std::vector<source::dataColumnT> columnData(columnsCount);

        // Create column references
        for (size_t row = 0; row < rowsCount; row++)
        {
            for (size_t column = 0; column < columnsCount; column++)
            {
                columnData[column].emplace_back(matrix[row][column]);
            }
        }

        // Discretization
        for (size_t column = 0; column < columnsCount; column++)
        {
            discretize(descriptions[column], columnData[column]);
            discretizeData(descriptions[column], columnData[column]);
        }
    }

    void Discretizer::discretize(source::dataDescriptionElementT & description,
                                 source::dataColumnT & data)
    {
        const auto type = std::get<0>(description);
        switch (type)
        {
            case source::INTEGER:
                discretizeInteger(description, data);
                break;
            case source::REAL:
                discretizeReal(description, data);
                break;
            default:
                DEBUG_PRINTLN("Data types other than INTEGER or REAL are not discretizable");
                break;
        }
    }

    void Discretizer::discretizeInteger(source::dataDescriptionElementT & description,
                                        source::dataColumnT & data)
    {
        std::get<0>(description) = source::INTEGER_DISCRETE;

        auto minVal = std::numeric_limits<int>::max();
        auto maxVal = std::numeric_limits<int>::min();

        for (const auto& elV : data)
        {
            auto el = std::get<int>(elV.get());
            if (el < minVal)
            {
                minVal = el;
            }

            if (el > maxVal)
            {
                maxVal = el;
            }
        }

        const auto diff = static_cast<size_t>(maxVal - minVal);
        auto effectiveBuckets = buckets;
        // For integers make sure that buckets ranges are different
        if (diff < effectiveBuckets)
        {
            DEBUG_PRINTLN("Changing bucket size from %zu to %zu", buckets, diff);
            effectiveBuckets = diff;
        }

        const auto delta = diff / static_cast<double>(effectiveBuckets);

        auto lowerBound = minVal;
        auto upperBoundD = minVal + delta;
        auto upperBound = static_cast<int>(upperBoundD);

        auto& bucketVector = std::get<2>(description);
        bucketVector.clear();

        for (size_t i = 0; i < effectiveBuckets - 1; i++)
        {
            auto bounds = std::make_pair(lowerBound, upperBound);
            const source::descriptionV boundsV(bounds);
            bucketVector.emplace_back(boundsV);
            lowerBound = upperBound;
            upperBoundD += delta;
            upperBound = static_cast<int>(std::round(upperBoundD));
        }

        upperBound = maxVal;
        auto bounds = std::make_pair(lowerBound, upperBound);
        const source::descriptionV boundsV(bounds);
        bucketVector.emplace_back(boundsV);
    }

    void Discretizer::discretizeReal(source::dataDescriptionElementT & description,
                                     source::dataColumnT & data)
    {
        std::get<0>(description) = source::REAL_DISCRETE;

        auto minVal = std::numeric_limits<double>::max();
        auto maxVal = std::numeric_limits<double>::lowest();

        for (const auto& elV : data)
        {
            auto el = std::get<double>(elV.get());
            if (el < minVal)
            {
                minVal = el;
            }

            if (el > maxVal)
            {
                maxVal = el;
            }
        }

        const auto diff = maxVal - minVal;
        const auto delta = diff / static_cast<double>(buckets);

        auto lowerBound = minVal;
        auto upperBound = minVal + delta;

        auto& bucketVector = std::get<2>(description);
        bucketVector.clear();

        for (size_t i = 0; i < buckets - 1; i++)
        {
            auto bounds = std::make_pair(lowerBound, upperBound);
            const source::descriptionV boundsV(bounds);
            bucketVector.emplace_back(boundsV);
            lowerBound = upperBound;
            upperBound += delta;
        }

        upperBound = maxVal;
        auto bounds = std::make_pair(lowerBound, upperBound);
        const source::descriptionV boundsV(bounds);
        bucketVector.emplace_back(boundsV);
    }

    void Discretizer::discretizeData(source::dataDescriptionElementT & description, source::dataColumnT & data)
    {
        const auto type = std::get<0>(description);
        switch (type)
        {
            case source::INTEGER_DISCRETE:
                discretizeDataTemplate<int>(description, data);
                break;
            case source::REAL_DISCRETE:
                discretizeDataTemplate<double>(description, data);
                break;
            default:
                DEBUG_PRINTLN("Data types other than INTEGER_DISCRETE or REAL_DISCRETE are not discretizable");
                break;
        }
    }

    template<typename T>
    void Discretizer::discretizeDataTemplate(source::dataDescriptionElementT & description,
                                             source::dataColumnT & data)
    {

        static_assert(is_int<T>::value ||
                      is_double<T>::value,
                      "");

        const auto& buckets = std::get<2>(description);
        for (auto& el : data)
        {
            bool anyFound = false;
            auto value = std::get<T>(el.get());
            for (size_t i = 0; i < buckets.size(); i++)
            {
                bool lowerBoundResult = false;
                bool upperBoundResult = false;
                const auto& boundPair = std::get<std::pair<T, T>>(buckets[i]);
                const auto& lowerBound = boundPair.first;
                const auto& upperBound = boundPair.second;

                // Lower bound always inclusive
                lowerBoundResult = value >= lowerBound;

                // Upper bound only on last bucket inclusive
                if (i == buckets.size() - 1)
                {
                    upperBoundResult = value <= upperBound;
                }
                else
                {
                    upperBoundResult = value < upperBound;
                }

                // Found bucket
                if (lowerBoundResult && upperBoundResult)
                {
                    el.get() = boundPair;
                    anyFound = true;
                    break;
                }
            }

            ASSERT_VERBOSE(anyFound, "Did not find any bucket to assign");
        }
    }
}
