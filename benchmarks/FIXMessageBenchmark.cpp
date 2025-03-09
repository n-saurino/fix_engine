#include <benchmark/benchmark.h>
#include <cstring>
#include <vector>
#include "FIXMessage.hpp"
#include "MemoryPool.hpp"
#include "FIXNewOrderSingle.hpp"
#include "SampleOrders.hpp"

class FIXMessageBenchmark : public benchmark::Fixture {
protected:
    MemoryPool pool;
    SampleNewOrder order;

public:
    void SetUp(const ::benchmark::State& state) override {
        // Set up any necessary data before the benchmark
        // The pool and order are already class members and constructed before each run.
    }

    void TearDown(const ::benchmark::State& state) override {
        // Cleanup if necessary (not needed here since MemoryPool manages itself)
    }
};

// Register the benchmark with the fixture
BENCHMARK_F(FIXMessageBenchmark, Benchmark_FIXMessageBuild)(benchmark::State& state) {
    for (auto _ : state) {
        FIXBuffer buffer(pool.Allocate());  // Simulate real allocation on hot path
        FIXNewOrderSingle new_order_single(buffer, order);
        benchmark::DoNotOptimize(buffer.Data());  // Prevent compiler optimizations
        pool.Free(buffer.Data());  // Return buffer to the pool for reuse
    }
}

BENCHMARK_MAIN();