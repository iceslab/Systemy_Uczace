#pragma once
#include "Discretizer.h"

namespace discretizer
{
    class ClassicDiscretizer : public Discretizer
    {
    public:
        ClassicDiscretizer(source::DataSource &source,
                           size_t buckets);
        ClassicDiscretizer(source::dataDescriptionT &descriptions,
                           source::dataMatrixT &matrix,
                           size_t buckets);
        virtual ~ClassicDiscretizer() = default;
    };
}
