#pragma once
#include "DataSource.h"
#include <algorithm>

class Discretizer
{
public:
    Discretizer(source::DataSource &source,
                size_t buckets);
    Discretizer(source::dataDescriptionT &descriptions,
                source::dataMatrixT &matrix,
                size_t buckets);
    ~Discretizer() = default;

    void discretize();

private:
    source::dataDescriptionT & descriptions;
    source::dataMatrixT & matrix;
    const size_t buckets;

    void discretize(source::dataDescriptionElementT &description,
                    source::dataColumnT &data);

    void discretizeInteger(source::dataDescriptionElementT &description,
                           source::dataColumnT &data);
    void discretizeReal(source::dataDescriptionElementT &description,
                        source::dataColumnT &data);

};

