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

    OperationsGenerator(std::vector<OperationWeight> distribution);

    size_t GetCount() const { return count; }
    void SetCount(size_t count) { this->count = count; }

    TOperations operator()();

    static std::vector<OperationWeight> GetTaskDistribution();

private:

    std::vector<Operation> operations;
    std::vector<double> weights;

    std::mt19937 gen;
    std::discrete_distribution<size_t> disc_distr;

    size_t count = 0;
};

