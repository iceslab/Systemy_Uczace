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

        // Integer discretization
        virtual void discretizeInteger(source::dataDescriptionElementT &description,
                                       source::dataColumnT &data) override;
        // Real discretization
        virtual void discretizeReal(source::dataDescriptionElementT &description,
                                    source::dataColumnT &data) override;
    };
}
