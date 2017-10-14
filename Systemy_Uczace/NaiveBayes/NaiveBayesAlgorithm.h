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
    typedef std::vector<classProbabilitiesT> elementProbabilitiesT;
    typedef std::vector<elementProbabilitiesT> attributesProbabilitiesT;

    classProbabilitiesT getClassProbability(const loader::dataDescriptionT &descriptions,
                                            const loader::trainingDataT &trainingData);
    attributesProbabilitiesT getAttributesProbability(const loader::dataDescriptionT &descriptions,
                                                      const loader::trainingDataT &trainingData);

    elementProbabilitiesT getElementProbability(const loader::dataDescriptionElementT &description,
                                                const loader::trainingColumnT &trainingData,
                                                const loader::dataDescriptionElementT &classDescription,
                                                const loader::trainingColumnT &classData);
    elementProbabilitiesT categoryProbability(const loader::dataDescriptionElementT &description,
                                              const loader::trainingColumnT &trainingData,
                                              const loader::dataDescriptionElementT &classDescription,
                                              const loader::trainingColumnT &classData);
    elementProbabilitiesT numberProbability(const loader::dataDescriptionElementT &description,
                                            const loader::trainingColumnT &trainingData,
                                            const loader::dataDescriptionElementT &classDescription,
                                            const loader::trainingColumnT &classData);
    classProbabilitiesT classProbability(const loader::dataDescriptionElementT &description,
                                         const loader::trainingColumnT &trainingData);
};

