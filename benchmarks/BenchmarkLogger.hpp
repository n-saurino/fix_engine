#pragma once
#include <benchmark/benchmark.h>

#include <algorithm>  // for std::max
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "BenchmarkFormatter.hpp"
#include "MetricCalcs.hpp"

const int kTotalRuns{10};

class BenchmarkLogger : public benchmark::BenchmarkReporter {
 public:
  // Constructor opens two files: one for CSV output and one for Prometheus
  // output.
  BenchmarkLogger(const std::string& csv_filename,
                  const std::string& prom_filename,
                  const std::string& epoch_tag,
                  const BenchmarkFormatter& formatter)
      : log_file(csv_filename, std::ios::app),
        prom_file(prom_filename, std::ios::app),
        epoch_time{epoch_tag},
        formatter_{&formatter} {
    if (!log_file.is_open()) {
      std::cerr << "Error: Could not open CSV log file " << csv_filename
                << "\n";
    } else {
      // If the CSV file is empty, write the CSV header.
      if (log_file.tellp() == 0) {
        log_file << formatter_->GetCSVHeader();
      }
    }
    if (!prom_file.is_open()) {
      std::cerr << "Error: Could not open Prometheus log file " << prom_filename
                << "\n";
    }
  }

  // Override ReportContext to log benchmark context information.
  bool ReportContext(const Context& context) override {
    // if (log_file.is_open()) {
    //     log_file << "# Benchmark Context\n";
    //     log_file << "# CPU cores: " << context.cpu_info.num_cpus << "\n";
    //     // Add any additional context info as needed.
    // }

    return true;
  }

  // Override ReportRuns to log each run's metrics.
  // Improvement: Find a more elegant way to only log the custom aggregator line
  // that you've created. Natively, Google benchmark wants to print a single run
  // line, then my custom aggregator line and then it's own standard aggregator
  // lines. I'm working around this by filtering on the total number of
  // benchmark runs that are in the report and only logging if it matches the
  // custom number I set in kTotalRuns
  void ReportRuns(const std::vector<Run>& report) override {
    std::vector<double> times{};
    for (const auto& run : report) {
      times.push_back(run.GetAdjustedRealTime());
    }
    if (times.size() == kTotalRuns) {
      // std::string name = report[0].benchmark_name() + " - " + epoch_time;
      std::string name = report[0].benchmark_name();
      formatter_->LogToCSV(log_file, name, times, 0);
      formatter_->LogToPrometheus(prom_file, name, times);
    }
  }

  ~BenchmarkLogger() override {
    if (log_file.is_open()) {
      log_file.close();
    }
    if (prom_file.is_open()) {
      prom_file.close();
    }
  }

 private:
  std::ofstream log_file;
  std::ofstream prom_file;
  std::string epoch_time;
  const BenchmarkFormatter* formatter_;
};
