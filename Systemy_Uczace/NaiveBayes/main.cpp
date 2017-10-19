#include "DataSource.h"
#include "Crossvalidator.h"
#include "NaiveBayesAlgorithm.h"

using source::DataSource;
using algorithm::NaiveBayesAlgorithm;

int main(int argc, char** argv)
{
    DataSource dl("test.txt");

    Crossvalidator cv(dl);
    auto data = cv.getData();
    NaiveBayesAlgorithm nba(dl.getDataDescription(), data.second);

    return 0;
}