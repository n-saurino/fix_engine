#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "MetricCalcs.hpp"

class BenchmarkFormatter {
 public:
  virtual ~BenchmarkFormatter() = default;

  virtual std::string GetCSVHeader() const = 0;

  //   pure virtual function to log data to csv
  virtual void LogToCSV(std::ofstream& log_file, const std::string& name,
                        const std::vector<double>& times,
                        uint64_t cpu_cycles = 0) const = 0;

  // pure virtual funciton to log data to prometheus
  virtual void LogToPrometheus(std::ofstream& log_file, const std::string& name,
                               const std::vector<double>& times) const = 0;
};