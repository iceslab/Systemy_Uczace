#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>
#include <tuple>

#include "DataDescription.h"
#include "DataVector.h"

namespace loader
{
    const char* const categoryStr = "category";
    const char* const integerStr = "integer";
    const char* const realStr = "real";
    const char* const delimitersStr = ";, ";

    const int categoryCount = 2;

    class DataLoader
    {
    public:
        DataLoader(std::string filePath);
        DataLoader(std::ifstream fileStream);
        ~DataLoader() = default;

        const dataDescriptionT& getDataDescription() const;
        const dataMatrixT& getDataMatrix() const;

    private:
        dataDescriptionT description;
        dataMatrixT matrix;

        void loadDescription(std::ifstream &fileStream);
        void loadMatrix(std::ifstream &fileStream);
    };
}
