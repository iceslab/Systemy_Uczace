#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>
#include <tuple>
#include <variant>

namespace source
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

    typedef std::variant<std::string, std::pair<int, int>, std::pair<double, double>> descriptionV;
    typedef std::tuple<DataTypeE, std::string, std::vector<descriptionV>> dataDescriptionElementT;

    class DataDescription : public std::vector<dataDescriptionElementT>
    {
    public:
        DataDescription() = default;
        ~DataDescription() = default;
    };

    typedef DataDescription dataDescriptionT;
};