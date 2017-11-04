#pragma once
#include "DataVector.h"

namespace algorithm
{
    typedef std::vector<size_t> attributesIndicesT;
    typedef std::vector<source::dataV> attributesValuesT;

    class Rule
    {
    public:
        Rule(size_t attributeIndex,
             const source::dataV & attributeValue,
             const std::string & classValue);
        Rule(const attributesIndicesT & attributesIndices,
             const attributesValuesT & attributesValues,
             const std::string & classValue);
        Rule(const Rule & rule);
        ~Rule() = default;

        bool classify(const source::dataVectorT & data, std::string & outClassValue) const;
        static bool classify(const std::vector<Rule> & rules,
                             const source::dataVectorT & data,
                             std::string & outClassValue);

        static void concatenateRules(std::vector<Rule> & r1, const std::vector<Rule> & r2);
        static void concatenateRules(std::vector<Rule> & r1, const Rule & r2);

        Rule& operator=(const Rule& other);
    private:
        const attributesIndicesT attributesIndices;
        const attributesValuesT attributesValues;
        const std::string classValue;
    };

    typedef Rule ruleT;
    typedef std::vector<ruleT> rulesVectorT;
}
