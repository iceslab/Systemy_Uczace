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

    Statistics Statistics::calculateMean(const std::vector<Statistics>& statistics)
    {

        ASSERT(!statistics.empty());
        const auto matrixSize = statistics.front().matrix.size();
        const auto description = statistics.front().description;
        const auto classIndices = statistics.front().classIndices;

        for (const auto& stat : statistics)
        {
            ASSERT(matrixSize == stat.matrix.size());
            ASSERT(description == stat.description);
            ASSERT(classIndices == stat.classIndices);
        }

        auto retVal = Statistics(matrixSize, description, classIndices);

        for (const auto& stat : statistics)
        {
            for (size_t row = 0; row < matrixSize; row++)
            {
                for (size_t column = 0; column < matrixSize; column++)
                {
                    auto& retValCell = retVal.matrix[row][column];
                    const auto& statCell = stat.matrix[row][column];
                    retValCell += statCell;
                }
            }
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
            DEBUG_PRINTLN_VERBOSE_WARNING("Class with given name does not exist");
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
        return static_cast<double>(tp + tn) / static_cast<double>(tp + fp + fn + tn);
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
            DEBUG_PRINTLN_VERBOSE_WARNING("Class with given name does not exist");
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
        return static_cast<double>(tp) / static_cast<double>(tp + fp);
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
            DEBUG_PRINTLN_VERBOSE_WARNING("Class with given name does not exist");
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
        return static_cast<double>(tp) / static_cast<double>(tp + fn);
    }

    double Statistics::getFscore(source::descriptionV des) const
    {
        return getFscore(std::get<std::string>(des));
    }

    double Statistics::getFscore(std::string className) const
    {
        const auto it = classIndices.find(className);
        if (it == classIndices.end())
        {
            DEBUG_PRINTLN_VERBOSE_WARNING("Class with given name does not exist");
            return std::numeric_limits<double>::quiet_NaN();
        }

        return getFscore(it->second);
    }

    double Statistics::getFscore(size_t index) const
    {
        auto measures = getMeasuresForClass(index);
        const auto& tp = measures.truePositive;
        const auto& tn = measures.trueNegative;
        const auto& fp = measures.falsePositive;
        const auto& fn = measures.falseNegative;

        if (2 * tp + fp + fn == 0)
        {
            return std::numeric_limits<double>::quiet_NaN();
        }

        // Fscore = 2*TP / (2*TP + FP + FN)
        return static_cast<double>(2 * tp) / static_cast<double>(2 * tp + fp + fn);
    }

    void Statistics::saveToFile(const std::string & path) const
    {
        saveToFile(std::ofstream(path));
    }

    void Statistics::saveToFile(std::ofstream & fileStream) const
    {
        if (!fileStream.is_open())
        {
            DEBUG_PRINTLN_VERBOSE_WARNING("File is not open. Exiting function");
            return;
        }

        fileStream << "accuracy precision recall fscore" << std::endl;
        std::ostream_iterator<std::string> out_it(fileStream, " ");
        const auto& classNames = std::get<2>(description.back());
        for (const auto& el : classNames)
        {
            const auto& className = std::get<std::string>(el);
            std::array<std::string, 5> row = {
                className,
                std::to_string(getAccuracy(className)),
                std::to_string(getPrecision(className)),
                std::to_string(getRecall(className)),
                std::to_string(getFscore(className))
            };
            std::copy(row.begin(), row.end(), out_it);
            fileStream << std::endl;
        }
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