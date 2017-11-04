#include "SubTableRow.h"

namespace algorithm
{
    SubTableRow::SubTableRow(const source::dataVectorT & data) : source::dataVectorT(data)
    {
        classified = false;
    }

    bool SubTableRow::isClassified() const
    {
        return classified;
    }
}
