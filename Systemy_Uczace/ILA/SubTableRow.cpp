#include "SubTableRow.h"

namespace algorithm
{
    SubTableRow::SubTableRow(const source::dataVectorT & data) : source::dataVectorT(data)
    {
        isClassified = false;
    }

    source::dataColumnT
        SubTableRow::getUniqueValuesForAttribute(const subTableVectorT & subTables,
                                                 size_t subTableIndex,
                                                 size_t attributeIndex)
    {
        std::set<source::dataV> presentValues;

        // Fill presentValues set
        for (const auto& el : subTables[subTableIndex])
        {
            presentValues.emplace(el[attributeIndex]);
        }


        for (size_t i = 0; i < subTables.size(); i++)
        {
            // Skip sub table with which we compare
            if (i == subTableIndex)
                continue;

            // Consider all rows in other subTables...
            for (const auto& subRow : subTables[i])
            {
                // ...and find unique present values
                for (const auto& el : presentValues)
                {
                    if (el == subRow[attributeIndex])
                    {
                        presentValues.erase(el);
                        break;
                    }
                }
            }
        }

        return source::dataColumnT(presentValues.begin(), presentValues.end());
    }

    source::dataColumnRefT SubTableRow::getAttributeColumn(std::vector<SubTableRow>& str,
                                                           size_t attributeIndex)
    {
        source::dataColumnRefT retVal;
        retVal.reserve(str.size());

        for (auto& el : str)
        {
            retVal.emplace_back(el[attributeIndex]);
        }

        return retVal;
    }

    bool SubTableRow::allRowsClassified(const subTableT & subTable)
    {
        for (const auto el : subTable)
        {
            if (el.isClassified == false)
                return false;
        }

        return true;
    }
}
