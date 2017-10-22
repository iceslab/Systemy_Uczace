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
}