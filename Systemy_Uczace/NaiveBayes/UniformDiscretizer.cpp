#include "UniformDiscretizer.h"

namespace discretizer
{
    UniformDiscretizer::UniformDiscretizer(source::DataSource & source,
                                           size_t buckets) :
        Discretizer(source, buckets)
    {
        // Nothing to do
    }

    UniformDiscretizer::UniformDiscretizer(source::dataDescriptionT & descriptions,
                                           source::dataMatrixT & matrix,
                                           size_t buckets) :
        Discretizer(descriptions, matrix, buckets)
    {
        // Nothing to do
    }

    void UniformDiscretizer::discretizeInteger(source::dataDescriptionElementT & description,
                                               source::dataColumnRefT & data)
    {
        std::get<0>(description) = source::INTEGER_DISCRETE;
        discretizeTemplate<int>(description, data);
    }

    void UniformDiscretizer::discretizeReal(source::dataDescriptionElementT & description,
                                            source::dataColumnRefT & data)
    {
        std::get<0>(description) = source::REAL_DISCRETE;
        discretizeTemplate<double>(description, data);
    }

    template<typename T>
    void UniformDiscretizer::discretizeTemplate(source::dataDescriptionElementT &description,
                                                source::dataColumnRefT &data)
    {
        static_assert(source::is_int<T>::value ||
                      source::is_double<T>::value,
                      "");
        auto minMax = source::minMax<T>(data);
        auto minVal = minMax.first;
        auto maxVal = minMax.second;

        const auto diff = static_cast<size_t>(maxVal - minVal);
        auto effectiveBuckets = buckets;
        // For integers make sure that buckets ranges are different
        if (diff < effectiveBuckets)
        {
            DEBUG_PRINTLN("Changing bucket size from %zu to %zu", buckets, diff);
            effectiveBuckets = diff;
        }

        const auto fullBuckets = data.size() % effectiveBuckets;
        const auto minNumbersPerBucket = static_cast<size_t>(
            std::floor(static_cast<double>(data.size()) /
                       static_cast<double>(effectiveBuckets)));
        const auto maxNumbersPerBucket = static_cast<size_t>(
            std::ceil(static_cast<double>(data.size()) /
                      static_cast<double>(effectiveBuckets)));
        auto lowerBound = minVal;
        decltype(lowerBound) upperBound;

        // Copying values manually due to reference_wrapper used in data
        source::dataColumnT sortedData;
        for (const auto& el : data)
        {
            sortedData.emplace_back(el.get());
        }
        std::sort(sortedData.begin(), sortedData.end());

        // Vector for buckets bounds
        auto& bucketVector = std::get<2>(description);
        bucketVector.clear();

        size_t allElCount = 0;
        for (size_t i = 0; i < effectiveBuckets - 1; i++)
        {
            if (i <= fullBuckets)
            {
                allElCount += maxNumbersPerBucket;
            }
            else
            {
                allElCount += minNumbersPerBucket;
            }
            ASSERT(allElCount < sortedData.size());

            upperBound = std::get<T>(sortedData[allElCount]);
            auto bounds = std::make_pair(lowerBound, upperBound);
            const source::descriptionV boundsV(bounds);
            bucketVector.emplace_back(boundsV);
            lowerBound = upperBound;
        }

        upperBound = maxVal;
        auto bounds = std::make_pair(lowerBound, upperBound);
        const source::descriptionV boundsV(bounds);
        bucketVector.emplace_back(boundsV);
    }
}