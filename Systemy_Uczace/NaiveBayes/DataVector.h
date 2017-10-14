#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>
#include <tuple>

namespace loader
{
    class DataVector : public std::vector<std::string>
    {
    public:
        DataVector() = default;
        ~DataVector() = default;
    };

    typedef std::vector<DataVector> dataMatrixT;
    typedef dataMatrixT trainingDataT;
    typedef dataMatrixT testDataT;
};