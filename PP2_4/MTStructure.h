#pragma once

#include <mutex>
#include <shared_mutex>
#include <string>

struct MTStructure
{
public:
    void SetField(int fieldIndex, int fieldValue);
    int GetField(int fieldIndex) const;

    operator std::string() const;

private: 
    int filed1 = 0;
    int filed2 = 0;
    int filed3 = 0;

    mutable std::mutex field1mt;
    mutable std::shared_mutex field2mt;
    mutable std::mutex field3mt;
};

