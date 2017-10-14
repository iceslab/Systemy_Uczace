#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>
#include <tuple>
#include <variant>
#include "asserts.h"
#include "DataDescription.h"

namespace loader
{
    typedef std::variant<std::string, int, double, std::pair<int, int>, std::pair<double, double>> DataV;

    class DataVector : public std::vector<DataV>
    {
    public:
        DataVector() = default;
        DataVector(size_t size) : std::vector<DataV>(size) {};
        ~DataVector() = default;
    };

    typedef std::vector<DataVector> dataMatrixT;

    typedef dataMatrixT trainingDataT;
    typedef std::vector<std::reference_wrapper<const DataV>> dataColumnT;
    typedef dataColumnT trainingColumnT;

    typedef dataMatrixT testDataT;
};