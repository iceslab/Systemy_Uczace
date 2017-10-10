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

    // Array of tuples containing type, name and optional values
    typedef std::vector<std::tuple<DataTypeE, std::string, std::vector<std::string>>> dataDescriptionT;
    // Array of vectors
    typedef std::vector<std::vector<std::string>> dataMatrixT;

    class DataLoader
    {
    public:
        DataLoader(std::string filePath);
        DataLoader(std::ifstream fileStream);
        ~DataLoader() = default;

        const DataDescription& getDataDescription() const;
        const DataMatrixT& getDataMatrix() const;

    private:
        DataDescription description;
        DataMatrixT matrix;

        void loadDescription(std::ifstream &fileStream);
        void loadMatrix(std::ifstream &fileStream);
    };
}
