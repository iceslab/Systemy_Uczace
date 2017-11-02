#include "stdafx.h"
#include "DiscretizerFactory.h"

namespace discretizer
{
    std::unique_ptr<Discretizer> discretizer::DiscretizerFactory::getDiscretizer(
        DiscretizerTypeE type,
        source::DataSource & source,
        size_t buckets)
    {
        switch (type)
        {
            case CLASSIC:
                return std::make_unique<ClassicDiscretizer>(source, buckets);
            case UNIFORM:
                return std::make_unique<UniformDiscretizer>(source, buckets);
            default:
                FATAL_ERROR_VERBOSE("Not recognizable type. "
                                    "Valid types are CLASSIC (%d) and UNIFORM (%d)",
                                    CLASSIC,
                                    UNIFORM);
                break;
        }

        // Supress warning C4715
        return std::unique_ptr<ClassicDiscretizer>(nullptr);
    }

    std::unique_ptr<Discretizer> discretizer::DiscretizerFactory::getDiscretizer(
        DiscretizerTypeE type,
        source::dataDescriptionT & descriptions,
        source::dataMatrixT & matrix,
        size_t buckets)
    {
        switch (type)
        {
            case CLASSIC:
                return std::make_unique<ClassicDiscretizer>(descriptions, matrix, buckets);
            case UNIFORM:
                return std::make_unique<UniformDiscretizer>(descriptions, matrix, buckets);
            default:
                FATAL_ERROR_VERBOSE("Not recognizable type. "
                                    "Valid types are CLASSIC (%d) and UNIFORM (%d)",
                                    CLASSIC,
                                    UNIFORM);
                break;
        }

        // Supress warning C4715
        return std::unique_ptr<ClassicDiscretizer>(nullptr);
    }
}
