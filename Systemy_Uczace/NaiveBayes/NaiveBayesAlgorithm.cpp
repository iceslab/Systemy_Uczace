#include "NaiveBayesAlgorithm.h"

namespace algorithm
{
    NaiveBayesAlgorithm::NaiveBayesAlgorithm(const source::dataDescriptionT & descriptions,
                                             const source::trainingDataT & trainingData) :
        abstracts::Algorithm(descriptions, trainingData), modelBuilt(false), dataWeights(trainingData.size(), 1.0)
    {
        // Nothing to do
    }

    const classProbabilitiesT& NaiveBayesAlgorithm::getClassProbability() const
    {
        if (!modelBuilt)
        {
            calculateModel();
        }

        return p_c;
    }

    const attributesProbabilitiesT & NaiveBayesAlgorithm::getAttributesProbability() const
    {
        if (!modelBuilt)
        {
            calculateModel();
        }

        return p_xc;
    }

    const distributionsT & NaiveBayesAlgorithm::getDistributions() const
    {
        if (!modelBuilt)
        {
            calculateModel();
        }

        return distributions;
    }

    void NaiveBayesAlgorithm::setDataWeights(const weightsVectorT _dataWeights)
    {
        const auto copySize = std::min(dataWeights.size(), _dataWeights.size());
        std::copy(_dataWeights.begin(), _dataWeights.begin() + copySize, dataWeights.begin());
    }

    void NaiveBayesAlgorithm::calculateModel() const
    {
        calculateClassProbability();
        calculateAttributesProbability();
        modelBuilt = true;
    }

    void NaiveBayesAlgorithm::calculateClassProbability() const
    {
        // Vector of names for all classifiers
        const auto& classNames = descriptions.back();
        const auto classCount = std::get<2>(classNames).size();
        double allClassesCount = 0.0;
        for (const auto& el : dataWeights)
        {
            allClassesCount += el;
        }

        // Class probability - return value
        p_c.resize(classCount);

        for (size_t i = 0; i < classCount; i++)
        {
            // Name of currently processed classifier
            const auto& processedClassName = std::get<std::string>(std::get<2>(classNames)[i]);
            double processedClassCount = 0.0;
            for (size_t idx = 0; idx < trainingData.size(); idx++)
            {
                const auto& dataRow = trainingData[idx];
                const auto& weight = dataWeights[idx];
                // Class name in training data
                const auto& classValue = std::get<std::string>(dataRow.back());
                if (classValue == processedClassName)
                {
                    processedClassCount += weight;
                }
            }

            p_c[i] = processedClassCount / allClassesCount;
        }
    }

    void NaiveBayesAlgorithm::calculateAttributesProbability() const
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
        p_xc.resize(columnsCount - 1);
        distributions.resize(columnsCount - 1);
        for (size_t i = 0; i < columnsCount - 1; i++)
        {
            p_xc[i] = getDiscreteElementProbability(descriptions[i],
                                                    columnData[i],
                                                    classDescription,
                                                    classData);
            distributions[i] = getContinuousElementProbability(descriptions[i],
                                                               columnData[i],
                                                               classDescription,
                                                               classData);
        }
    }

    elementProbabilitiesT
        NaiveBayesAlgorithm::getDiscreteElementProbability(
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
            DEBUG_PRINTLN_VERBOSE_INFO(
                "For continuous attributes use continuous version of function. "
                "Returning empty array");
            break;
        default:
            FATAL_ERROR();
            break;
        }
        return elementProbabilitiesT();
    }

    distributionsElementT NaiveBayesAlgorithm::getContinuousElementProbability(
        const source::dataDescriptionElementT & description,
        const source::trainingColumnT & trainingData,
        const source::dataDescriptionElementT & classDescription,
        const source::trainingColumnT & classData) const
    {
        const auto attributeType = std::get<0>(description);
        switch (attributeType)
        {
        case source::CATEGORY:
        case source::INTEGER_DISCRETE:
        case source::REAL_DISCRETE:
            DEBUG_PRINTLN_VERBOSE_INFO(
                "For discrete attributes use discrete version of function. "
                "Returning empty array");
            break;
        case source::INTEGER:
            return numberProbability<int>(
                description,
                trainingData,
                classDescription,
                classData);
        case source::REAL:
            return numberProbability<double>(
                description,
                trainingData,
                classDescription,
                classData);
        default:
            FATAL_ERROR();
            break;
        }
        return distributionsElementT();
    }

    template <typename T>
    elementProbabilitiesT
        NaiveBayesAlgorithm::categoryProbability(
            const source::dataDescriptionElementT & description,
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
        std::unordered_map<std::string, double> classInstancesCount;

        for (size_t i = 0; i < allClassesCount; i++)
        {
            classInstancesCount.emplace(std::get<std::string>(classNames[i]), 0.0);
        }

        // 1.0L added according to Laplace estimator
        elementProbabilitiesT p_xc(attributesCount, classProbabilitiesT(allClassesCount, 1.0L));

        const auto allVectorsCount = trainingData.size();
        double allVectorsWeightsCount = 0.0;
        for (const auto& el : dataWeights)
        {
            allVectorsWeightsCount += el;
        }

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
            mapIt->second += dataWeights[row];

            // For every value of attribute
            for (size_t i = 0; i < attributesCount; i++)
            {
                const auto& processedElementName = std::get<T>(std::get<2>(description)[i]);

                // Count which attribute it is
                if (elementValue == processedElementName)
                {
                    p_xc[i][classIndex] += dataWeights[row];
                    break;
                }
            }
        }

        // Calculate probability
        for (size_t i = 0; i < p_xc.size(); i++)
        {
            auto& elements = p_xc[i];
            for (size_t j = 0; j < elements.size(); j++)
            {
                auto& probability = elements[j];
                auto mapIt = classInstancesCount.find(std::get<std::string>(classData[j].get()));
                ASSERT(mapIt != classInstancesCount.end());
                probability /= mapIt->second;
            }
        }

        return p_xc;
    }

    template<typename T>
    distributionsElementT
        NaiveBayesAlgorithm::numberProbability(
            const source::dataDescriptionElementT & description,
            const source::trainingColumnT & trainingData,
            const source::dataDescriptionElementT & classDescription,
            const source::trainingColumnT & classData) const
    {
        static_assert(source::is_int<T>::value ||
                      source::is_double<T>::value,
                      "");

        const auto attributesCount = std::get<2>(description).size();
        const auto classNames = std::get<2>(classDescription);
        const auto allClassesCount = classNames.size();
        std::unordered_map<std::string, source::dataColumnT> attributesWithClass;
        std::vector<source::dataColumnT> attributesWithClassVector(allClassesCount);

        for (size_t i = 0; i < allClassesCount; i++)
        {
            attributesWithClass.emplace(std::get<std::string>(classNames[i]), source::dataColumnT());
        }

        // Map for attributes
        for (size_t row = 0; row < trainingData.size(); row++)
        {
            const auto value = trainingData[row].get();

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

            attributesWithClassVector[classIndex].emplace_back(value);
        }

        // Map for distributions
        distributionsElementT classDistribution;
        for (const auto& el : attributesWithClassVector)
        {
            auto distribution = NormalDistribution::getNormalDistribution<T>(el);
            classDistribution.emplace_back(distribution);
        }

        return classDistribution;
    }
}