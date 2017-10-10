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
        REAL,
        UNDEFINED
    };

    class DataDescription : public std::vector<std::tuple<DataTypeE, std::string, std::vector<std::string>>>
    {
    public:
        DataDescription() = default;
        ~DataDescription() = default;
    };
};