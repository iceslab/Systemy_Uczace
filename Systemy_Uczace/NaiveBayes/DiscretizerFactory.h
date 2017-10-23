#pragma once
#include <memory>
#include "Discretizer.h"
#include "ClassicDiscretizer.h"
#include "UniformDiscretizer.h"

namespace discretizer
{
    enum DiscretizerTypeE
    {
        CLASSIC,
        UNIFORM,
        NONE
    };

    class DiscretizerFactory
    {
    public:
        static std::unique_ptr<Discretizer> getDiscretizer(DiscretizerTypeE type,
                                                           source::DataSource &source,
                                                           size_t buckets);
        static std::unique_ptr<Discretizer> getDiscretizer(DiscretizerTypeE type,
                                                           source::dataDescriptionT &descriptions,
                                                           source::dataMatrixT &matrix,
                                                           size_t buckets);
    private:
        DiscretizerFactory() = delete;
        ~DiscretizerFactory() = delete;
    };
}
