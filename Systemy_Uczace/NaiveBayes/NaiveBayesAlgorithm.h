#pragma once
#include "Algorithm.h"
#include "NaiveBayesModel.h"

class NaiveBayesAlgorithm :
    public interfaces::Algorithm
{
public:
    NaiveBayesAlgorithm(const loader::DataLoader & loader);
    ~NaiveBayesAlgorithm();

    std::unique_ptr<interfaces::Model> produceModel() override;

private:
    typedef std::vector<double> classProbabilitiesT;

    classProbabilitiesT getClassProbability(const loader::dataDescriptionT &descriptions,
                                            const loader::trainingDataT &trainingData);
};

