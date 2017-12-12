#include "stdafx.h"
#include "NormalDistribution.h"

namespace distribution
{
    NormalDistribution::NormalDistribution(double mean, double stddev) :
        mean(mean), stddev(stddev), variance(stddev * stddev)
    {
        // Nohing to do
    }

    double NormalDistribution::getProbabilityDenisty(double x) const
    {
        const double exponent = -(((x - mean) * (x - mean)) / (2.0L * variance));
        return 1.0L / sqrt(2.0L * M_PI * variance) * pow(M_E, exponent);
    }

    double NormalDistribution::calculateMean(const source::dataColumnT & data)
    {
        const auto dataSize = data.size();
        double dataSum = 0.0;

        // Calculating mean
        for (const auto& element : data)
        {
            const auto currentValue = source::DataVector::getNumericValue(element);
            if (currentValue == source::DataVector::invalidNumericValue)
            {
                DEBUG_PRINTLN("Cannot calculate mean");
                dataSum = 0.0;
                break;
            }
            dataSum += currentValue;
        }
        return static_cast<double>(dataSum) / static_cast<double>(dataSize);
    }

    double NormalDistribution::calculateStddev(const source::dataColumnT & data,
                                                      const double calculatedMean)
    {
        const auto dataSize = data.size();
        double dataSum = 0.0;

        // Calculating stddev
        for (const auto& element : data)
        {
            const auto currentValue = source::DataVector::getNumericValue(element);
            if (currentValue == source::DataVector::invalidNumericValue)
            {
                DEBUG_PRINTLN("Cannot calculate stddev");
                dataSum = 0.0;
                break;
            }
            dataSum += std::pow(std::abs(currentValue - calculatedMean), 2.0);
        }
        return sqrt(dataSum / static_cast<double>(dataSize));
    }

    double NormalDistribution::calculateStddev(const source::dataColumnT & data)
    {
        const auto dataSize = data.size();
        double dataSum = 0.0;
        const double calculatedMean = calculateMean(data);
        return calculateStddev(data, calculatedMean);
    }

    double NormalDistribution::calculateVariance(const source::dataColumnT & data)
    {
        const double stddev = calculateStddev(data);
        return stddev * stddev;
    }

    double NormalDistribution::calculateMean(const source::dataColumnRefT & data)
    {
        const auto dataSize = data.size();
        double dataSum = 0.0;

        // Calculating mean
        for (const auto& element : data)
        {
            const auto currentValue = source::DataVector::getNumericValue(element.get());
            if (currentValue == source::DataVector::invalidNumericValue)
            {
                DEBUG_PRINTLN("Cannot calculate mean");
                dataSum = 0.0;
                break;
            }
            dataSum += currentValue;
        }
        return static_cast<double>(dataSum) / static_cast<double>(dataSize);
    }

    double NormalDistribution::calculateStddev(const source::dataColumnRefT & data,
                                               const double calculatedMean)
    {
        const auto dataSize = data.size();
        double dataSum = 0.0;

        // Calculating stddev
        for (const auto& element : data)
        {
            const auto currentValue = source::DataVector::getNumericValue(element.get());
            if (currentValue == source::DataVector::invalidNumericValue)
            {
                DEBUG_PRINTLN("Cannot calculate stddev");
                dataSum = 0.0;
                break;
            }
            dataSum += std::pow(std::abs(currentValue - calculatedMean), 2.0);
        }
        return sqrt(dataSum / static_cast<double>(dataSize));
    }

    double NormalDistribution::calculateStddev(const source::dataColumnRefT & data)
    {
        const auto dataSize = data.size();
        double dataSum = 0.0;
        const double calculatedMean = calculateMean(data);
        return calculateStddev(data, calculatedMean);
    }

    double NormalDistribution::calculateVariance(const source::dataColumnRefT & data)
    {
        const double stddev = calculateStddev(data);
        return stddev * stddev;
    }

    void NormalDistribution::normalize(source::dataVectorT & vector)
    {
        const auto length = source::DataVector::vectorLength(vector);
        if (length != 0.0)
        {
            for (auto &el : vector)
            {
                auto currentValue = source::DataVector::getNumericValue(el);
                if (currentValue == source::DataVector::invalidNumericValue)
                {
                    DEBUG_PRINTLN("Cannot normalize non numeric element. Skipping...");
                }
                else
                {
                    el = source::dataV(currentValue / length);
                }
            }
        }
        else
        {
            DEBUG_PRINTLN("Cannot normalize vector. Length is 0");
        }
    }

    void NormalDistribution::normalize(source::dataMatrixT & data)
    {
        for (auto& el : data)
        {
            normalize(el);
        }
    }

    void NormalDistribution::standardize(source::dataMatrixT & data)
    {
        const auto attributesSize = data.size() - 1;
        for (size_t columnIdx = 0; columnIdx < attributesSize; columnIdx++)
        {
            auto column = source::DataVector::getAttributeColumn(data, columnIdx);
            const auto mean = distribution::NormalDistribution::calculateMean(column);
            const auto stddev = distribution::NormalDistribution::calculateStddev(column);

            for (auto& el : column)
            {
                const auto val = source::DataVector::getNumericValue(el.get());
                el.get() = (val - mean) / stddev;
            }
        }
    }
}
