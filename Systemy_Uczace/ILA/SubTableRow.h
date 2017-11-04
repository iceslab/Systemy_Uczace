#pragma once
#include "DataVector.h"
#include <set>

namespace algorithm
{
    class SubTableRow : public source::dataVectorT
    {
        friend class SubTable;
    public:
        SubTableRow() = default;
        SubTableRow(const source::dataVectorT & data);
        ~SubTableRow() = default;

        bool isClassified() const;
    private:
        bool classified;
    };

    
}
