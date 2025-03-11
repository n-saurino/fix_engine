#include <benchmark/benchmark.h>

#include "BenchmarkLogger.hpp"

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;

  // FIXMessage Builder Benchmark
  BenchmarkLogger fix_message_builder_reporter(
      "logs/benchmarks/csv/fix_benchmark_results.csv",
      "logs/benchmarks/prometheus/prometheus.prom");
  benchmark::RunSpecifiedBenchmarks(&fix_message_builder_reporter);

  return 0;
}
