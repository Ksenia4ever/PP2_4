// Visual Studio 2022
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <format>
#include <thread>
#include <list>
#include <stdexcept>
#include <syncstream>

#include "OperationsGenerator.h"
#include "OperationsSerializer.h"
#include "PerformanceTester.h"
#include "MTStructure.h"


void PrepareFiles(int fileCount, const OperationsGenerator::TOperationDistribution& distribution)
{
    std::cout << std::format("\tGenerating {} input file(s)", fileCount) << std::endl;

    auto generator = OperationsGenerator(distribution);
    generator.SetCount(100000);

    for (int fileIndex = 0; fileIndex < fileCount; fileIndex++)
    {
        const auto ops = generator();

        auto serializer = OperationsSerializer(std::format("file{}.txt", fileIndex));
        serializer.Write(ops);
    }

}

void TestStructure(int threadCount)
{
    std::cout << std::format("\tTesting by {} thread(s)", threadCount) << std::endl;

    MTStructure mts;

    auto threads = std::list<std::thread>();
    auto threadOps = std::vector<TOperations>();

    for (int threadIndex = 0; threadIndex < threadCount; threadIndex++)
    {
        auto serializer = OperationsSerializer(std::format("file{}.txt", threadIndex));
        auto ops = serializer.Read();
        threadOps.push_back(std::move(ops));
    }

    for (int threadIndex = 0; threadIndex < threadCount; threadIndex++)
    {
        auto th = std::thread([=, &mts, &threadOps]()
        {
            const auto& ops = threadOps[threadIndex];

            auto perfTester = PerformanceTester();
            perfTester.Measure([&]()
            {
                for (const auto& op : ops)
                {
                    switch (op.Do)
                    {
                    case Operation::Action::Write:
                        mts.SetField(op.FieldIndex, op.FieldValue);
                        break;
                    case Operation::Action::Read:
                        mts.GetField(op.FieldIndex);
                        break;
                    case Operation::Action::String:
                        mts.operator std::string();
                        break;
                    default:
                        throw std::invalid_argument("Invalis operation.");
                        break;
                    }
                }
            });
            auto mcs = perfTester.GetMeasureDuration();
            std::osyncstream(std::cout) << "\t\tThread#" << threadIndex << ": " << mcs << " mcs." << std::endl;
        });

        threads.push_back(std::move(th));
    }
    
    for (auto& th : threads)
    {
        if (th.joinable())
        {
            th.join();
        }
    }
}

void RunProgram()
{
    const auto threadCount = 3;

    {
        std::cout << "Test#1 Distrubution variant#3" << std::endl;

        PrepareFiles(threadCount, OperationsGenerator::GetTaskDistribution());

        for (int threads = 1; threads <= threadCount; threads++)
        {
            TestStructure(threads);
        }

        std::cout << std::endl << std::endl;
    }

    {
        std::cout << "Test#2 Uniform distrubution (all weights are equal)" << std::endl;

        PrepareFiles(threadCount, OperationsGenerator::GetUniformDistribution());

        for (int threads = 1; threads <= threadCount; threads++)
        {
            TestStructure(threads);
        }

        std::cout << std::endl << std::endl;
    }

    {
        std::cout << "Test#3 Distrubution significantly differs from variant#3" << std::endl;

        PrepareFiles(threadCount, OperationsGenerator::GetNonTaskDistribution());

        for (int threads = 1; threads <= threadCount; threads++)
        {
            TestStructure(threads);
        }

        std::cout << std::endl << std::endl;
    }
}

int main()
{
    try
    {
        RunProgram();
    }
    catch (std::exception ex)
    {
        std::cout << std::endl;
        std::cout << "Error:" << ex.what();
    }
}
