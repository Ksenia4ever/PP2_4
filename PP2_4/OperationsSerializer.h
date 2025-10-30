#pragma once

#include <string>
#include <unordered_map>

#include "Operation.h"

class OperationsSerializer
{
public:
    OperationsSerializer(std::string filePath);

    void Write(const TOperations& operations);
    TOperations Read();

private:

    static std::unordered_map<Operation::Action, std::string> operationCodes;

    std::string filePath;
};

