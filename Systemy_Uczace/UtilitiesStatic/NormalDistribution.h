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
        static NormalDistribution getNormalDistribution(const source::dataColumnT & data);

        static double calculateMean(const source::dataColumnT & data);
        static double calculateStddev(const source::dataColumnT & data, const double calculatedMean);
        static double calculateStddev(const source::dataColumnT & data);
        static double calculateVariance(const source::dataColumnT & data);

        static double calculateMean(const source::dataColumnRefT & data);
        static double calculateStddev(const source::dataColumnRefT & data, const double calculatedMean);
        static double calculateStddev(const source::dataColumnRefT & data);
        static double calculateVariance(const source::dataColumnRefT & data);

    private:
        const double mean;
        const double stddev;
        const double variance;
    };

    template<typename T>
    inline NormalDistribution NormalDistribution::getNormalDistribution(const source::dataColumnT & data)
    {
        static_assert(source::is_int<T>::value ||
                      source::is_double<T>::value,
                      "");

        if (data.empty())
        {
            return NormalDistribution(0.0, 0.0);
        }

        const auto mean = calculateMean(data);
        const auto stddev = calculateStddev(data, mean);

        return NormalDistribution(mean, stddev);
    }
}
