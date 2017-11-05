#include "ILAAlgorithm.h"
#define ALTERNATE_IMPLEMENTATION

namespace algorithm
{
    ILAAlgorithm::ILAAlgorithm(const source::dataDescriptionT & description,
                               const source::trainingDataT & trainingData) :
        abstracts::Algorithm(description, trainingData),
        allAttributesSize(description.size() - 1)
    {
        // Nothing to do
    }

    void ILAAlgorithm::createRules() const
    {
        createSubTables();

        for (size_t i = 1; !allClassified() && i <= allAttributesSize; i++)
        {
            DEBUG_CALL(
                for (size_t j = 0; j < subTables.size(); j++)
                {
                    fprintf(stderr,
                            "\n\nSub table no. %3d, not classified: %zu\n",
                            j,
                            subTables[j].getNotClassifiedCount());
                    subTables[j].printNotClassifiedValues();
                }
            );

            Rule::concatenateRules(rules, getRulesForAttributes(i));

            DEBUG_CALL(
                fprintf(stderr, "\nRules: \n");
            //for(const auto& el : rules)
            for (size_t j = 0; j < rules.size(); j++)
            {
                fprintf(stderr, "%3d: %s\n", j, rules[j].toString().c_str());
            }
            );
        }

        DEBUG_CALL(
            if (!allClassified())
            {
                for (size_t j = 0; j < subTables.size(); j++)
                {
                    fprintf(stderr,
                            "\n\nSub table no. %3d, not classified: %zu\n",
                            j,
                            subTables[j].getNotClassifiedCount());
                    subTables[j].printNotClassifiedValues();
                }

                fprintf(stderr, "\nRules: \n");
                //for(const auto& el : rules)
                for (size_t j = 0; j < rules.size(); j++)
                {
                    fprintf(stderr, "%3d: %s\n", j, rules[j].toString().c_str());
                }
            }
        );

        // Indicate that model is built
        modelBuilt = true;
    }

    const rulesVectorT & ILAAlgorithm::getRules() const
    {
        if (!modelBuilt)
        {
            createRules();
        }

        return rules;
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

    rulesVectorT ILAAlgorithm::getRulesForValues(const attributesIndicesT & indices,
                                                 const dataCombinationT & combination,
                                                 const size_t currentSubTableIndex) const
    {
        // If there is no combination to check, return empty vector
        if (combination.empty())
        {
            return rulesVectorT();
        }

        rulesVectorT retVal;
        std::string className;
        // For each sub table row...
        for (size_t subRowIndex = 0; subRowIndex < subTables[currentSubTableIndex].size(); subRowIndex++)
        {
            auto& subRow = subTables[currentSubTableIndex][subRowIndex];
            // Skip already classified rows
            if (subRow.isClassified())
            {
                continue;
            }

            // ...create data combination...
            dataCombinationT rowData;
            for (const auto& index : indices)
            {
                rowData.push_back(subRow[index]);
            }

            if (rowData == combination)
            {
                subTables[currentSubTableIndex].setClassified(subRowIndex);
                className = subRow.getClassName();
            }
        }

        ASSERT(!className.empty());
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
            // Skip table if already classified
            if (subTables[subTableIndex].allRowsClassified())
            {
                DEBUG_PRINTLN("All rows in subTable[%zu] classified. Skipping...", subTableIndex);
                continue;
            }

            for (const auto& indicesCombination : indicesCombinations)
            {

                PRINT_SEPARATOR();
                DEBUG_PRINTLN("subTableIndex: %zu", subTableIndex);
                DEBUG_PRINTLN("indicesCombination:\n%s",
                              toStringAttributesIndices(indicesCombination).c_str());
                const auto valueCombination =
                    getSubTableValueCombination(subTableIndex,
                                                indicesCombination);
                DEBUG_PRINTLN("\ngetSubTableValueCombination:\n%s",
                              toStringValueCombinations(valueCombination).c_str());
                const auto combinations =
                    getUniqueSubTableValueCombination(subTableIndex,
                                                      valueCombination,
                                                      indicesCombination);
                DEBUG_PRINTLN("\ngetUniqueSubTableValueCombination:\n%s",
                              toStringValueCombinations(combinations).c_str());
                const auto combination =
                    getMaxUniqueSubTableValueCombination(subTableIndex,
                                                         combinations,
                                                         indicesCombination);
                DEBUG_PRINTLN("\ngetMaxUniqueSubTableValueCombination:\n%s",
                              toStringDataCombination(combination).c_str());
                const auto rule =
                    getRulesForValues(indicesCombination,
                                      combination,
                                      subTableIndex);
                Rule::concatenateRules(retVal, rule);

                // Break looop if already classified
                if (subTables[subTableIndex].allRowsClassified())
                {
                    DEBUG_PRINTLN("All rows in subTable[%zu] classified. Breaking loop.", subTableIndex);
                    PRINT_SEPARATOR();
                    break;
                }
                PRINT_SEPARATOR();
            }
        }

        return retVal;
    }

#ifdef ALTERNATE_IMPLEMENTATION
    valuesCombinationsT
        ILAAlgorithm::getSubTableValueCombination(
            size_t subTableIndex,
            const attributesIndicesT & combinations) const
    {
        const auto& subTable = subTables[subTableIndex];
        std::set<dataCombinationT> retValSet;

        for (size_t subRowIndex = 0; subRowIndex < subTable.size(); subRowIndex++)
        {
            dataCombinationT combination;
            combination.reserve(combinations.size());
            const auto& subRow = subTable[subRowIndex];
            for (size_t i = 0; !subRow.isClassified() && i < combinations.size(); i++)
            {
                combination.push_back(subRow[combinations[i]]);
            }

            if (!combination.empty())
            {
                retValSet.insert(combination);
            }
        }
        return valuesCombinationsT(retValSet.begin(), retValSet.end());
    }

#else

    valuesCombinationsT
        ILAAlgorithm::getSubTableValueCombination(
            size_t subTableIndex,
            const attributesIndicesT & combinations) const
    {
        const auto& subTable = subTables[subTableIndex];
        std::vector<std::set<source::dataV>> uniqueValues(combinations.size());

        for (size_t subRowIndex = 0; subRowIndex < subTable.size(); subRowIndex++)
        {
            const auto& subRow = subTable[subRowIndex];
            for (size_t i = 0; !subRow.isClassified() && i < combinations.size(); i++)
            {
                uniqueValues[i].insert(subRow[combinations[i]]);
            }
        }

        // Initialize set iterators
        std::vector<std::set<source::dataV>::iterator> setIterators;
        for (auto& set : uniqueValues)
        {
            // It means that there are no valid combinations for given indices
            if (set.empty())
            {
                return valuesCombinationsT();
            }

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
#endif // ALTERNATE_IMPLEMENTATION

    valuesCombinationsT
        ILAAlgorithm::getUniqueSubTableValueCombination(
            size_t currentSubTableIndex,
            const valuesCombinationsT & valuesCombinations,
            const attributesIndicesT & combinations) const
    {
        // There is not use to go further
        if (valuesCombinations.empty())
        {
            return valuesCombinationsT();
        }

        auto valuesCopy(valuesCombinations);
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
                std::remove_if(valuesCopy.begin(), valuesCopy.end(),
                               [rowData](dataCombinationT data)->bool
                {
                    return rowData == data;
                });
            }
        }
        valuesCombinationsT retVal;
        for (const auto& el : valuesCopy)
        {
            if (!el.empty())
            {
                retVal.push_back(el);
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
        if (!valuesCombinations.empty())
        {
            ASSERT(valuesCombinations.front().size() == combinations.size());
        }
        else
        {
            return dataCombinationT();
        }

        std::vector<size_t> occurences(valuesCombinations.size(), 0);

        // For each row in sub table...
        //for (size_t subRow = 0; subRow < subTables[currentSubTableIndex].size(); subRow++)
        for (const auto& subRow : subTables[currentSubTableIndex])
        {
            // Skip classified rows in counting
            if (subRow.isClassified())
            {
                continue;
            }

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
        DEBUG_PRINT("max occurences: %zu", *it);

        if (*it > 0)
        {
            const auto index = it - occurences.begin();
            return valuesCombinations[index];
        }
        else
        {
            return dataCombinationT();
        }
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
        signed_size_t t = v | (v - 1); // t gets v's least significant 0 bits set to 1
                                // Next set to 1 the most significant bit to change, 
                                // set to 0 the least significant ones, and add the necessary 1 bits.
        DWORD bsfRet;
        _BitScanForward(&bsfRet, v);
        auto w = (t + 1) | (((~t & -~t) - 1) >> (bsfRet + 1));
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
        } while (combinationsMask <= finishMask);
        //std::reverse(retVal.begin(), retVal.end());
        return retVal;
    }

    bool ILAAlgorithm::allClassified() const
    {
        for (const auto el : subTables)
        {
            if (el.allRowsClassified() == false)
            {
                return false;
            }
        }

        return true;
    }

    std::string ILAAlgorithm::toStringDataCombination(const dataCombinationT & dc)
    {
        std::stringstream ss;
        ss << "{";
        for (size_t i = 0; i < dc.size(); i++)
        {
            ss << source::DataVector::toString(dc[i]);
            if (i < dc.size() - 1)
            {
                ss << ", ";
            }
        }
        ss << "} ";
        return ss.str();
    }

    std::string ILAAlgorithm::toStringValueCombinations(const valuesCombinationsT & vc)
    {
        std::stringstream ss;
        ss << "<";
        for (size_t i = 0; i < vc.size(); i++)
        {
            ss << toStringDataCombination(vc[i]);
            if (i < vc.size() - 1)
            {
                ss << "\n";
            }
        }
        ss << "> ";
        return ss.str();
    }

    std::string ILAAlgorithm::toStringAttributesIndices(const attributesIndicesT & ai)
    {
        std::stringstream ss;
        ss << "<";
        for (size_t i = 0; i < ai.size(); i++)
        {
            ss << ai[i];
            if (i < ai.size() - 1)
            {
                ss << ", ";
            }
        }
        ss << "> ";
        return ss.str();
    }
}
