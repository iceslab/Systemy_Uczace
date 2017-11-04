#include "DataSource.h"
#include "DiscretizerFactory.h"
#include "Crossvalidator.h"
#include "ILAAlgorithm.h"
#include "ILAModel.h"
#include "Statistics.h"
#include <filesystem>

#define IRIS_ONLY
#define FINAL_TESTS
#define NUMBER_OF_BUCKETS 10

using source::DataSource;
using discretizer::DiscretizerFactory;
using crossvalidator::Crossvalidator;
using algorithm::ILAAlgorithm;
using model::ILAModel;
using stats::Statistics;

int main(int argc, char** argv)
{
    // Disable warnings
    UNREFERENCED_PARAMETER(argc);
    UNREFERENCED_PARAMETER(argv);

    const std::string testDataDir = "../data/";
    const std::string resultsDataDir = "../results/";
    const std::vector<std::string> names =
    {
#ifdef IRIS_ONLY
        "iris.txt"
#else
        "glass_test.txt",
        "car_test.txt",
        "vertrebal_column_test.txt"
#ifndef FINAL_TESTS        
        ,
        "iris.txt",
        "wine.txt",
        "diabetes.txt",
        "transfusion.txt",
        "seeds.txt"
#endif
#endif
    };

    const std::vector<discretizer::DiscretizerTypeE> discretizersTypes =
    {
        discretizer::CLASSIC,
        discretizer::UNIFORM,
    };

    const std::vector<std::string> discretizersNames =
    {
        "CLASSIC",
        "UNIFORM",
    };

    std::experimental::filesystem::create_directory(resultsDataDir);

    std::vector<size_t> namesLenghts;
    for (auto& path : names)
    {
        namesLenghts.emplace_back(testDataDir.size() + path.size());
    }
    const auto maxPathLenght = static_cast<int>(*std::max_element(namesLenghts.begin(),
                                                                  namesLenghts.end()));

    for (const auto& path : names)
    {
        DataSource dl(testDataDir + path);
        dl.shuffleData();
        for (size_t i = 0; i < discretizersTypes.size(); i++)
        {
            const auto& discretizerType = discretizersTypes[i];
            printf("\n========== Path: %*s ==========\n\n", -maxPathLenght, (testDataDir + path).c_str());

            auto description(dl.getDataDescription());
            auto matrix(dl.getDataMatrix());
            if (discretizerType != discretizer::NONE)
            {
                auto discretizer = DiscretizerFactory::getDiscretizer(discretizerType,
                                                                      description,
                                                                      matrix,
                                                                      NUMBER_OF_BUCKETS);
                discretizer->discretize();
            }

            Crossvalidator cv(matrix);
            std::vector<Statistics> allStats;

            while (cv.hasNext())
            {
                auto data = cv.getNextData();
                auto testData = data.first;
                auto trainingData = data.second;
                ILAAlgorithm ilaa(description, trainingData);
                ILAModel ilam(testData, ilaa);
                auto testResult = ilam.classify();

                auto stats = Statistics::calculateStatistics(dl.getDataDescription(),
                                                             testData,
                                                             testResult);
                allStats.emplace_back(stats);

                DEBUG_CALL(
                    const auto maxClassNameLenght = dl.getDataDescription().getLongestClassNameLength();
                for (auto& dataDescription : std::get<2>(dl.getDataDescription().back()))
                {
                    const auto className = std::get<std::string>(dataDescription);
                    printf("%*s: accuracy: %6.2lf%% "
                           "precision: %6.2lf%% "
                           "recall: %6.2lf%% "
                           "fscore: %6.2lf%%\n",
                           -static_cast<int>(maxClassNameLenght),
                           className.c_str(),
                           stats.getAccuracy(className) * 100.0L,
                           stats.getPrecision(className) * 100.0L,
                           stats.getRecall(className) * 100.0L,
                           stats.getFscore(className) * 100.0L);

                }
                printf("\n");
                );
            }

            const auto maxClassNameLenght = dl.getDataDescription().getLongestClassNameLength();
            auto stats = Statistics::calculateMean(allStats);

            const auto& discretizerName = discretizersNames[i];
            stats.saveToFile(resultsDataDir + discretizerName + "_" + path);
            for (auto& dataDescription : std::get<2>(dl.getDataDescription().back()))
            {
                const auto className = std::get<std::string>(dataDescription);
                printf("%*s: accuracy: %6.2lf%% "
                       "precision: %6.2lf%% "
                       "recall: %6.2lf%% "
                       "fscore: %6.2lf%%\n",
                       -static_cast<int>(maxClassNameLenght),
                       className.c_str(),
                       stats.getAccuracy(className) * 100.0L,
                       stats.getPrecision(className) * 100.0L,
                       stats.getRecall(className) * 100.0L,
                       stats.getFscore(className) * 100.0L);

            }
            printf("\n");
        }
    }

    system("pause");
    return 0;
}
