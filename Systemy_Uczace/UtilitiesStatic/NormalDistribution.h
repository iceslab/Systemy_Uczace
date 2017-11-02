#pragma once
#include "DataDescription.h"
#include "DataVector.h"

namespace distribution
{
    class NormalDistribution
    {
    public:
        NormalDistribution(double mean, double stddev);
        ~NormalDistribution() = default;

        double getProbabilityDenisty(double x) const;

        template<typename T>
        static NormalDistribution getNormalDistribution(source::dataColumnT data);

    private:
        const double mean;
        const double stddev;
        const double variance;
    };
    template<typename T>
    inline NormalDistribution NormalDistribution::getNormalDistribution(source::dataColumnT data)
    {
        static_assert(source::is_int<T>::value ||
                      source::is_double<T>::value,
                      "");

        if (data.empty())
        {
            return NormalDistribution(0.0, 0.0);
        }

        const auto dataSize = data.size();
        double dataSum = 0.0;

        // Calculating mean
        for (const auto& element : data)
        {
            dataSum += std::get<T>(element);
        }
        auto mean = static_cast<double>(dataSum) / static_cast<double>(dataSize);

        // Calculating stddev
        dataSum = 0.0;
        for (const auto& element : data)
        {
            dataSum += std::pow(std::abs(std::get<T>(element) - mean), 2.0);
        }
        double stddev = sqrt(dataSum / static_cast<double>(dataSize));

        return NormalDistribution(mean, stddev);
    }
}
