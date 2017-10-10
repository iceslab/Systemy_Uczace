#pragma once
#include "Algorithm.h"
class NaiveBayesAlgorithm :
    public interfaces::Algorithm
{
public:
    NaiveBayesAlgorithm(const loader::DataLoader & loader);
    ~NaiveBayesAlgorithm();
};

