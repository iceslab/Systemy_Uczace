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

    bool Rule::classifySimple(const std::vector<Rule>& rules,
                              const source::dataVectorT & data,
                              std::string & outClassValue)
    {
        auto classSet = false;
        for (const auto& el : rules)
        {
            classSet = el.classify(data, outClassValue);
            if (classSet)
            {
                break;
            }
        }

        return classSet;
    }

    bool Rule::classifyVote(const std::vector<Rule>& rules, const source::dataVectorT & data, std::string & outClassValue)
    {
        std::unordered_map<std::string, size_t> classNameOccurences;
        for (const auto& el : rules)
        {
            if (el.classify(data, outClassValue))
            {
                const auto it = classNameOccurences.find(outClassValue);

                if (it != classNameOccurences.end())
                {
                    it->second++;
                }
                else
                {
                    classNameOccurences.insert(std::make_pair(outClassValue, 0));
                }
            }
        }

        auto maxElement = std::make_pair(std::string(), 0U);
        for (const auto& el : classNameOccurences)
        {
            if (el.second > maxElement.second)
            {
                maxElement = el;
            }
        }

        if (!maxElement.first.empty())
        {
            outClassValue = maxElement.first;
            return true;
        }

        return false;
    }

    void Rule::classifyOccurences(const source::dataMatrixT & allData,
                                  std::string & outClassValue)
    {
        std::unordered_map<std::string, size_t> classNameOccurences;
        const auto classNames = source::DataVector::getClassColumnAsString(allData);

        // Fill map
        for (const auto& el : classNames)
        {
            const auto it = classNameOccurences.find(el);
            if (it != classNameOccurences.end())
            {
                it->second++;
            }
            else
            {
                classNameOccurences.insert(std::make_pair(outClassValue, 0));
            }
        }

        auto maxElement = std::make_pair(classNames.front(), 0U);
        for (const auto& el : classNameOccurences)
        {
            if (el.second > maxElement.second)
            {
                maxElement = el;
            }
        }

        outClassValue = maxElement.first;
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

    std::string Rule::toString() const
    {
        std::stringstream ss;
        ss << "{";
        for (size_t i = 0; i < attributesIndices.size(); i++)
        {
            ss << attributesIndices[i];
            if (i < attributesIndices.size() - 1)
            {
                ss << ", ";
            }
        }
        ss << "} {";
        for (size_t i = 0; i < attributesValues.size(); i++)
        {
            ss << source::DataVector::toString(attributesValues[i]);
            if (i < attributesValues.size() - 1)
            {
                ss << ", ";
            }
        }
        ss << "} " << classValue;

        return ss.str();
    }

}
