#pragma once
#include "asserts.h"
#include "DataDescription.h"
#include "NormalDistribution.h"

namespace source
{
    typedef std::variant<std::string, int, double, std::pair<int, int>, std::pair<double, double>> dataV;

    typedef std::vector<std::reference_wrapper<const dataV>> constDataColumnRefT;
    typedef std::vector<std::reference_wrapper<dataV>> dataColumnRefT;
    typedef std::vector<dataV> dataColumnT;
    typedef constDataColumnRefT trainingColumnT;
    typedef std::vector<std::string> classAsStringColumnT;

    class DataVector : public std::vector<dataV>
    {
    public:
        DataVector() = default;
        DataVector(size_t size) : std::vector<dataV>(size)
        {};
        ~DataVector() = default;

        const dataV& getClass() const;
        std::string getClassName() const;

        std::string toString() const;

        static const double invalidNumericValue;

        static dataColumnRefT getAttributeColumn(std::vector<DataVector> & dc,
                                                 size_t attributeIndex);

        static dataColumnRefT getClassColumn(std::vector<DataVector> & dc);
        static classAsStringColumnT getClassColumnAsString(const std::vector<DataVector> & dc);
        static std::string toString(const dataV & data);

        static double getNumericValue(const dataV & element);
        static double vectorLength(const DataVector & vector);

        static void normalize(DataVector & vector);
        static void normalize(std::vector<DataVector> & data);
        static void standardize(std::vector<DataVector> & data);

        static double euclideanDistance(const source::dataVectorT & v1, const source::dataVectorT & v2);
        static double manhattanDistance(const source::dataVectorT & v1, const source::dataVectorT & v2);
        static double minkowskiDistance(const source::dataVectorT & v1, const source::dataVectorT & v2, const double p);
    };

    typedef DataVector dataVectorT;
    typedef std::vector<dataVectorT> dataMatrixT;

    typedef dataMatrixT trainingDataT;
    typedef dataMatrixT testDataT;

    template<typename T>
    struct is_int : std::false_type
    {};
    template<>
    struct is_int<int> : std::true_type
    {};

    template<typename T>
    struct is_double : std::false_type
    {};
    template<>
    struct is_double<double> : std::true_type
    {};

    template<typename T>
    inline auto minMax(const dataColumnRefT & column)
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