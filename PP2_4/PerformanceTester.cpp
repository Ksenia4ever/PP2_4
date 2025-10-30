#include "PerformanceTester.h"

void PerformanceTester::Measure(const TAlg& alg)
{
    const auto clock = std::chrono::high_resolution_clock();
    auto avgDuration = TMcs(0);

    for (int i = 0; i < GetCount(); i++)
    {
        const auto start = clock.now();

        alg();

        const auto end = clock.now();
        const auto duration = end - start;

        avgDuration += static_cast<TMcs>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count());
    }

    avgDuration /= GetCount();
    measureRes = avgDuration;
}