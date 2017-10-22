#pragma once
#include "Discretizer.h"

namespace discretizer
{
    class UniformDiscretizer : public Discretizer
    {
    public:
        UniformDiscretizer(source::DataSource &source,
                           size_t buckets);
        UniformDiscretizer(source::dataDescriptionT &descriptions,
                           source::dataMatrixT &matrix,
                           size_t buckets);
        virtual ~UniformDiscretizer() = default;

        // Integer discretization
        virtual void discretizeInteger(source::dataDescriptionElementT &description,
                                       source::dataColumnT &data) override;
        // Real discretization
        virtual void discretizeReal(source::dataDescriptionElementT &description,
                                    source::dataColumnT &data) override;
    };
}
