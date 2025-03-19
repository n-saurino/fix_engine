#include <benchmark/benchmark.h>

#include <chrono>
#include <sstream>
#include <string>

#include "BenchmarkLogger.hpp"
#include "LFQueueBenchmarkFormatter.hpp"

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;

  auto epoch_seconds = std::chrono::duration_cast<std::chrono::seconds>(
                           std::chrono::system_clock::now().time_since_epoch())
                           .count();

  std::stringstream promFileName;
  promFileName << "logs/benchmarks/prometheus/active/prometheus_"
               << epoch_seconds << ".prom";

  LFQueueBenchmarkFormatter lf_queue_formatter;
  BenchmarkLogger lf_queue_reporter(
      "logs/benchmarks/csv/lf_queue_results.csv", promFileName.str(),
      std::to_string(epoch_seconds), lf_queue_formatter);
  benchmark::RunSpecifiedBenchmarks(&lf_queue_reporter);
  return 0;
}
