#include "DataSource.h"
#include "Discretizer.h"
#include "Crossvalidator.h"
#include "NaiveBayesAlgorithm.h"
#include "NaiveBayesModel.h"

#define NUMBER_OF_BUCKETS 10

using source::DataSource;
using algorithm::NaiveBayesAlgorithm;
using model::NaiveBayesModel;

int main(int argc, char** argv)
{
    DataSource dl("test.txt");
    Discretizer discretizer(dl, NUMBER_OF_BUCKETS);
    discretizer.discretize();
    Crossvalidator cv(dl);
    auto data = cv.getData();
    auto testData = data.first;
    auto trainingData = data.second;
    NaiveBayesAlgorithm nba(dl.getDataDescription(), trainingData);
    NaiveBayesModel nbm(testData, nba);
    auto result = nbm.classify();

    return 0;
}