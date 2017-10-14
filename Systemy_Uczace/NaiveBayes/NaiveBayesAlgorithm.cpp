#include "NaiveBayesAlgorithm.h"

NaiveBayesAlgorithm::NaiveBayesAlgorithm(const loader::DataLoader & loader) :
    interfaces::Algorithm(loader)
{
}

NaiveBayesAlgorithm::~NaiveBayesAlgorithm()
{
}

std::unique_ptr<interfaces::Model> NaiveBayesAlgorithm::produceModel()
{
    const auto descriptions = loader.getDataDescription();
    const auto trainingData = loader.getDataMatrix();


    //P(C | X) = (P(X | C) * P(C)) / P(X)

    auto retVal = std::make_unique<interfaces::Model>();
    return retVal;
}

NaiveBayesAlgorithm::classProbabilitiesT
NaiveBayesAlgorithm::getClassProbability(const loader::dataDescriptionT &descriptions,
                                         const loader::trainingDataT &trainingData)
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
        const auto& processedClassName = std::get<2>(classNames)[i];
        size_t processedClassCount = 0;
        for (auto& dataRow : trainingData)
        {
            // Class name in training data
            const auto& classValue = dataRow.back();
            if (classValue == processedClassName)
            {
                ++processedClassCount;
            }
        }

        p_c[i] = static_cast<double>(processedClassCount) / static_cast<double>(allClassesCount);
    }

    return p_c;
}

NaiveBayesAlgorithm::elementProbabilitiesT
NaiveBayesAlgorithm::getElementPorbability(const loader::dataDescriptionT & descriptions,
                                           const loader::trainingDataT & trainingData)
{
    ASSERT(descriptions.size() > 1);
    const auto attributesCount = descriptions.size() - 1;
    elementProbabilitiesT p_xc(attributesCount);

    const auto allVectorsCount = trainingData.size();

    for (size_t i = 0; i < attributesCount; i++)
    {
        // Vector of names for all classifiers
        const auto& attributeNames = descriptions[i];
        const auto attributeType = std::get<0>(attributeNames);

        // TODO: implement non-category probaility calculation
        if (attributeType != loader::CATEGORY)
            continue;

        const auto elementCount = std::get<2>(attributeNames).size();

        auto& elementsProbability = p_xc[i];
        elementsProbability.resize(elementCount);

        for (auto& dataRow : trainingData)
        {
            // For every value of attribute
            for (size_t j = 0; j < elementCount; j++)
            {
                // Name of currently processed classifier
                const auto& processedElementName = std::get<2>(attributeNames)[j];

                // Class name in training data
                const auto& elementValue = dataRow[i];
                if (elementValue == processedElementName)
                {
                    elementsProbability[j]++;
                    break;
                }
            }
        }

        // For every value of attribute
        for (size_t j = 0; j < elementCount; j++)
        {
            elementsProbability[j] /= static_cast<double>(allVectorsCount);
        }
    }

    return p_xc;
}
