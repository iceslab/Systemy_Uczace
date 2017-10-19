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

namespace source
{
    typedef std::variant<std::string, int, double, std::pair<int, int>, std::pair<double, double>> dataV;

    class DataVector : public std::vector<dataV>
    {
    public:
        DataVector() = default;
        DataVector(size_t size) : std::vector<dataV>(size) {};
        ~DataVector() = default;
    };

    typedef DataVector dataVectorT;
    typedef std::vector<dataVectorT> dataMatrixT;

    typedef dataMatrixT trainingDataT;
    typedef std::vector<std::reference_wrapper<const dataV>> constDataColumnT;
    typedef std::vector<std::reference_wrapper<dataV>> dataColumnT;
    typedef constDataColumnT trainingColumnT;

    typedef dataMatrixT testDataT;
};