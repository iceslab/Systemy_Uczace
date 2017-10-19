#include "Discretizer.h"

void Discretizer::discretize(loader::dataDescriptionElementT & description,
                             loader::dataColumnT & data,
                             size_t buckets)
{
    const auto type = std::get<0>(description);
    switch (type)
    {
        case loader::INTEGER:
            break;
        case loader::REAL:
            break;
        default:
            DEBUG_PRINTLN("Data types other than INTEGER or REAL are not discretizable");
            break;
    }
}

void Discretizer::discretizeInteger(loader::dataDescriptionElementT & description,
                                    loader::dataColumnT & data,
                                    size_t buckets)
{
    std::get<0>(description) = loader::INTEGER_DISCRETE;

    auto minVal = std::numeric_limits<int>::max();
    auto maxVal = std::numeric_limits<int>::min();

    for (const auto& elV : data)
    {
        auto el = std::get<int>(elV.get());
        if (el < minVal)
        {
            minVal = el;
        }
        else if (el > maxVal)
        {
            maxVal = el;
        }
    }

    const auto diff = static_cast<size_t>(maxVal - minVal);
    // For integers make sure that buckets ranges are different
    if (diff < buckets)
    {
        DEBUG_PRINTLN("Changing bucket size form %zu to %zu", buckets, diff);
        buckets = diff;
    }

    const auto delta = diff / static_cast<double>(buckets);

    auto lowerBound = minVal;
    auto upperBoundD = minVal + delta;
    auto upperBound = static_cast<int>(upperBoundD);

    for (size_t i = 0; i < buckets; i++)
    {
        auto bounds = std::make_pair(lowerBound, upperBound);
        const loader::DataV boundsV(bounds);
        data[i] = std::reference_wrapper<const loader::DataV>(boundsV);
        lowerBound = upperBound;
        upperBoundD += delta;
        upperBound = static_cast<int>(std::round(upperBoundD));
    }

    upperBound = maxVal;
    auto bounds = std::make_pair(lowerBound, upperBound);
    const loader::DataV boundsV(bounds);

    data.back() = std::reference_wrapper<const loader::DataV>(boundsV);
}

void Discretizer::discretizeReal(loader::dataDescriptionElementT & description,
                                 loader::dataColumnT & data,
                                 size_t buckets)
{
    std::get<0>(description) = loader::REAL_DISCRETE;

    auto minVal = std::numeric_limits<double>::max();
    auto maxVal = std::numeric_limits<double>::min();

    for (const auto& elV : data)
    {
        auto el = std::get<double>(elV.get());
        if (el < minVal)
        {
            minVal = el;
        }
        else if (el > maxVal)
        {
            maxVal = el;
        }
    }

    const auto diff = maxVal - minVal;
    const auto delta = diff / static_cast<double>(buckets);

    auto lowerBound = minVal;
    auto upperBound = minVal + delta;

    for (size_t i = 0; i < buckets; i++)
    {
        auto bounds = std::make_pair(lowerBound, upperBound);
        const loader::DataV boundsV(bounds);
        data[i] = std::reference_wrapper<const loader::DataV>(boundsV);
        lowerBound = upperBound;
        upperBound += delta;
    }

    upperBound = maxVal;
    auto bounds = std::make_pair(lowerBound, upperBound);
    const loader::DataV boundsV(bounds);

    data.back() = std::reference_wrapper<const loader::DataV>(boundsV);
}
