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
}