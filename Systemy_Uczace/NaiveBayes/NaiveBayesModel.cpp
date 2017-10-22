#include "NaiveBayesModel.h"

namespace model
{
    NaiveBayesModel::NaiveBayesModel(const source::testDataT & testData,
                                     const algorithm::NaiveBayesAlgorithm & algorithm) :
        NaiveBayesModel(testData,
                        algorithm.getAttributesProbability(),
                        algorithm.getClassProbability(),
                        algorithm.getDescriptions(),
                        algorithm.getDistributions())
    {
        // Nothing to do
    }

    NaiveBayesModel::NaiveBayesModel(const source::testDataT & testData,
                                     const algorithm::attributesProbabilitiesT & p_xc,
                                     const algorithm::classProbabilitiesT & p_c,
                                     const source::dataDescriptionT & descriptions,
                                     const algorithm::distributionsT & distributions) :
        testData(testData),
        p_xc(p_xc),
        p_c(p_c),
        descriptions(descriptions),
        distributions(distributions)
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
        auto classAssignProbability = algorithm::classProbabilitiesT(classesCount);

        for (size_t classIndex = 0; classIndex < classesCount; classIndex++)
        {
            // P(C)
            classAssignProbability[classIndex] = p_c[classIndex];
        }

        for (size_t attributeIndex = 0; attributeIndex < attributesCount; attributeIndex++)
        {
            auto type = std::get<0>(descriptions[attributeIndex]);
            auto elementIndex = source::DataSource::getElementIndex(
                type,
                data[attributeIndex],
                descriptions[attributeIndex]);


            for (size_t classIndex = 0; classIndex < classesCount; classIndex++)
            {
                // P(x_j | C_i)
                const auto xInClassProbability = p_xc[attributeIndex][elementIndex][classIndex];
                // product_of{ j = 1 n }(P(x_j | C_i))
                classAssignProbability[classIndex] *= xInClassProbability;
            }
        }

        const auto it = std::max_element(classAssignProbability.begin(),
                                         classAssignProbability.end());
        ASSERT(it != classAssignProbability.end());
        const size_t index = it - classAssignProbability.begin();
        const auto classes = std::get<2>(descriptions.back());
        const auto className = std::get<std::string>(classes[index]);

        auto retVal = source::dataVectorT(data);
        retVal.back() = className;
        return retVal;
    }

    double NaiveBayesModel::getAttributeProbability(size_t attributeIndex,
                                                    size_t elementIndex,
                                                    size_t classIndex,
                                                    const source::dataVectorT & data)
    {

        const auto attributeType = std::get<0>(descriptions.back());
        double x = 0.0;
        switch (attributeType)
        {
            case source::CATEGORY:
            case source::INTEGER_DISCRETE:
            case source::REAL_DISCRETE:
                return p_xc[attributeIndex][elementIndex][classIndex];;
            case source::INTEGER:
                x = std::get<int>(data[elementIndex]);
                return distributions[attributeIndex][classIndex].getProbabilityDenisty(x);
            case source::REAL:
                x = std::get<double>(data[elementIndex]);
                return distributions[attributeIndex][classIndex].getProbabilityDenisty(x);
            default:
                FATAL_ERROR();
                break;
        }
        return 0.0;
    }

}