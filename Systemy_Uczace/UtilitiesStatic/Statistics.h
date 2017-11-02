#pragma once
#include "DataSource.h"

namespace stats
{
    typedef std::vector<size_t> confusionMatrixRowT;
    typedef std::vector<confusionMatrixRowT> confusionMatrixT;
    typedef std::unordered_map<std::string, size_t> classMapT;

    struct MeasuresS
    {
        size_t truePositive;
        size_t trueNegative;
        size_t falsePositive;
        size_t falseNegative;
    };

    class Statistics
    {
    private:
        Statistics(size_t matrixSize,
                   const source::dataDescriptionT & description,
                   const classMapT & classIndices);
    public:
        ~Statistics() = default;
        static Statistics calculateStatistics(const source::dataDescriptionT & description,
                                              const source::testDataT & testData,
                                              const source::testDataT & testResult);

        static Statistics calculateMean(const std::vector<Statistics> & statistics);

        double getAccuracy(source::descriptionV des) const;
        double getAccuracy(std::string className) const;
        double getAccuracy(size_t index) const;

        double getPrecision(source::descriptionV des) const;
        double getPrecision(std::string className) const;
        double getPrecision(size_t index) const;

        double getRecall(source::descriptionV des) const;
        double getRecall(std::string className) const;
        double getRecall(size_t index) const;

        double getFscore(source::descriptionV des) const;
        double getFscore(std::string className) const;
        double getFscore(size_t index) const;

        void saveToFile(const std::string & path) const;
        void saveToFile(std::ofstream& fileStream) const;

    private:
        const source::dataDescriptionT description;
        confusionMatrixT matrix;
        const classMapT classIndices;

        MeasuresS getMeasuresForClass(size_t index) const;
    };
}
