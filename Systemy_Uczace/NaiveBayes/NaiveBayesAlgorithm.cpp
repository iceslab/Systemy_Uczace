#include "NaiveBayesAlgorithm.h"

namespace algorithm
{
    NaiveBayesAlgorithm::NaiveBayesAlgorithm(const source::dataDescriptionT & descriptions,
                                             const source::trainingDataT & trainingData) :
        abstracts::Algorithm(descriptions, trainingData)
    {
        // Nothing to do
    }

    classProbabilitiesT NaiveBayesAlgorithm::getClassProbability() const
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

            p_c[i] = static_cast<double>(processedClassCount) /
                static_cast<double>(allClassesCount);
        }

        return p_c;
    }

    attributesProbabilitiesT NaiveBayesAlgorithm::getAttributesProbability() const
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
        NaiveBayesAlgorithm::getElementProbability(
            const source::dataDescriptionElementT & description,
            const source::trainingColumnT & trainingData,
            const source::dataDescriptionElementT &classDescription,
            const source::trainingColumnT &classData) const
    {
        const auto attributeType = std::get<0>(description);
        switch (attributeType)
        {
            case source::CATEGORY:
                return categoryProbability<std::string>(
                    description,
                    trainingData,
                    classDescription,
                    classData);
            case source::INTEGER_DISCRETE:
                return categoryProbability<std::pair<int, int>>(
                    description,
                    trainingData,
                    classDescription,
                    classData);
            case source::REAL_DISCRETE:
                return categoryProbability<std::pair<double, double>>(
                    description,
                    trainingData,
                    classDescription,
                    classData);
            case source::INTEGER:
            case source::REAL:
                DEBUG_PRINTLN("Normal distribution not implemented yet. "
                              "Use discretized values. "
                              "Returning empty array");
                break;
            default:
                FATAL_ERROR();
                break;
        }
        return elementProbabilitiesT();
    }

    template <typename T>
    elementProbabilitiesT
        NaiveBayesAlgorithm::categoryProbability(const source::dataDescriptionElementT & description,
                                                 const source::trainingColumnT & trainingData,
                                                 const source::dataDescriptionElementT &classDescription,
                                                 const source::trainingColumnT &classData) const
    {
        static_assert(std::is_base_of<std::string, T>::value ||
                      std::is_base_of<std::pair<int, int>, T>::value ||
                      std::is_base_of<std::pair<double, double>, T>::value,
                      "");

        const auto attributesCount = std::get<2>(description).size();
        const auto classNames = std::get<2>(classDescription);
        const auto allClassesCount = classNames.size();
        std::unordered_map<std::string, size_t> classInstancesCount;

        for (size_t i = 0; i < allClassesCount; i++)
        {
            classInstancesCount.emplace(std::get<std::string>(classNames[i]), 0);
        }

        // 1.0L added according to Laplace estimator
        elementProbabilitiesT p_xc(attributesCount, classProbabilitiesT(allClassesCount, 1.0L));

        const auto allVectorsCount = trainingData.size();

        // For every data in trainingData and ClassData
        for (size_t row = 0; row < allVectorsCount; row++)
        {
            const auto& elementValue = std::get<T>(trainingData[row].get());
            const auto desiredClassName = std::get<std::string>(classData[row].get());
            auto it = std::find_if(
                classNames.begin(),
                classNames.end(),
                [desiredClassName](source::descriptionV el)->bool
            {
                return std::get<std::string>(el) == desiredClassName;
            });

            ASSERT(it != classNames.end());
            const auto classIndex = it - classNames.begin();

            // Count specific class instances
            auto mapIt = classInstancesCount.find(std::get<std::string>(*it));
            ASSERT(mapIt != classInstancesCount.end());
            mapIt->second++;

            // For every value of attribute
            for (size_t i = 0; i < attributesCount; i++)
            {
                const auto& processedElementName = std::get<T>(std::get<2>(description)[i]);

                // Count which attribute it is
                if (elementValue == processedElementName)
                {
                    p_xc[i][classIndex]++;
                    break;
                }
            }
        }

        // Calculate probability
        for (size_t i = 0; i < p_xc.size(); i++)
        {
            auto& elements = p_xc[i];
            for (auto& probability : elements)
            {
                auto mapIt = classInstancesCount.find(std::get<std::string>(classNames[i]));
                ASSERT(mapIt != classInstancesCount.end());
                probability /= static_cast<double>(mapIt->second);
            }
        }

        return p_xc;
    }
}