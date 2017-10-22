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

    protected:
        source::dataDescriptionT & descriptions;
        source::dataMatrixT & matrix;
        const size_t buckets;

        /// Discretizer methods for descriptions

        // General discretization method
        void discretize(source::dataDescriptionElementT &description,
                        source::dataColumnRefT &data);

        // Integer discretization
        virtual void discretizeInteger(source::dataDescriptionElementT &description,
                                       source::dataColumnRefT &data) = 0;
        // Real discretization
        virtual void discretizeReal(source::dataDescriptionElementT &description,
                                    source::dataColumnRefT &data) = 0;

        /// Discretizer methods for data

        // General data discretization method
        void discretizeData(source::dataDescriptionElementT &description,
                            source::dataColumnRefT &data);
        // Template data discretization
        template<typename T>
        void discretizeDataTemplate(source::dataDescriptionElementT &description,
                                    source::dataColumnRefT &data);
    };
}