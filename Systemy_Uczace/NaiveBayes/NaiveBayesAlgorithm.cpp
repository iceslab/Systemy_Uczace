#include "NaiveBayesAlgorithm.h"

namespace algorithm
{
    NaiveBayesAlgorithm::NaiveBayesAlgorithm(const source::dataDescriptionT & descriptions,
                                             const source::trainingDataT & trainingData) :
        abstracts::Algorithm(descriptions, trainingData)
    {
    }

    NaiveBayesAlgorithm::~NaiveBayesAlgorithm()
    {
    }

    void NaiveBayesAlgorithm::produceModel()
    {
        //P(C | X) = (P(X | C) * P(C)) / P(X)
    }

    classProbabilitiesT
        NaiveBayesAlgorithm::getClassProbability(const source::dataDescriptionT &descriptions,
                                                 const source::trainingDataT &trainingData)
    {
        // Vector of names for all classifiers
        const auto& classNames = descriptions.back();
        const auto classCount = std::get<2>(classNames).size();
        const auto allClassesCount = trainingData.size();

        // Class probability - return value
        classProbabilitiesT p_c(classCount);

        for (size_t i = 0; i < classCount; i++)
        {
            // Name of currently processed classifier
            const auto& processedClassName = std::get<std::string>(std::get<2>(classNames)[i]);
            size_t processedClassCount = 0;
            for (auto& dataRow : trainingData)
            {
                // Class name in training data
                const auto& classValue = std::get<std::string>(dataRow.back());
                if (classValue == processedClassName)
                {
                    ++processedClassCount;
                }
            }

            p_c[i] = static_cast<double>(processedClassCount) / static_cast<double>(allClassesCount);
        }

        return p_c;
    }

    attributesProbabilitiesT
        NaiveBayesAlgorithm::getAttributesProbability(const source::dataDescriptionT & descriptions,
                                                      const source::trainingDataT & trainingData)
    {
        const auto rowsCount = trainingData.size();
        const auto columnsCount = descriptions.size();
        const auto classDescription = descriptions.back();
        std::vector<source::trainingColumnT> columnData(columnsCount);

        for (size_t row = 0; row < rowsCount; row++)
        {
            for (size_t column = 0; column < columnsCount; column++)
            {
                columnData[column].emplace_back(trainingData[row][column]);
            }
        }

        const auto classData = columnData.back();
        attributesProbabilitiesT p_xc(columnsCount - 1);
        for (size_t i = 0; i < columnsCount - 1; i++)
        {
            p_xc[i] = getElementProbability(descriptions[i],
                                            columnData[i],
                                            classDescription,
                                            classData);
        }

        return p_xc;
    }

    elementProbabilitiesT
        NaiveBayesAlgorithm::getElementProbability(const source::dataDescriptionElementT & description,
                                                   const source::trainingColumnT & trainingData,
                                                   const source::dataDescriptionElementT &classDescription,
                                                   const source::trainingColumnT &classData)
    {
        const auto attributeType = std::get<0>(description);
        switch (attributeType)
        {
            case source::CATEGORY:
                return categoryProbability(description,
                                           trainingData,
                                           classDescription,
                                           classData);
                break;
            case source::INTEGER:
            case source::REAL:
                return numberProbability(description,
                                         trainingData,
                                         classDescription,
                                         classData);
                break;
            default:
                FATAL_ERROR();
                break;
        }
        return elementProbabilitiesT();
    }

    elementProbabilitiesT
        NaiveBayesAlgorithm::categoryProbability(const source::dataDescriptionElementT & description,
                                                 const source::trainingColumnT & trainingData,
                                                 const source::dataDescriptionElementT &classDescription,
                                                 const source::trainingColumnT &classData)
    {
        const auto attributesCount = std::get<2>(description).size();
        const auto classNames = std::get<2>(classDescription);
        const auto classCount = classNames.size();
        elementProbabilitiesT p_xc(attributesCount, classProbabilitiesT(classCount));

        const auto allVectorsCount = trainingData.size();

        // For every data in trainingData and ClassData
        for (size_t row = 0; row < allVectorsCount; row++)
        {
            const auto& elementValue = std::get<std::string>(trainingData[row].get());
            const auto desiredClassName = std::get<std::string>(classData[row].get());
            auto it = std::find(classNames.begin(), classNames.end(), source::descriptionV(desiredClassName));
            ASSERT(it != classNames.end());
            const auto classIndex = it - classNames.begin();

            // For every value of attribute
            for (size_t i = 0; i < attributesCount; i++)
            {
                const auto& processedElementName = std::get<std::string>(std::get<2>(description)[i]);

                // Count which attribute it is
                if (elementValue == processedElementName)
                {
                    p_xc[i][classIndex]++;
                    break;
                }
            }
        }

        // Calculate probability
        for (auto& elements : p_xc)
        {
            for (auto& probability : elements)
            {
                probability /= static_cast<double>(classCount);
            }
        }

        return p_xc;
    }

    elementProbabilitiesT
        NaiveBayesAlgorithm::numberProbability(const source::dataDescriptionElementT & description,
                                               const source::trainingColumnT & trainingData,
                                               const source::dataDescriptionElementT & classDescription,
                                               const source::trainingColumnT &classData)
    {
        FATAL_ERROR_VERBOSE("Not implemented yet");
        return elementProbabilitiesT();
    }

    classProbabilitiesT
        NaiveBayesAlgorithm::classProbability(const source::dataDescriptionElementT & description,
                                              const source::trainingColumnT & trainingData)
    {
        return classProbabilitiesT();
    }
}