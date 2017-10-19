#pragma once
#include "DataSource.h"
#include <algorithm>

class Discretizer
{
public:
    Discretizer() = delete;
    ~Discretizer() = delete;

    static void discretize(source::dataDescriptionT &descriptions,
                           source::dataMatrixT &matrix,
                           size_t buckets);

private:
    static void discretize(source::dataDescriptionElementT &description,
                           source::dataColumnT &data,
                           size_t buckets);

    static void discretizeInteger(source::dataDescriptionElementT &description,
                                  source::dataColumnT &data,
                                  size_t buckets);
    static void discretizeReal(source::dataDescriptionElementT &description,
                               source::dataColumnT &data,
                               size_t buckets);
};

