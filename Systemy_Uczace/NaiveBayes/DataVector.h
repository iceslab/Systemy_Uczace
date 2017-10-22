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

        //template<typename T>
        //static auto minMax(const dataColumnT& column);
    };

    typedef DataVector dataVectorT;
    typedef std::vector<dataVectorT> dataMatrixT;

    typedef dataMatrixT trainingDataT;
    typedef std::vector<std::reference_wrapper<const dataV>> constDataColumnT;
    typedef std::vector<std::reference_wrapper<dataV>> dataColumnT;
    typedef constDataColumnT trainingColumnT;

    typedef dataMatrixT testDataT;

    template<typename T>
    struct is_int : std::false_type {};
    template<>
    struct is_int<int> : std::true_type {};

    template<typename T>
    struct is_double : std::false_type {};
    template<>
    struct is_double<double> : std::true_type {};

    template<typename T>
    inline auto minMax(const dataColumnT & column)
    {
        static_assert(is_int<T>::value ||
                      is_double<T>::value,
                      "");

        auto minVal = std::numeric_limits<T>::max();
        auto maxVal = std::numeric_limits<T>::min();

        for (const auto& elV : column)
        {
            auto el = std::get<T>(elV.get());
            if (el < minVal)
            {
                minVal = el;
            }

            if (el > maxVal)
            {
                maxVal = el;
            }
        }

        return std::make_pair(minVal, maxVal);
    }
};