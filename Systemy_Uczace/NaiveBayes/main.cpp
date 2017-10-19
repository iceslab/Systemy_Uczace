#include "DataLoader.h"
#include "Crossvalidator.h"
#include "NaiveBayesAlgorithm.h"

using loader::DataLoader;
using algorithm::NaiveBayesAlgorithm;

int main(int argc, char** argv)
{
    DataLoader dl("test.txt");
    Crossvalidator cv(dl);
    auto data = cv.getData();
    NaiveBayesAlgorithm nba(dl.getDataDescription(), data.second);

    return 0;
}