#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>
#include <tuple>

namespace loader
{
    enum DataTypeE
    {
        CATEGORY,
        INTEGER,
        REAL,
        UNDEFINED
    };
    
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
        ~DataLoader();

        const dataDescriptionT& getDataDescription() const;
        const dataMatrixT& getDataMatrix() const;

    private:
        dataDescriptionT description;
        dataMatrixT matrix;
    };
}
