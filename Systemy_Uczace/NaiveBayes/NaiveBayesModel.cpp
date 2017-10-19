#include "NaiveBayesModel.h"

namespace model
{
    NaiveBayesModel::NaiveBayesModel(const source::testDataT & testData,
                                     const algorithm::NaiveBayesAlgorithm & algorithm) :
        p_xc(p_xc), p_c(p_c)
    {
        // Nothing to do
    }

    NaiveBayesModel::NaiveBayesModel(const source::testDataT & testData,
                                     const algorithm::attributesProbabilitiesT & p_xc,
                                     const algorithm::classProbabilitiesT & p_c,
                                     const source::dataDescriptionT & descriptions) :
        testData(testData), p_xc(p_xc), p_c(p_c), descriptions(descriptions)
    {
        // Nothing to do
    }

    source::testDataT NaiveBayesModel::classify()
    {
        // P(C | X) = (P(X | C) * P(C)) / P(X)

        source::testDataT retVal;

        for (auto& row : testData)
        {
            retVal.emplace_back(classify(row));
        }

        return retVal;
    }

    source::dataVectorT NaiveBayesModel::classify(const source::dataVectorT & data)
    {
        // P(C | X) = (P(X | C) * P(C)) / P(X)
        // P(X|C_i) = product_of{j=1 n}(P(x_j | C_i))

        const auto attributesCount = p_xc.size();
        const auto classesCount = p_c.size();
        auto classAssignProbability = algorithm::classProbabilitiesT(classesCount, 1.0L);

        for (size_t attributeIndex = 0; attributeIndex < attributesCount; attributeIndex++)
        {
            auto type = std::get<0>(descriptions[attributeIndex]);
            auto elementIndex = getElementIndex(type,
                                                data[attributeIndex],
                                                descriptions[attributeIndex]);
            for (size_t classIndex = 0; classIndex < classesCount; classIndex++)
            {
                classAssignProbability[classIndex];
            }
        }

        return source::dataVectorT();
    }
    size_t NaiveBayesModel::getElementIndex(source::DataTypeE type,
                                            const source::dataV & data,
                                            const source::dataDescriptionElementT & description)
    {
        switch (type)
        {
            case source::CATEGORY:
                return getElementIndex<std::string>(data, description);
            case source::INTEGER_DISCRETE:
                return getElementIndex<std::pair<int, int>>(data, description);
            case source::REAL_DISCRETE:
                return getElementIndex<std::pair<double, double>>(data, description);
            case source::INTEGER:
            case source::REAL:
                DEBUG_PRINTLN("Function %s() does not support values INTEGER or REAL. "
                              "Returning max of size_t.",
                              __FUNCTION__);
                break;
            default:
                FATAL_ERROR_VERBOSE("Function %s() does not support value provided (%d)",
                                    __FUNCTION__,
                                    type);
                break;
        }
        return std::numeric_limits<size_t>::max();
    }

    template<typename T>
    size_t NaiveBayesModel::getElementIndex(const source::dataV & data,
                                            const source::dataDescriptionElementT & description)
    {
        auto desiredClassName = std::get<T>(data);
        auto classes = std::get<2>(description);
        auto it = std::find_if(
            classes.begin(),
            classes.end(),
            [desiredClassName](source::descriptionV el)->bool
        {
            return std::get<T>(el) == desiredClassName;
        });

        ASSERT(it != classes.end());
        const auto classIndex = it - classes.begin();
        return classIndex;
    }
}