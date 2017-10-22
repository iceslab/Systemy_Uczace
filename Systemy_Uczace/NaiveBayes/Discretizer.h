#pragma once
#include "DataSource.h"
#include <algorithm>

namespace discretizer
{

    class Discretizer
    {
    public:
        Discretizer(source::DataSource &source,
                    size_t buckets);
        Discretizer(source::dataDescriptionT &descriptions,
                    source::dataMatrixT &matrix,
                    size_t buckets);
        virtual ~Discretizer() = default;

        void discretize();

    private:
        source::dataDescriptionT & descriptions;
        source::dataMatrixT & matrix;
        const size_t buckets;

        /// Discretizer methods for descriptions

        // General discretization method
        void discretize(source::dataDescriptionElementT &description,
                        source::dataColumnT &data);

        // Integer discretization
        virtual void discretizeInteger(source::dataDescriptionElementT &description,
                                       source::dataColumnT &data) = 0;
        // Real discretization
        virtual void discretizeReal(source::dataDescriptionElementT &description,
                                    source::dataColumnT &data) = 0;

        /// Discretizer methods for data

        // General data discretization method
        void discretizeData(source::dataDescriptionElementT &description,
                            source::dataColumnT &data);
        // Template data discretization
        template<typename T>
        void discretizeDataTemplate(source::dataDescriptionElementT &description,
                                    source::dataColumnT &data);


        template<typename T>
        struct is_int : std::false_type {};
        template<>
        struct is_int<int> : std::true_type {};

        template<typename T>
        struct is_double : std::false_type {};
        template<>
        struct is_double<double> : std::true_type {};
    };
}