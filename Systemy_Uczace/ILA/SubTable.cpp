#include "SubTable.h"

namespace algorithm
{
    source::dataColumnT
        SubTable::getUniqueValuesForAttribute(const std::vector<SubTable> & subTables,
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

    source::dataColumnRefT SubTable::getAttributeColumn(size_t attributeIndex)
    {
        source::dataColumnRefT retVal;
        retVal.reserve(size());

        for (auto& el : *this)
        {
            retVal.emplace_back(el[attributeIndex]);
        }

        return retVal;
    }

    bool SubTable::allRowsClassified() const
    {
        return classifiedRows == size();
    }

    void SubTable::printClassifiedStatus() const
    {
        for (size_t i = 0; i < size(); i++)
        {
            fprintf(stderr, "%4d: %d\t", i, (*this)[i].isClassified());
        }
    }

    void SubTable::printValues() const
    {
        for (size_t i = 0; i < size(); i++)
        {
            const auto& row = (*this)[i];
            fprintf(stderr, "%4zu: %s\n", i, row.toString().c_str());
        }
    }

    void SubTable::printNotClassifiedValues() const
    {
        for (size_t i = 0; i < size(); i++)
        {
            const auto& row = (*this)[i];
            if (!row.isClassified())
            {
                fprintf(stderr, "%4zu: %s\n", i, row.toString().c_str());
            }
        }
    }

    void SubTable::setClassified(size_t index)
    {
        auto& subRowClassified = (*this)[index].classified;
        if (subRowClassified == false)
        {
            subRowClassified = true;
            classifiedRows++;
        }

    }

    size_t SubTable::getClassifiedCount() const
    {
        return classifiedRows;
    }

    size_t SubTable::getNotClassifiedCount() const
    {
        return size() - classifiedRows;
    }
}
