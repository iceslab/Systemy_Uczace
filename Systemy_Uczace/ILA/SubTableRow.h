#pragma once
#include "DataVector.h"
#include <set>

namespace algorithm
{
    class SubTableRow : public source::dataVectorT
    {
    public:
        typedef std::vector<SubTableRow> subTableT;
        typedef std::vector<subTableT> subTableVectorT;

        SubTableRow() = default;
        SubTableRow(const source::dataVectorT & data);
        ~SubTableRow() = default;
        bool isClassified;

        static source::dataColumnT getUniqueValuesForAttribute(const subTableVectorT & subTables,
                                                               size_t subTableIndex,
                                                               size_t attributeIndex);

        static source::dataColumnRefT getAttributeColumn(std::vector<SubTableRow>& str,
                                                         size_t attributeIndex);
    };


}
