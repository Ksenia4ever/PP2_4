#include <stdexcept>

#include "OperationsGenerator.h"


OperationsGenerator::OperationsGenerator(const TOperationDistribution& distribution)
{
    if (distribution.empty())
    {
        throw std::invalid_argument("distribution is empty");
    }

    for (const auto& data : distribution)
    {
        operations.push_back(data.Op);
        weights.push_back(data.Weight);
    }

    gen = std::mt19937(std::random_device{}());
    disc_distr = std::discrete_distribution<size_t>(weights.begin(), weights.end());
}

TOperations OperationsGenerator::operator()()
{
    TOperations res;

    for (size_t index = 0; index < GetCount(); index++)
    {
        const auto op_index = disc_distr(gen);
        const auto op = operations[op_index];
        res.push_back(op);
    }

    return res;
}

OperationsGenerator::TOperationDistribution OperationsGenerator::GetTaskDistribution()
{
    auto res = TOperationDistribution
    {
        { Operation{Operation::Action::Read, 0}, 10 },      // 10% read field#0
        { Operation{Operation::Action::Write, 0, 100}, 10 },// 10% write field#0

        { Operation{Operation::Action::Read, 1}, 50 },      // 50% read field#1
        { Operation{Operation::Action::Write, 1, 200}, 10 },// 10% write field#1

        { Operation{Operation::Action::Read, 2}, 5 },       // 10% read field#2
        { Operation{Operation::Action::Write, 2, 300}, 5 }, // 10% write field#2

        { Operation{Operation::Action::String}, 10 },       // 10% string
    };

    return res;
}

OperationsGenerator::TOperationDistribution OperationsGenerator::GetUniformDistribution()
{
    // All operations have equal weight == 1
    auto res = TOperationDistribution
    {
        { Operation{Operation::Action::Read, 0}, 1 },      // read field#0
        { Operation{Operation::Action::Write, 0, 100}, 1 },// write field#0

        { Operation{Operation::Action::Read, 1}, 1 },      // read field#1
        { Operation{Operation::Action::Write, 1, 200}, 1 },// write field#1

        { Operation{Operation::Action::Read, 2}, 1 },      // read field#2
        { Operation{Operation::Action::Write, 2, 300}, 1 },// write field#2

        { Operation{Operation::Action::String}, 1 },       // string
    };

    return res;
}

OperationsGenerator::TOperationDistribution OperationsGenerator::GetNonTaskDistribution()
{
    // All operations have equal weights that significantly differ from task weight
    auto res = TOperationDistribution
    {
        { Operation{Operation::Action::Read, 0}, 3 },       // 3% read field#0
        { Operation{Operation::Action::Write, 0, 100}, 1 }, // 1% write field#0

        { Operation{Operation::Action::Read, 1}, 5 },       // 5% read field#1
        { Operation{Operation::Action::Write, 1, 200}, 40 },// 40% write field#1

        { Operation{Operation::Action::Read, 2}, 20 },      // 20% read field#2
        { Operation{Operation::Action::Write, 2, 300}, 1 }, // 1% write field#2

        { Operation{Operation::Action::String}, 30 },       // 30% string
    };

    return res;
}
