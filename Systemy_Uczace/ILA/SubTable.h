#pragma once
#include "SubTableRow.h"

namespace algorithm
{
    class SubTable : public std::vector<SubTableRow>
    {
    public:
        SubTable() = default;
        ~SubTable() = default;

        static source::dataColumnT
            getUniqueValuesForAttribute(const std::vector<SubTable> & subTables,
                                        size_t subTableIndex,
                                        size_t attributeIndex);

        source::dataColumnRefT getAttributeColumn(size_t attributeIndex);

        bool allRowsClassified() const;
        void printClassified() const;
        void setClassified(size_t index);
    private:
        size_t classifiedRows;
    };

    typedef SubTable subTableT;
    typedef std::vector<subTableT> subTableVectorT;
}
