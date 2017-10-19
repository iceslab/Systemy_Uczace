#include "Discretizer.h"

Discretizer::Discretizer(source::DataSource & source,
                         size_t buckets) :
    Discretizer(source.getDataDescription(),
                source.getDataMatrix(),
                buckets)
{
    // Nothing to do
}

Discretizer::Discretizer(source::dataDescriptionT & descriptions,
                         source::dataMatrixT & matrix,
                         size_t buckets) :
    descriptions(descriptions), matrix(matrix), buckets(buckets)
{
    // Nothing to do
}

void Discretizer::discretize()
{
    const auto rowsCount = matrix.size();
    const auto columnsCount = descriptions.size() - 1; // Last column ommited
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
        discretize(descriptions[column], columnData[column]);
    }
}

void Discretizer::discretize(source::dataDescriptionElementT & description,
                             source::dataColumnT & data)
{
    const auto type = std::get<0>(description);
    switch (type)
    {
        case source::INTEGER:
            discretizeInteger(description, data);
            break;
        case source::REAL:
            discretizeReal(description, data);
            break;
        default:
            DEBUG_PRINTLN("Data types other than INTEGER or REAL are not discretizable");
            break;
    }
}

void Discretizer::discretizeInteger(source::dataDescriptionElementT & description,
                                    source::dataColumnT & data)
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
    auto effectiveBuckets = buckets;
    // For integers make sure that buckets ranges are different
    if (diff < effectiveBuckets)
    {
        DEBUG_PRINTLN("Changing bucket size form %zu to %zu", buckets, diff);
        effectiveBuckets = diff;
    }

    const auto delta = diff / static_cast<double>(effectiveBuckets);

    auto lowerBound = minVal;
    auto upperBoundD = minVal + delta;
    auto upperBound = static_cast<int>(upperBoundD);

    auto& bucketVector = std::get<2>(description);
    bucketVector.clear();

    for (size_t i = 0; i < effectiveBuckets - 1; i++)
    {
        auto bounds = std::make_pair(lowerBound, upperBound);
        const source::descriptionV boundsV(bounds);
        bucketVector.emplace_back(boundsV);
        lowerBound = upperBound;
        upperBoundD += delta;
        upperBound = static_cast<int>(std::round(upperBoundD));
    }

    upperBound = maxVal;
    auto bounds = std::make_pair(lowerBound, upperBound);
    const source::descriptionV boundsV(bounds);
    bucketVector.emplace_back(boundsV);
}

void Discretizer::discretizeReal(source::dataDescriptionElementT & description,
                                 source::dataColumnT & data)
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

    auto& bucketVector = std::get<2>(description);
    bucketVector.clear();

    for (size_t i = 0; i < buckets - 1; i++)
    {
        auto bounds = std::make_pair(lowerBound, upperBound);
        const source::descriptionV boundsV(bounds);
        bucketVector.emplace_back(boundsV);
        lowerBound = upperBound;
        upperBound += delta;
    }

    upperBound = maxVal;
    auto bounds = std::make_pair(lowerBound, upperBound);
    const source::descriptionV boundsV(bounds);
    bucketVector.emplace_back(boundsV);
}
