#include "Algorithm.h"

namespace interfaces
{
    Algorithm::Algorithm(const loader::DataLoader & loader) : loader(loader)
    {
    }

    Algorithm::~Algorithm()
    {
    }

    Model & Algorithm::produceModel()
    {
        auto descriptions = loader.getDataDescription();
        auto trainingData = loader.getDataMatrix();
        
        auto classCount = std::get<2>(descriptions.back()).size();

        return Model();
    }
};


