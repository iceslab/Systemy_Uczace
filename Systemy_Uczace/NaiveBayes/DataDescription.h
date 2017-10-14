#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>
#include <tuple>

namespace loader
{
    enum DataTypeE
    {
        CATEGORY,
        INTEGER,
        INTEGER_DISCRETE,
        REAL,
        REAL_DISCRETE,
        UNDEFINED
    };

    typedef std::tuple<DataTypeE, std::string, std::vector<std::string>> dataDescriptionElementT;

    class DataDescription : public std::vector<dataDescriptionElementT>
    {
    public:
        DataDescription() = default;
        ~DataDescription() = default;
    };

    typedef DataDescription dataDescriptionT;
};