#pragma once

#include <functional>
#include <chrono>


class PerformanceTester
{
public:
    using TAlg = std::function<void()>;
    using TMcs = unsigned long int;

    void SetCount(int c) { count = c; }
    int GetCount() const { return count; }

    void Measure(const TAlg& alg);

    TMcs GetMeasureDuration() const { return measureRes; }

private:

    int count = 1;
    TMcs measureRes = 0;
};

