#include "Rule.h"

namespace algorithm
{
    Rule::Rule(size_t attributeIndex, source::dataV attributeValue, std::string classValue) :
        Rule(attributesIndicesT(1, attributeIndex),
             attributesValuesT(1, attributeValue),
             classValue)

    {
        // Nothing to do
    }

    Rule::Rule(attributesIndicesT attributesIndices,
               attributesValuesT attributesValues,
               std::string classValue) :
        attributesIndices(attributesIndices),
        attributesValues(attributesValues),
        classValue(classValue)
    {
        // Nothing to do
    }

    void Rule::concatenateRules(std::vector<Rule>& r1, const std::vector<Rule>& r2)
    {
        r1.insert(r1.end(), r2.begin(), r2.end());
    }

    void Rule::concatenateRules(std::vector<Rule>& r1, const Rule & r2)
    {
        r1.push_back(r2);
    }
}
