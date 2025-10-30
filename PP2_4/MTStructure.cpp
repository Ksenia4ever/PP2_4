#include <stdexcept>
#include <format>

#include "MTStructure.h"

void MTStructure::SetField(int fileIndex, int fieldValue)
{
    switch (fileIndex)
    {
    case 0:
    {
        std::unique_lock<std::mutex> lock(field1mt);
        filed1 = fieldValue;
        break;
    }

    case 1:
    {
        std::unique_lock<std::shared_mutex> lock(field2mt);
        filed2 = fieldValue;
        break;
    }

    case 2:
    {
        std::unique_lock<std::mutex> lock(field3mt);
        filed3 = fieldValue;
        break;
    }

    default:
        throw std::invalid_argument("Invalid field index");
        break;
    }
}

int MTStructure::GetField(int fileIndex) const
{
    int value = 0;

    switch (fileIndex)
    {
    case 0:
    {
        std::unique_lock<std::mutex> lock(field1mt);
        value = filed1;
        break;
    }

    case 1:
    {
        std::shared_lock<std::shared_mutex> lock(field2mt);
        value = filed2;
        break;
    }

    case 2:
    {
        std::unique_lock<std::mutex> lock(field3mt);
        value = filed3;
        break;
    }

    default:
        throw std::invalid_argument("Invalid field index");
        break;
    }

    return value;
}

MTStructure::operator std::string() const
{
    std::unique_lock<std::mutex> lock1(field1mt);
    std::shared_lock<std::shared_mutex> lock2(field2mt);
    std::unique_lock<std::mutex> lock3(field3mt);

    std::string str = std::format("{} {} {}", filed1, filed2, filed3);
    return str;
}