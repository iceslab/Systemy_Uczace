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

        template<typename T>
        static double calculateMean(const source::dataColumnT & data);
        template<typename T>
        static double calculateStddev(const source::dataColumnT & data, const double calculatedMean);
        template<typename T>
        static double calculateStddev(const source::dataColumnT & data);
        template<typename T>
        static double calculateVariance(const source::dataColumnT & data);

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

        const auto mean = calculateMean<T>(data);
        const auto stddev = calculateStddev<T>(data, mean);

        return NormalDistribution(mean, stddev);
    }

    template<typename T>
    inline double NormalDistribution::calculateMean(const source::dataColumnT & data)
    {
        static_assert(source::is_int<T>::value ||
                      source::is_double<T>::value,
                      "");
        const auto dataSize = data.size();
        double dataSum = 0.0;

        // Calculating mean
        for (const auto& element : data)
        {
            dataSum += std::get<T>(element);
        }
        return static_cast<double>(dataSum) / static_cast<double>(dataSize);
    }

    template<typename T>
    inline double NormalDistribution::calculateStddev(const source::dataColumnT & data,
                                                      const double calculatedMean)
    {
        static_assert(source::is_int<T>::value ||
                      source::is_double<T>::value,
                      "");
        const auto dataSize = data.size();
        double dataSum = 0.0;

        // Calculating stddev
        for (const auto& element : data)
        {
            dataSum += std::pow(std::abs(std::get<T>(element) - calculatedMean), 2.0);
        }
        return sqrt(dataSum / static_cast<double>(dataSize));
    }

    template<typename T>
    inline double NormalDistribution::calculateStddev(const source::dataColumnT & data)
    {
        static_assert(source::is_int<T>::value ||
                      source::is_double<T>::value,
                      "");
        const auto dataSize = data.size();
        double dataSum = 0.0;
        const double calculatedMean = calculateMean<T>(data);
        return calculateStddev<T>(data, calculatedMean);
    }

    template<typename T>
    inline double NormalDistribution::calculateVariance(const source::dataColumnT & data)
    {
        static_assert(source::is_int<T>::value ||
                      source::is_double<T>::value,
                      "");
        const double stddev = calculateStddev<T>(data);
        return stddev * stddev;
    }
}
