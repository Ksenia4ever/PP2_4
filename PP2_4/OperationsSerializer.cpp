#include <fstream>
#include <sstream>

#include "OperationsSerializer.h"

std::unordered_map<Operation::Action, std::string> OperationsSerializer::operationCodes
{
    {Operation::Action::Write, "write"},
    {Operation::Action::Read, "read"},
    {Operation::Action::String, "string"},
};


OperationsSerializer::OperationsSerializer(std::string filePath)
    : filePath(filePath)
{
}

void OperationsSerializer::Write(const TOperations& operations)
{
    std::ofstream file(filePath);
    if (!file.is_open())
    {
        throw std::invalid_argument("File \"" + filePath + "\" is not opened.");
    }

    for (const auto& op : operations)
    {
        file << operationCodes[op.Do];
        switch (op.Do)
        {
        case Operation::Action::Write:
            file << " " << op.FieldIndex << " " << op.FieldValue;
            break;
        case Operation::Action::Read:
            file << " " << op.FieldIndex;
            break;
        case Operation::Action::String:
            break;
        default:
            throw std::runtime_error("Invalid operation.");
        }

        file << std::endl;
    }
}

TOperations OperationsSerializer::Read()
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        throw std::invalid_argument("File \"" + filePath + "\" is not opened.");
    }

    TOperations res;

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);

        std::string operationCode;
        iss >> operationCode;

        const auto itOperation = std::find_if(operationCodes.begin(),
                                              operationCodes.end(),
                                              [&](auto kvp) { return operationCode == kvp.second; });
        if (itOperation == operationCodes.end())
        {
            throw std::runtime_error("Unknown operation.");
        }

        auto op = Operation{itOperation->first};

        switch (op.Do)
        {
        case Operation::Action::Write:
            iss >> op.FieldIndex;
            iss >> op.FieldValue;
            break;
        case Operation::Action::Read:
            iss >> op.FieldIndex;
            break;
        case Operation::Action::String:
            break;
        }

        res.push_back(std::move(op));
    }

    return res;
}
