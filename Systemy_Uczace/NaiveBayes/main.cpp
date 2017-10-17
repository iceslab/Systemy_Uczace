#include "DataLoader.h"
#include "Crossvalidator.h"
#include "NaiveBayesAlgorithm.h"

using loader::DataLoader;

int main(int argc, char** argv)
{
    DataLoader dl("test.txt");
    Crossvalidator cv(dl);
    auto data = cv.getData();
    NaiveBayesAlgorithm nba(data.second);
    auto model = nba.produceModel();

    return 0;
}