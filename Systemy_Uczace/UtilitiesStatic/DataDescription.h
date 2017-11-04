#pragma once
#include "stdafx.h"

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
    typedef std::vector<descriptionV> descriptionVectorT;
    typedef std::tuple<DataTypeE, std::string, descriptionVectorT> dataDescriptionElementT;
    typedef std::vector<std::string> classNamesT;

    class DataDescription : public std::vector<dataDescriptionElementT>
    {
    public:
        DataDescription() = default;
        ~DataDescription() = default;

        size_t getLongestClassNameLength() const;

        dataDescriptionElementT getClassDescriptions() const;
        classNamesT getClassNames() const;
        static classNamesT getClassNames(const dataDescriptionElementT & description);
        size_t getClassNamesCount() const;
    };

    typedef DataDescription dataDescriptionT;
};