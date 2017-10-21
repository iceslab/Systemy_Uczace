#include "DataSource.h"
#include "Discretizer.h"
#include "Crossvalidator.h"
#include "NaiveBayesAlgorithm.h"
#include "NaiveBayesModel.h"
#include "Statistics.h"

#define NUMBER_OF_BUCKETS 10

using source::DataSource;
using crossvalidator::Crossvalidator;
using algorithm::NaiveBayesAlgorithm;
using model::NaiveBayesModel;
using stats::Statistics;

int main(int argc, char** argv)
{
    const std::vector<std::string> names =
    {
        "data/iris.txt",
        "data/glass.txt",
        "data/wine.txt",
        "data/diabetes.txt",
        "data/car.txt",
        "data/transfusion.txt"
    };

    for (const auto& path : names)
    {
        printf("\n========== Path: %-20s ==========\n\n", path.c_str());
        DataSource dl(path);
        Discretizer discretizer(dl, NUMBER_OF_BUCKETS);
        discretizer.discretize();
        Crossvalidator cv(dl);

        while (cv.hasNext())
        {
            auto data = cv.getNextData();
            auto testData = data.first;
            auto trainingData = data.second;
            NaiveBayesAlgorithm nba(dl.getDataDescription(), trainingData);
            NaiveBayesModel nbm(testData, nba);
            auto testResult = nbm.classify();

            auto stats = Statistics::calculateStatistics(dl.getDataDescription(),
                                                         testData,
                                                         testResult);

            for (auto& description : std::get<2>(dl.getDataDescription().back()))
            {
                const auto className = std::get<std::string>(description);
                printf("%s: accuracy: %3.2lf%% precision: %3.2lf%% recall: %3.2lf%%\n",
                       className.c_str(),
                       stats.getAccuracy(className),
                       stats.getPrecision(className),
                       stats.getRecall(className));

            }
            printf("\n");
        }
    }

    system("pause");
    return 0;
}
