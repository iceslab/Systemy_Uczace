#include "DataSource.h"
#include "Discretizer.h"
#include "Crossvalidator.h"
#include "NaiveBayesAlgorithm.h"
#include "NaiveBayesModel.h"
#include "Statistics.h"

#define NUMBER_OF_BUCKETS 10

using source::DataSource;
using algorithm::NaiveBayesAlgorithm;
using model::NaiveBayesModel;
using stats::Statistics;

int main(int argc, char** argv)
{
    DataSource dl("data/iris.txt");
    Discretizer discretizer(dl, NUMBER_OF_BUCKETS);
    discretizer.discretize();
    Crossvalidator cv(dl);
    auto data = cv.getData();
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

    system("pause");
    return 0;
}