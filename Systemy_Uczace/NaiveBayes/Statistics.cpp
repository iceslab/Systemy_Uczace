#include "Statistics.h"

namespace stats
{

    Statistics::Statistics(size_t matrixSize,
                           const source::dataDescriptionT & description,
                           const classMapT & classIndices) :
        matrix(matrixSize, confusionMatrixRowT(matrixSize)),
        description(description),
        classIndices(classIndices)
    {
        // Nothing to do
    }

    Statistics Statistics::calculateStatistics(const source::dataDescriptionT & description,
                                               const source::testDataT & testData,
                                               const source::testDataT & testResult)
    {
        ASSERT(testData.size() == testResult.size());
        ASSERT(testData.front().size() == description.size());

        const auto classes = std::get<2>(description.back());
        const auto classCount = classes.size();

        // For easier class index search
        classMapT classIndices;

        for (size_t index = 0; index < classCount; index++)
        {
            const auto className = std::get<std::string>(classes[index]);
            classIndices.emplace(className, index);
        }

        auto retVal = Statistics(classCount, description, classIndices);

        const auto dataSize = testData.size();

        for (size_t row = 0; row < dataSize; row++)
        {
            const auto resultClass = std::get<std::string>(testResult[row].back());
            const auto dataClass = std::get<std::string>(testData[row].back());
            const auto predicted = classIndices.find(resultClass)->second;
            const auto real = classIndices.find(dataClass)->second;
            retVal.matrix[predicted][real]++;
        }

        return retVal;
    }

    double Statistics::getAccuracy(source::descriptionV des) const
    {
        return getAccuracy(std::get<std::string>(des));
    }

    double Statistics::getAccuracy(std::string className) const
    {
        const auto it = classIndices.find(className);
        if (it == classIndices.end())
        {
            DEBUG_PRINTLN("Class with given name does not exist");
            return std::numeric_limits<double>::quiet_NaN();
        }

        return getAccuracy(it->second);
    }

    double Statistics::getAccuracy(size_t index) const
    {
        auto measures = getMeasuresForClass(index);
        const auto& tp = measures.truePositive;
        const auto& tn = measures.trueNegative;
        const auto& fp = measures.falsePositive;
        const auto& fn = measures.falseNegative;
        
        if (tp + fp + fn + tn == 0)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // Accuracy = TP+TN/TP+FP+FN+TN
        return static_cast<double>(tp + tn) / static_cast<double>(tp + fp + fn + tn) * 100.0L;
    }

    double Statistics::getPrecision(source::descriptionV des) const
    {
        return getPrecision(std::get<std::string>(des));
    }

    double Statistics::getPrecision(std::string className) const
    {
        const auto it = classIndices.find(className);
        if (it == classIndices.end())
        {
            DEBUG_PRINTLN("Class with given name does not exist");
            return std::numeric_limits<double>::quiet_NaN();
        }

        return getPrecision(it->second);
    }

    double Statistics::getPrecision(size_t index) const
    {
        auto measures = getMeasuresForClass(index);
        const auto& tp = measures.truePositive;
        const auto& tn = measures.trueNegative;
        const auto& fp = measures.falsePositive;
        const auto& fn = measures.falseNegative;

        if (tp + fp == 0)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // Precision = TP/TP+FP
        return static_cast<double>(tp) / static_cast<double>(tp + fp) * 100.0L;
    }

    double Statistics::getRecall(source::descriptionV des) const
    {
        return getRecall(std::get<std::string>(des));
    }

    double Statistics::getRecall(std::string className) const
    {
        const auto it = classIndices.find(className);
        if (it == classIndices.end())
        {
            DEBUG_PRINTLN("Class with given name does not exist");
            return std::numeric_limits<double>::quiet_NaN();
        }

        return getRecall(it->second);
    }

    double Statistics::getRecall(size_t index) const
    {
        auto measures = getMeasuresForClass(index);
        const auto& tp = measures.truePositive;
        const auto& tn = measures.trueNegative;
        const auto& fp = measures.falsePositive;
        const auto& fn = measures.falseNegative;

        if (tp + fn == 0)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // Recall = TP/TP+FN
        return static_cast<double>(tp) / static_cast<double>(tp + fn) * 100.0L;
    }

    MeasuresS Statistics::getMeasuresForClass(size_t index) const
    {
        const auto classesCount = matrix.size();
        MeasuresS retVal{};

        retVal.truePositive = matrix[index][index];

        for (size_t i = 0; i < classesCount; i++)
        {
            retVal.falsePositive += matrix[index][i];
            retVal.falseNegative += matrix[i][index];
        }

        retVal.falsePositive -= retVal.truePositive;
        retVal.falseNegative -= retVal.truePositive;

        for (auto& row : matrix)
        {
            for (auto& cell : row)
            {
                retVal.trueNegative += cell;
            }
        }

        retVal.trueNegative -= retVal.truePositive + retVal.falsePositive + retVal.falseNegative;
        return retVal;
    }
}