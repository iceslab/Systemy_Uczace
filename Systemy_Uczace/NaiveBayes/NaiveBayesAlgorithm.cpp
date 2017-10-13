#include "NaiveBayesAlgorithm.h"

NaiveBayesAlgorithm::NaiveBayesAlgorithm(const loader::DataLoader & loader) : interfaces::Algorithm(loader)
{
}

NaiveBayesAlgorithm::~NaiveBayesAlgorithm()
{
}

std::unique_ptr<interfaces::Model> NaiveBayesAlgorithm::produceModel()
{
    auto descriptions = loader.getDataDescription();
    // Vector of names for all classifiers
    auto& classNames = descriptions.back();
    auto trainingData = loader.getDataMatrix();

    const auto classCount = std::get<2>(classNames).size();
    const auto allClassesCount = trainingData.size();

    // Class probability
    std::vector<double> p_c(classCount);
    for (size_t i = 0; i < classCount; i++)
    {
        // Name of currently processed classifier
        const auto& processedClassName = std::get<2>(classNames)[i];
        size_t processedClassCount = 0;
        for(auto& dataRow : trainingData)
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

    std::vector<double> classProbability(classCount);

    //P(C | X) = (P(X | C) * P(C)) / P(X)

    auto retVal = std::make_unique<interfaces::Model>();
    return retVal;
}
