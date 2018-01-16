#include "DataSource.h"
#include "DiscretizerFactory.h"
#include "Crossvalidator.h"
#include "Bagger.h"
#include "Booster.h"
#include "Statistics.h"
#include <filesystem>

#define FINAL_TESTS
#define NUMBER_OF_BUCKETS 10
#define DISCRETIZATIONS 3U
#define OUT_FILE_NAME "allStats.txt"

using source::DataSource;
using discretizer::DiscretizerFactory;
using crossvalidator::Crossvalidator;
using algorithm::NaiveBayesAlgorithm;
using model::NaiveBayesModel;
using stats::Statistics;
using ensembles::Bagger;
using ensembles::Booster;

int main(int argc, char** argv)
{
    const std::string testDataDir = "../data/";
    const std::string resultsDataDir = "../results/";
    const std::vector<std::string> names =
    {
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
    };

    const std::vector<discretizer::DiscretizerTypeE> discretizersTypes =
    {
        discretizer::CLASSIC,
        discretizer::UNIFORM,
        discretizer::NONE
    };

    const std::vector<std::string> discretizersNames =
    {
        "CLASSIC",
        "UNIFORM",
        "NONE"
    };

    std::experimental::filesystem::create_directory(resultsDataDir);

    std::vector<size_t> namesLenghts;
    for (auto& path : names)
    {
        namesLenghts.emplace_back(testDataDir.size() + path.size());
    }
    const auto maxPathLenght = static_cast<int>(*std::max_element(namesLenghts.begin(),
                                                                  namesLenghts.end()));

    FILE* outFile = nullptr;
    fopen_s(&outFile, OUT_FILE_NAME, "w");
    if (outFile == nullptr)
    {
        printf("Couldn't open file \"%s\" for writing. Exiting...", OUT_FILE_NAME);
    }
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

            fprintf(outFile, "path discretizer voting ensemble className accuracy precision recall fscore\n");
            for (size_t ensemble = 0; ensemble < 2; ensemble++)
            {
                for (size_t votingType = 0; votingType < 2; votingType++)
                {
                    std::string votingName = "WEIGHTED";
                    classifier::VotingTypeE votingTypeEnum = classifier::WEIGHTED;
                    if (votingType == 0)
                    {
                        votingTypeEnum = classifier::NORMAL;
                        votingName = "NORMAL";
                    }

                    source::testDataT testData;
                    source::testDataT testResult;
                    std::string classifierName;
                    if (ensemble == 0)
                    {
                        Bagger b(dl, votingTypeEnum, 10);
                        testResult = b.classify();
                        testData = b.getTestData();
                        classifierName = "BAGGER";
                    }
                    else
                    {
                        Booster b(dl, votingTypeEnum, 10, 1.0);
                        testResult = b.classify();
                        testData = b.getTestData();
                        classifierName = "BOOSTER";
                    }

                    auto stats = Statistics::calculateStatistics(dl.getDataDescription(),
                                                                 testData,
                                                                 testResult);
                    const auto& discretizerName = discretizersNames[i];
                    stats.saveToFile(resultsDataDir + discretizerName + "_" + votingName + "_" + classifierName + "_" + path);
                    DEBUG_CALL(
                        const auto maxClassNameLenght = dl.getDataDescription().getLongestClassNameLength();
                    for (auto& description : std::get<2>(dl.getDataDescription().back()))
                    {
                        const auto className = std::get<std::string>(description);
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
            }
        }
    }

    system("pause");
    return 0;
}
