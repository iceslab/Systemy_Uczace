#pragma once
#include "Model.h"
#include "asserts.h"
#include "ILAAlgorithm.h"

namespace model
{
    class ILAModel : abstracts::Model
    {
    public:
        ILAModel(const source::testDataT & testData,
                 const algorithm::ILAAlgorithm & algorithm);
        ~ILAModel() = default;

        virtual source::testDataT classify() override;
    };
}
