#include "DataSource.h"
#include "DiscretizerFactory.h"
#include "Crossvalidator.h"
#include "KNNAlgorithm.h"
#include "KNNModel.h"
#include "KNNFactory.h"
#include "Statistics.h"
#include <filesystem>

//#define IRIS_ONLY
#define FINAL_TESTS
#define OUT_FILE_NAME "allStats.txt"

using source::DataSource;
using discretizer::DiscretizerFactory;
using crossvalidator::Crossvalidator;
using algorithm::KNNAlgorithm;
using model::KNNModel;
using model::KNNFactory;
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

    const std::vector<model::DistanceFunctionE> distanceType =
    {
        model::MANHATTAN,
        model::EUCLIDEAN,
        model::MINKOWSKI
    };

    const std::vector<std::string> distanceNames =
    {
        "MANHATTAN",
        "EUCLIDEAN",
        "MINKOWSKI"
    };

    const std::vector<model::VotingMethodE> votingType =
    {
        model::WEIGHTED,
        model::MAJORITY,
    };

    const std::vector<std::string> votingNames =
    {
        "WEIGHTED",
        "MAJORITY"
    };

    const std::vector<size_t> kValue =
    {
        2,
        5,
        10
    };

    std::experimental::filesystem::create_directory(resultsDataDir);

    std::vector<size_t> namesLenghts;
    for (auto& path : names)
    {
        namesLenghts.emplace_back(testDataDir.size() + path.size());
    }
    const auto maxPathLength = static_cast<int>(*std::max_element(namesLenghts.begin(),
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

        printf("\n========== Path: %*s ==========\n", -maxPathLength, (testDataDir + path).c_str());
        auto description(dl.getDataDescription());
        auto matrix(dl.getDataMatrix());

        Crossvalidator cv(matrix);
        size_t fold = 1;

        fprintf(outFile, "path distance voting k fold className accuracy precision recall fscore\n");
        while (cv.hasNext())
        {
            auto data = cv.getNextData();
            auto testData = data.first;
            auto trainingData = data.second;

            for (size_t i = 0; i < distanceType.size(); i++)
            {
                for (size_t j = 0; j < votingType.size(); j++)
                {
                    for (size_t k = 0; k < kValue.size(); k++)
                    {
                        KNNAlgorithm knna(description, trainingData);
                        auto knnm_p = KNNFactory::getKNNModel(testData, knna, distanceType[i], votingType[j], kValue[k]);
                        auto testResult = knnm_p->classify();

                        auto stats = Statistics::calculateStatistics(dl.getDataDescription(),
                                                                     testData,
                                                                     testResult);

                        for (auto& dataDescription : std::get<2>(dl.getDataDescription().back()))
                        {
                            const auto className = std::get<std::string>(dataDescription);
                            fprintf(outFile, "%s %s %s %zu %zu %s %f %f %f %f\n",
                                    path.c_str(),
                                    distanceNames[i].c_str(),
                                    votingNames[j].c_str(),
                                    kValue[k],
                                    fold,
                                    className.c_str(),
                                    stats.getAccuracy(className),
                                    stats.getPrecision(className),
                                    stats.getRecall(className),
                                    stats.getFscore(className));
                            DEBUG_CALL(fprintf(stderr, "%s %s %s %zu %zu %s %f %f %f %f\n",
                                               path.c_str(),
                                               distanceNames[i].c_str(),
                                               votingNames[j].c_str(),
                                               kValue[k],
                                               fold,
                                               className.c_str(),
                                               stats.getAccuracy(className),
                                               stats.getPrecision(className),
                                               stats.getRecall(className),
                                               stats.getFscore(className)););
                        }
                    }
                }
            }
            fold++;
        }
    }
    // system("pause");
    return 0;
}