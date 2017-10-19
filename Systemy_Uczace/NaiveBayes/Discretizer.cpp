#include "Discretizer.h"

void Discretizer::discretize(source::dataDescriptionT & descriptions,
                             source::dataMatrixT & matrix,
                             size_t buckets)
{
    const auto rowsCount = matrix.size();
    const auto columnsCount = descriptions.size();
    const auto classDescription = descriptions.back();
    std::vector<source::dataColumnT> columnData(columnsCount);

    // Create column references
    for (size_t row = 0; row < rowsCount; row++)
    {
        for (size_t column = 0; column < columnsCount; column++)
        {
            columnData[column].emplace_back(matrix[row][column]);
        }
    }

    // Discretization
    for (size_t column = 0; column < columnsCount; column++)
    {
        discretize(descriptions[column], columnData[column], buckets);
    }

    // Copy back values
    for (size_t row = 0; row < rowsCount; row++)
    {
        for (size_t column = 0; column < columnsCount; column++)
        {
            matrix[row][column] = columnData[column][row];
        }
    }
}

void Discretizer::discretize(source::dataDescriptionElementT & description,
                             source::dataColumnT & data,
                             size_t buckets)
{
    const auto type = std::get<0>(description);
    switch (type)
    {
        case source::INTEGER:
            break;
        case source::REAL:
            break;
        default:
            DEBUG_PRINTLN("Data types other than INTEGER or REAL are not discretizable");
            break;
    }
}

void Discretizer::discretizeInteger(source::dataDescriptionElementT & description,
                                    source::dataColumnT & data,
                                    size_t buckets)
{
    std::get<0>(description) = source::INTEGER_DISCRETE;

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
        const source::DataV boundsV(bounds);
        data[i] = std::reference_wrapper<const source::DataV>(boundsV);
        lowerBound = upperBound;
        upperBoundD += delta;
        upperBound = static_cast<int>(std::round(upperBoundD));
    }

    upperBound = maxVal;
    auto bounds = std::make_pair(lowerBound, upperBound);
    const source::DataV boundsV(bounds);

    data.back() = std::reference_wrapper<const source::DataV>(boundsV);
}

void Discretizer::discretizeReal(source::dataDescriptionElementT & description,
                                 source::dataColumnT & data,
                                 size_t buckets)
{
    std::get<0>(description) = source::REAL_DISCRETE;

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
        const source::DataV boundsV(bounds);
        data[i] = std::reference_wrapper<const source::DataV>(boundsV);
        lowerBound = upperBound;
        upperBound += delta;
    }

    upperBound = maxVal;
    auto bounds = std::make_pair(lowerBound, upperBound);
    const source::DataV boundsV(bounds);

    data.back() = std::reference_wrapper<const source::DataV>(boundsV);
}
