#include "DataLoader.h"
#include "Crossvalidator.h"

using loader::DataLoader;

int main(int argc, char** argv)
{
    DataLoader dl("test.txt");
    Crossvalidator cv(dl);

    return 0;
}