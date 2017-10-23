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
}
