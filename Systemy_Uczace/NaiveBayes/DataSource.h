#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <exception>
#include <sstream>
#include <tuple>

#include "DataDescription.h"
#include "DataVector.h"

namespace source
{
    const char* const categoryStr = "category";
    const char* const integerStr = "integer";
    const char* const realStr = "real";
    const char* const delimitersStr = ";, ";

    const int categoryCount = 2;

    class DataSource
    {
    public:
        DataSource(std::string filePath);
        DataSource(std::ifstream fileStream);
        ~DataSource() = default;

        const dataDescriptionT& getDataDescription() const;
        const dataMatrixT& getDataMatrix() const;

        dataDescriptionT& getDataDescription();
        dataMatrixT& getDataMatrix();

        static DataV variantFromString(source::DataTypeE type, std::string value);
    private:
        dataDescriptionT description;
        dataMatrixT matrix;

        void loadDescription(std::ifstream &fileStream);
        void loadMatrix(std::ifstream &fileStream);
    };
}
