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
    Discretizer::discretize(dl, NUMBER_OF_BUCKETS);
    Crossvalidator cv(dl);
    auto data = cv.getData();
    NaiveBayesAlgorithm nba(dl.getDataDescription(), data.second);
    NaiveBayesModel nbm(data.first, nba);

    return 0;
}