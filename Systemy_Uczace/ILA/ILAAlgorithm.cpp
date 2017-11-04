#include "ILAAlgorithm.h"

namespace algorithm
{
    ILAAlgorithm::ILAAlgorithm(const source::dataDescriptionT & description,
                               const source::trainingDataT & trainingData) :
        abstracts::Algorithm(descriptions, trainingData),
        allAttributesSize(description.size() - 1)
    {
        // Nothing to do
    }

    void ILAAlgorithm::createRules() const
    {
        createSubTables();

        rules = getUniqueRules();


        // Indicate that model is built
        modelBuilt = true;
    }

    void ILAAlgorithm::getRules() const
    {
        if (!modelBuilt)
        {
            createRules();
        }

        return;
    }

    void ILAAlgorithm::createSubTables() const
    {
        const auto subTablesCount = descriptions.getClassNamesCount();

        subTables.resize(subTablesCount);
        for (const auto& el : trainingData)
        {
            const auto subTableIndex = classNameToIndex(el.getClassName());
            subTables[subTableIndex].emplace_back(el);
        }
    }

    rulesVectorT ILAAlgorithm::getUniqueRules() const
    {
        rulesVectorT retVal;

        // For each sub table...
        for (size_t subTableIndex = 0; subTableIndex < subTables.size(); subTableIndex++)
        {
            auto maxOccurencesPair = std::make_pair(source::dataV(),
                                                    std::numeric_limits<size_t>::min());
            auto maxAttributeIndex = std::numeric_limits<size_t>::max();
            // ...and for each attriute...
            for (size_t attributeIndex = 0; attributeIndex < allAttributesSize; attributeIndex++)
            {
                auto uniqueValues = SubTableRow::getUniqueValuesForAttribute(subTables,
                                                                             subTableIndex,
                                                                             attributeIndex);
                for (const auto& value : uniqueValues)
                {
                    // Column reference
                    auto column = SubTableRow::getAttributeColumn(subTables[subTableIndex],
                                                                  attributeIndex);
                    // Count value appereances
                    const size_t count = std::count_if(column.begin(),
                                                       column.end(),
                                                       [value](auto el)->bool
                    {
                        return el.get() == source::dataV(value);
                    });

                    // If count is greater replace values
                    if (count > maxOccurencesPair.second)
                    {
                        maxOccurencesPair = std::make_pair(value, count);
                    }
                }
            }

            // Mark classified rows
            if (maxAttributeIndex != std::numeric_limits<decltype(maxAttributeIndex)>::max())
            {
                for (auto& subRow : subTables[subTableIndex])
                {
                    if (subRow[maxAttributeIndex] == maxOccurencesPair.first)
                    {
                        subRow.isClassified = true;
                    }
                }
            }

            // Add rule
            retVal.emplace_back(maxAttributeIndex,
                                maxOccurencesPair.first,
                                subTables[subTableIndex].front().getClassName());
        }

        return retVal;
    }

    rulesVectorT ILAAlgorithm::getRulesForValues(const attributesIndicesT & indices,
                                                 const dataCombinationT & combination,
                                                 const size_t currentSubTableIndex) const
    {
        rulesVectorT retVal;
        std::string className;
        // For each sub table row...
        for (auto& subRow : subTables[currentSubTableIndex])
        {
            // ...create data combination...
            dataCombinationT rowData;
            for (const auto& index : indices)
            {
                rowData.push_back(subRow[index]);
            }

            subRow.isClassified = rowData == combination;

            if (subRow.isClassified)
            {
                className = subRow.getClassName();
            }
        }

        // Add rule
        retVal.emplace_back(indices,
                            combination,
                            className);

        return retVal;
    }

    rulesVectorT ILAAlgorithm::getRulesForAttributes(size_t attributesCount) const
    {
        auto indicesCombinations = getIndicesCombinations(attributesCount);

        rulesVectorT retVal;
        for (size_t subTableIndex = 0; subTableIndex < subTables.size(); subTableIndex++)
        {
            for (const auto& indicesCombination : indicesCombinations)
            {
                const auto valueCombination = getSubTableValueCombination(subTableIndex,
                                                                          indicesCombination);
                const auto combinations = getUniqueSubTableValueCombination(subTableIndex,
                                                                            valueCombination,
                                                                            indicesCombination);
                const auto combination = getMaxUniqueSubTableValueCombination(subTableIndex,
                                                                              combinations,
                                                                              indicesCombination);
                const auto rule = getRulesForValues(indicesCombination,
                                                     combination,
                                                     subTableIndex);
                Rule::concatenateRules(retVal, rule);
            }
        }

        return retVal;
    }

    valuesCombinationsT
        ILAAlgorithm::getSubTableValueCombination(
            size_t subTableIndex,
            const attributesIndicesT & combinations) const
    {
        const auto& subTable = subTables[subTableIndex];
        std::vector<std::set<source::dataV>> uniqueValues(combinations.size());

        for (const auto& subRow : subTable)
        {
            for (size_t i = 0; !subRow.isClassified && i < combinations.size(); i++)
            {
                uniqueValues[i].insert(subRow[combinations[i]]);
            }
        }

        // Initialize set iterators
        std::vector<std::set<source::dataV>::iterator> setIterators;
        for (auto& set : uniqueValues)
        {
            setIterators.push_back(set.begin());
        }

        valuesCombinationsT retVal;
        // Generate combinations
        auto generatedAll = false;
        do
        {
            dataCombinationT combination(combinations.size());
            for (size_t i = 0; i < uniqueValues.size(); i++)
            {
                const auto index = uniqueValues.size() - i - 1;
                combination[index] = *(setIterators[index]);

                // Least significant set
                if (i == 0)
                {
                    setIterators[index]++;
                }
                // Not the least significant set
                else
                {
                    // If is on end() iterator, reset to begin and increment current
                    if (setIterators[index + 1] == uniqueValues[index + 1].end())
                    {
                        setIterators[index + 1] = uniqueValues[index + 1].begin();
                        setIterators[index]++;
                    }
                }

                // Most significant set
                if (index == 0 &&
                    setIterators[index] == uniqueValues[index].end())
                {
                    // Finish generating
                    generatedAll = true;
                }
            }

            // Insert generated combination
            retVal.emplace_back(combination);
        } while (!generatedAll);

        return retVal;
    }

    valuesCombinationsT
        ILAAlgorithm::getUniqueSubTableValueCombination(
            size_t currentSubTableIndex,
            const valuesCombinationsT & valuesCombinations,
            const attributesIndicesT & combinations) const
    {
        auto retVal(valuesCombinations);
        // For each sub table...
        for (size_t subTableIndex = 0; subTableIndex < subTables.size(); subTableIndex++)
        {
            if (currentSubTableIndex == subTableIndex)
            {
                continue;
            }

            // ...and for each row in sub table...
            for (const auto& subRow : subTables[subTableIndex])
            {
                // ...create data combination...
                dataCombinationT rowData;
                for (const auto& index : combinations)
                {
                    rowData.push_back(subRow[index]);
                }

                // ... and remove it from all combinations if exist
                std::remove_if(retVal.begin(), retVal.end(),
                               [rowData](dataCombinationT data)->bool
                {
                    return rowData == data;
                });
            }
        }

        return retVal;
    }

    dataCombinationT
        ILAAlgorithm::getMaxUniqueSubTableValueCombination(
            size_t currentSubTableIndex,
            const valuesCombinationsT & valuesCombinations,
            const attributesIndicesT & combinations) const
    {
        ASSERT(valuesCombinations.size() == combinations.size());
        std::vector<size_t> occurences(valuesCombinations.size(), 0);

        // For each row in sub table...
        //for (size_t subRow = 0; subRow < subTables[currentSubTableIndex].size(); subRow++)
        for (const auto& subRow : subTables[currentSubTableIndex])
        {
            // ...create data combination...
            dataCombinationT rowData;
            for (const auto& index : combinations)
            {
                rowData.push_back(subRow[index]);
            }

            // ...and for each value to check...
            for (size_t valuesIndex = 0; valuesIndex < valuesCombinations.size(); valuesIndex++)
            {
                // ...increment occurences
                if (valuesCombinations[valuesIndex] == rowData)
                {
                    occurences[valuesIndex]++;
                }
            }
        }

        const auto it = std::max_element(occurences.begin(), occurences.end());
        ASSERT(it != occurences.end());

        const auto index = it - occurences.begin();
        return valuesCombinations[index];
    }

    indicesCombinationsT
        ILAAlgorithm::getIndicesCombinations(size_t attributesSize) const
    {
        const auto permutations = allLexographicPermutations(attributesSize);
        std::vector<attributesIndicesT> retVal(permutations.size());

        for (size_t permutationIdx = 0; permutationIdx < permutations.size(); permutationIdx++)
        {
            const auto& el = permutations[permutationIdx];
            for (size_t mask = 1, i = 0, idx = 0;
                 i < attributesSize && idx < allAttributesSize;
                 mask <<= 1, idx++)
            {
                if (el & mask)
                {
                    retVal[permutationIdx].push_back(idx);
                    i++;
                }
            }
        }

        return retVal;
    }

    size_t ILAAlgorithm::nextLexographicPermutation(size_t v) const
    {
        size_t t = v | (v - 1); // t gets v's least significant 0 bits set to 1
                                // Next set to 1 the most significant bit to change, 
                                // set to 0 the least significant ones, and add the necessary 1 bits.
        DWORD bsfRet;
        _BitScanForward(&bsfRet, v);
        size_t w = (t + 1) | (((~t & (~t + 1)) - 1) >> (bsfRet + 1));
        return w;
    }

    std::vector<size_t> ILAAlgorithm::allLexographicPermutations(size_t attributesSize) const
    {
        const auto initialMask = (static_cast<size_t>(1) << attributesSize) - 1;
        const auto finishMask = initialMask << (allAttributesSize - attributesSize);
        auto combinationsMask = initialMask;

        std::vector<size_t> retVal;
        do
        {
            retVal.push_back(combinationsMask);
            combinationsMask = nextLexographicPermutation(combinationsMask);
        } while (combinationsMask < finishMask);
        std::reverse(retVal.begin(), retVal.end());
        return retVal;
    }
}
