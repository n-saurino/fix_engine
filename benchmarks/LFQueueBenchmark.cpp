#include <benchmark/benchmark.h>

#include <cstring>
#include <vector>

#include "BenchmarkLogger.hpp"
#include "LFQueue.hpp"
#include "Macros.hpp"

class LFQueueBenchmark : public benchmark::Fixture {
 protected:
  LFQueue<int> queue;

 public:
  LFQueueBenchmark() : queue(1024) {}

  void SetUp(const ::benchmark::State& state) override {
    // Set up any necessary data before the benchmark
    // The pool and order are already class members and constructed before each
    // run.
  }

  void TearDown(const ::benchmark::State& state) override {
    // Cleanup if necessary (not needed here since MemoryPool manages itself)
  }
};

// This benchmark repeatedly pushes and then pops an element from the LFQueue.
BENCHMARK_F(LFQueueBenchmark, Benchmark_LFQueuePushPop)
(benchmark::State& state) {
  int pushValue = 42;
  int popValue = 0;
  for (auto _ : state) {
    // Push an element onto the queue.
    bool pushSuccess = queue.Push(pushValue);
    benchmark::DoNotOptimize(pushSuccess);

    // Immediately pop the element.
    bool popSuccess = queue.Pop(popValue);
    benchmark::DoNotOptimize(popSuccess);

    // Prevent compiler optimizations.
    benchmark::ClobberMemory();
  }
}

BENCHMARK_REGISTER_F(LFQueueBenchmark, Benchmark_LFQueuePushPop)
    ->Repetitions(kTotalRuns);  // or add option --benchmark_repetitions=3

// BENCHMARK_MAIN();
