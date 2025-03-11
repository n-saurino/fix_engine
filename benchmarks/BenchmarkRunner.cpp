#include <benchmark/benchmark.h>

#include <chrono>
#include <sstream>
#include <string>

#include "BenchmarkLogger.hpp"

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;

  auto epoch_seconds = std::chrono::duration_cast<std::chrono::seconds>(
                           std::chrono::system_clock::now().time_since_epoch())
                           .count();

  std::stringstream promFileName;
  promFileName << "logs/benchmarks/prometheus/prometheus_" << epoch_seconds
               << ".prom";

  // FIXMessage Builder Benchmark
  BenchmarkLogger fix_message_builder_reporter(
      "logs/benchmarks/csv/fix_benchmark_results.csv", promFileName.str(),
      std::to_string(epoch_seconds));
  benchmark::RunSpecifiedBenchmarks(&fix_message_builder_reporter);

  return 0;
}
