#pragma once

#include <vector>
#include <random>

#include "Operation.h"

class OperationsGenerator
{
public:

    struct OperationWeight
    {
        Operation Op;
        double Weight = 0;
    };
    using TOperationDistribution = std::vector<OperationWeight>;


    OperationsGenerator(const TOperationDistribution& distribution);

    size_t GetCount() const { return count; }
    void SetCount(size_t count) { this->count = count; }

    TOperations operator()();

    static TOperationDistribution GetTaskDistribution();
    static TOperationDistribution GetUniformDistribution();
    static TOperationDistribution GetNonTaskDistribution();

private:

    std::vector<Operation> operations;
    std::vector<double> weights;

    std::mt19937 gen;
    std::discrete_distribution<size_t> disc_distr;

    size_t count = 0;
};

