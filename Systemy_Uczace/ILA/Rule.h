#pragma once
#include "DataVector.h"
#include <unordered_map>

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
        static bool classifySimple(const std::vector<Rule> & rules,
                                   const source::dataVectorT & data,
                                   std::string & outClassValue);
        static bool classifyVote(const std::vector<Rule> & rules,
                                 const source::dataVectorT & data,
                                 std::string & outClassValue);
        static void classifyOccurences(const source::dataMatrixT & allData,
                                       std::string & outClassValue);

        static void concatenateRules(std::vector<Rule> & r1, const std::vector<Rule> & r2);
        static void concatenateRules(std::vector<Rule> & r1, const Rule & r2);

        std::string toString() const;
    private:
        bool simpleClassification;

        const attributesIndicesT attributesIndices;
        const attributesValuesT attributesValues;
        const std::string classValue;
    };

    typedef Rule ruleT;
    typedef std::vector<ruleT> rulesVectorT;
}
