#pragma once
#include "DataVector.h"

namespace algorithm
{
    typedef std::vector<size_t> attributesIndicesT;
    typedef std::vector<source::dataV> attributesValuesT;

    class Rule
    {
    public:
        Rule(size_t attributeIndex, source::dataV attributeValue, std::string classValue);
        Rule(attributesIndicesT attributesIndices,
             attributesValuesT attributesValues,
             std::string classValue);
        ~Rule() = default;

        static void concatenateRules(std::vector<Rule> & r1, const std::vector<Rule> & r2);
        static void concatenateRules(std::vector<Rule> & r1, const Rule & r2);
    private:
        attributesIndicesT attributesIndices;
        attributesValuesT attributesValues;
        std::string classValue;
    };

    typedef Rule ruleT;
    typedef std::vector<ruleT> rulesVectorT;
}
