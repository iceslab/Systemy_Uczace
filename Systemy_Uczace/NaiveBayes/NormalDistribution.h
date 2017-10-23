#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <algorithm>
#include <vector>
#include "DataDescription.h"
#include "DataVector.h"

namespace distribution
{
    class NormalDistribution
    {
    public:
        NormalDistribution(double mean, double median, double stddev);
        ~NormalDistribution() = default;

        double getProbabilityDenisty(double x) const;

        template<typename T>
        static NormalDistribution getNormalDistribution(source::dataColumnT data);

    private:
        const double mean;
        const double median;
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
            return NormalDistribution(0.0, 0.0, 0.0);
        }

        const auto dataSize = data.size();
        double dataSum = 0.0;

        // Sorting data for median calculation
        std::vector<double> dataSorted;
        for(const auto& el : data)
        {
            dataSorted.emplace_back(std::get<T>(el));
        }
        std::sort(dataSorted.begin(), dataSorted.end());

        // Calculating median
        double median = 0.0;
        if (dataSize & 1)
        {
            median = static_cast<double>(dataSorted[dataSize / 2]);
        }
        else
        {
            auto firstElement = static_cast<double>(dataSorted[dataSize / 2 - 1]);
            auto secondElement = static_cast<double>(dataSorted[dataSize / 2]);
            median = static_cast<double>((firstElement + secondElement) / 2.0);
        }

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

        return NormalDistribution(mean, median, stddev);
    }
}