#pragma once
#include "Algorithm.h"
#include "asserts.h"
#include "SubTableRow.h"
#include "Rule.h"

namespace algorithm
{
    typedef std::vector<source::dataV> dataCombinationT;
    typedef std::vector<dataCombinationT> valuesCombinationsT;
    typedef std::vector<attributesIndicesT> indicesCombinationsT;

    class ILAAlgorithm : public abstracts::Algorithm
    {
    public:
        ILAAlgorithm(const source::dataDescriptionT & description,
                     const source::trainingDataT & trainingData);
        ~ILAAlgorithm() = default;

        void createRules() const;
        void getRules() const;
    private:
        mutable bool modelBuilt;
        mutable SubTableRow::subTableVectorT subTables;
        mutable rulesVectorT rules;

        size_t allAttributesSize;

        // Creates sub tables basing on training data
        void createSubTables() const;
        rulesVectorT getUniqueRules() const;

        rulesVectorT getRulesForValues(const attributesIndicesT & indices,
                                       const dataCombinationT & combination,
                                       const size_t currentSubTableIndex) const;

        rulesVectorT getRulesForAttributes(size_t attributesCount) const;

        // Generates all valid combinations of data in given sub table and given attributes indices
        valuesCombinationsT
            getSubTableValueCombination(size_t subTableIndex,
                                        const attributesIndicesT & combinations) const;
        valuesCombinationsT
            getUniqueSubTableValueCombination(size_t currentSubTableIndex,
                                              const valuesCombinationsT & valuesCombinations,
                                              const attributesIndicesT & combinations) const;

        dataCombinationT
            getMaxUniqueSubTableValueCombination(size_t currentSubTableIndex,
                                                 const valuesCombinationsT & valuesCombinations,
                                                 const attributesIndicesT & combinations) const;

        // Generates all sets of given size within valid attributes indices
        indicesCombinationsT getIndicesCombinations(size_t attributesSize) const;

        // Generates next lexographic bit permutation
        size_t nextLexographicPermutation(size_t v) const;

        // Generates all bit permutations
        std::vector<size_t> allLexographicPermutations(size_t attributesSize) const;
    };
}
