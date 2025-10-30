// Visual Studio 2022
#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <format>
#include <thread>
#include <list>
#include <stdexcept>

#include "OperationsGenerator.h"
#include "OperationsSerializer.h"
#include "MTStructure.h"

void PrepareFiles(int fileCount)
{
    auto generator = OperationsGenerator(OperationsGenerator::GetTaskDistribution());
    generator.SetCount(1000000);

    for (int fileIndex = 0; fileIndex < fileCount; fileIndex++)
    {
        const auto ops = generator();

        auto serializer = OperationsSerializer(std::format("file{}.txt", fileIndex));
        serializer.Write(ops);
    }

}

void TestStructure(int threadCount)
{
    MTStructure mts;

    auto threads = std::list<std::thread>();

    for (int threadIndex = 0; threadIndex < threadCount; threadIndex++)
    {
        auto th = std::thread([=, &mts]() 
        {
            auto serializer = OperationsSerializer(std::format("file{}.txt", threadIndex));
            auto ops = serializer.Read();

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

    PrepareFiles(threadCount);

    for (int threads = 1; threads < threadCount; threads++)
    {
        TestStructure(threads);
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
