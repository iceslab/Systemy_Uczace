#pragma once
#include "Algorithm.h"
#include "NaiveBayesModel.h"
#include "asserts.h"

class NaiveBayesAlgorithm :
    public interfaces::Algorithm
{
public:
    NaiveBayesAlgorithm(const loader::DataLoader & loader);
    ~NaiveBayesAlgorithm();

    std::unique_ptr<interfaces::Model> produceModel() override;

private:
    typedef std::vector<double> classProbabilitiesT;
    typedef std::vector<std::vector<double>> elementProbabilitiesT;

    classProbabilitiesT getClassProbability(const loader::dataDescriptionT &descriptions,
                                            const loader::trainingDataT &trainingData);
    elementProbabilitiesT getElementPorbability(const loader::dataDescriptionT &descriptions,
                                                const loader::trainingDataT &trainingData);

};

