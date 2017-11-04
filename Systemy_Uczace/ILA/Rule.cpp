#include "Rule.h"

namespace algorithm
{
    Rule::Rule(size_t attributeIndex,
               const source::dataV & attributeValue,
               const std::string & classValue) :
        Rule(attributesIndicesT(1, attributeIndex),
             attributesValuesT(1, attributeValue),
             classValue)

    {
        // Nothing to do
    }

    Rule::Rule(const attributesIndicesT & attributesIndices,
               const attributesValuesT & attributesValues,
               const std::string & classValue) :
        attributesIndices(attributesIndices),
        attributesValues(attributesValues),
        classValue(classValue)
    {
        // Nothing to do
    }

    Rule::Rule(const Rule & rule) :
        Rule(rule.attributesIndices, rule.attributesValues, rule.classValue)
    {
        // Nothing to do
    }

    bool Rule::classify(const source::dataVectorT & data, std::string & outClassValue) const
    {
        attributesValuesT dataRow;

        for (const auto& el : attributesIndices)
        {
            dataRow.push_back(data[el]);
        }

        auto retVal = attributesValues == dataRow;
        if (retVal)
        {
            outClassValue = classValue;
        }

        return retVal;
    }

    bool Rule::classify(const std::vector<Rule>& rules, const source::dataVectorT & data, std::string & outClassValue)
    {
        auto retVal = false;
        for (const auto& el : rules)
        {
            retVal = el.classify(data, outClassValue);
            if (retVal)
            {
                break;
            }
        }

        return retVal;
    }

    void Rule::concatenateRules(std::vector<Rule>& r1, const std::vector<Rule>& r2)
    {
        for (const auto& el : r2)
        {
            r1.emplace_back(el);
        }
    }

    void Rule::concatenateRules(std::vector<Rule>& r1, const Rule & r2)
    {
        r1.emplace_back(r2);
    }

}
