#pragma once
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

        static dataV variantFromString(source::DataTypeE type, std::string value);

        static size_t getElementIndex(source::DataTypeE type,
                                      const source::dataV & data,
                                      const source::dataDescriptionElementT & description);
        template<typename T>
        static size_t getElementIndex(const source::dataV & data,
                                      const source::dataDescriptionElementT & description);

        void shuffleData();

    private:
        dataDescriptionT description;
        dataMatrixT matrix;

        void loadDescription(std::ifstream &fileStream);
        void loadMatrix(std::ifstream &fileStream);
    };
}
