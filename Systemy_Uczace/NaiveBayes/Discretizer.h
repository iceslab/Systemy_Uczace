#pragma once
#include "DataLoader.h"
#include <algorithm>

class Discretizer
{
public:
    Discretizer() = delete;
    ~Discretizer() = delete;

    static void discretize(loader::dataDescriptionElementT &description,
                           loader::dataColumnT &data,
                           size_t buckets);

private:
    static void discretizeInteger(loader::dataDescriptionElementT &description,
                                  loader::dataColumnT &data,
                                  size_t buckets);
    static void discretizeReal(loader::dataDescriptionElementT &description,
                               loader::dataColumnT &data,
                               size_t buckets);
};

