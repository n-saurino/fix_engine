#pragma once
#include <benchmark/benchmark.h>

#include <algorithm>  // for std::max
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "MetricCalcs.hpp"

const int kTotalRuns{10};

class BenchmarkLogger : public benchmark::BenchmarkReporter {
 public:
  // Constructor opens two files: one for CSV output and one for Prometheus
  // output.
  BenchmarkLogger(const std::string& csv_filename,
                  const std::string& prom_filename)
      : log_file(csv_filename, std::ios::app),
        prom_file(prom_filename, std::ios::app) {
    if (!log_file.is_open()) {
      std::cerr << "Error: Could not open CSV log file " << csv_filename
                << "\n";
    } else {
      // If the CSV file is empty, write the CSV header.
      if (log_file.tellp() == 0) {
        log_file << "Test Name,Mean-Median Ratio,Normal Distribution,Mean "
                    "(ns),Median (ns),Std Dev (ns),Min (ns),Max "
                    "(ns),Iterations,CPU Cycles\n";
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
      LogToCSV(report[0].benchmark_name(), times, 0);
    }
  }

  // Log metrics in Prometheus exposition format.
  //   void LogToPrometheus(const benchmark::BenchmarkReporter::Run& run) {
  //     if (prom_file.is_open()) {
  //       prom_file << "# HELP fix_engine_encoding_latency_ns Time taken to
  //       build "
  //                    "a FIX message in nanoseconds.\n";
  //       prom_file << "# TYPE fix_engine_encoding_latency_ns gauge\n";
  //       prom_file << "fix_engine_encoding_latency_mean_ns " << run.<< "\n";
  //       prom_file << "fix_engine_encoding_latency_median_ns " << run.median
  //                 << "\n";
  //       prom_file << "fix_engine_encoding_latency_stddev_ns " << run.stddev
  //                 << "\n";
  //       prom_file << "fix_engine_encoding_latency_min_ns " << run.min_time
  //                 << "\n";
  //       prom_file << "fix_engine_encoding_latency_max_ns " << run.max_time
  //                 << "\n";
  //       prom_file << "fix_engine_encoding_iterations " << run.iterations <<
  //       "\n";
  //     }
  //   }

  // Log metrics in CSV format.
  void LogToCSV(const std::string& name, const std::vector<double> times,
                uint64_t cpu_cycles = 0) {
    double mean_median{CalcMeanMedian(times)};
    double mean{CalcMean(times)};
    double median{CalcMedian(times)};
    double std_dev{CalcStdDev(times, mean)};
    double max_agg{CalcMax(times)};
    double min_agg{CalcMin(times)};
    bool is_normal{IsNormal(times)};
    if (log_file.is_open()) {
      log_file << name << "," << mean_median << "," << is_normal << "," << mean
               << "," << median << "," << std_dev << "," << min_agg << ","
               << max_agg << ","
               << "," << times.size() << "," << cpu_cycles << "\n";
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
};