#pragma once
#include <fstream>
#include <string>

// add a logger to save benchmark testing results to a csv
class PerformanceLogger {
 public:
  explicit PerformanceLogger(const std::string& filename);
  void Log(const std::string& test_name, long long avg_rtt,
           double avg_throughput, double avg_cpu_usage);

 private:
  std::ofstream file_;
};
