#include "ClassicDiscretizer.h"

namespace discretizer
{
    ClassicDiscretizer::ClassicDiscretizer(source::DataSource & source,
                                           size_t buckets) :
        Discretizer(source, buckets)
    {
        // Nothing to do
    }

    ClassicDiscretizer::ClassicDiscretizer(source::dataDescriptionT & descriptions,
                                           source::dataMatrixT & matrix,
                                           size_t buckets) :
        Discretizer(descriptions, matrix, buckets)
    {
        // Nothing to do
    }

    void ClassicDiscretizer::discretizeInteger(source::dataDescriptionElementT & description,
                                        source::dataColumnRefT & data)
    {
        std::get<0>(description) = source::INTEGER_DISCRETE;
        auto minMax = source::minMax<int>(data);
        auto minVal = minMax.first;
        auto maxVal = minMax.second;

        const auto diff = static_cast<size_t>(maxVal - minVal);
        auto effectiveBuckets = buckets;
        // For integers make sure that buckets ranges are different
        if (diff < effectiveBuckets)
        {
            DEBUG_PRINTLN_VERBOSE_INFO("Changing bucket size from %zu to %zu", buckets, diff);
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

    void ClassicDiscretizer::discretizeReal(source::dataDescriptionElementT & description,
                                     source::dataColumnRefT & data)
    {
        std::get<0>(description) = source::REAL_DISCRETE;
        auto minMax = source::minMax<double>(data);
        auto minVal = minMax.first;
        auto maxVal = minMax.second;

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
}