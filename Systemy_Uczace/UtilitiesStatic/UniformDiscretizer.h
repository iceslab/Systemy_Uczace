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
                                       source::dataColumnRefT &data) override;
        // Real discretization
        virtual void discretizeReal(source::dataDescriptionElementT &description,
                                    source::dataColumnRefT &data) override;
    private:
        template<typename T>
        void discretizeTemplate(source::dataDescriptionElementT &description,
                                source::dataColumnRefT &data);
    };
}
