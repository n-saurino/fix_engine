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
                  const std::string& prom_filename,
                  const std::string& epoch_tag)
      : log_file(csv_filename, std::ios::app),
        prom_file(prom_filename, std::ios::app),
        epoch_time{epoch_tag} {
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
      // std::string name = report[0].benchmark_name() + " - " + epoch_time;
      std::string name = report[0].benchmark_name();
      LogToCSV(name, times, 0);
      LogToPrometheus(name, times);
    }
  }

  // Log metrics in Prometheus exposition format
  void LogToPrometheus(const std::string& name,
                       const std::vector<double>& times) {
    double mean_median{CalcMeanMedian(times)};
    double mean{CalcMean(times)};
    double median{CalcMedian(times)};
    double std_dev{CalcStdDev(times, mean)};
    double max_agg{CalcMax(times)};
    double min_agg{CalcMin(times)};
    bool is_normal{IsNormal(times)};

    if (prom_file.is_open()) {
      prom_file
          << "# HELP fix_message_encoding_latency_mean_median_test result of "
          << "mean-median test to check normal distribution.\n";
      prom_file
          << "# TYPE fix_message_encoding_latency_mean_median_test gauge\n";
      prom_file << "fix_message_encoding_latency_mean_median_test{benchmark=\""
                << name << "\"} " << mean_median << "\n\n";

      prom_file << "# HELP fix_message_encoding_latency_normal_distribution 1 "
                   "if normally distributed.\n";
      prom_file
          << "# TYPE fix_message_encoding_latency_normal_distribution gauge\n";
      prom_file << "fix_message_encoding_latency_is_normal{benchmark=\"" << name
                << "\"} " << is_normal << "\n\n";

      prom_file
          << "# HELP fix_message_encoding_latency_mean_ns Mean time taken to "
          << "build a FIX message in nanoseconds.\n";
      prom_file << "# TYPE fix_message_encoding_latency_mean_ns gauge\n";
      prom_file << "fix_message_encoding_latency_mean_ns{benchmark=\"" << name
                << "\"} " << mean << "\n\n";

      prom_file << "# HELP fix_message_encoding_latency_median_ns Median "
                << "time taken to "
                << "build a FIX message in nanoseconds.\n";
      prom_file << "# TYPE fix_message_encoding_latency_median_ns gauge\n";
      prom_file << "fix_message_encoding_latency_median_ns{benchmark=\"" << name
                << "\"} " << median << "\n\n";

      prom_file << "# HELP fix_message_encoding_latency_std_dev_ns Std. dev of "
                << "time taken to "
                << "build a FIX message in nanoseconds.\n";
      prom_file << "# TYPE fix_message_encoding_latency_std_dev_ns gauge\n";
      prom_file << "fix_message_encoding_latency_std_dev_ns{benchmark=\""
                << name << "\"} " << std_dev << "\n\n";

      prom_file
          << "# HELP fix_message_encoding_latency_min_ns Min time taken to "
          << "build a FIX message in nanoseconds.\n";
      prom_file << "# TYPE fix_message_encoding_latency_min_ns gauge\n";
      prom_file << "fix_message_encoding_latency_min_ns{benchmark=\"" << name
                << "\"} " << min_agg << "\n\n";

      prom_file
          << "# HELP fix_message_encoding_latency_max_ns Max time taken to "
          << "build a FIX message in nanoseconds.\n";
      prom_file << "# TYPE fix_message_encoding_latency_max_ns gauge\n";
      prom_file << "fix_message_encoding_latency_max_ns{benchmark=\"" << name
                << "\"} " << max_agg << "\n\n";

      prom_file << "# HELP fix_message_encoding_latency_runs Number of "
                << "benchmarks runs composing this result.\n";
      prom_file << "# TYPE fix_message_encoding_latency_run gauge\n";
      prom_file << "fix_message_encoding_iterations{benchmark=\"" << name
                << "\"} " << times.size() << "\n\n";
    }
  }

  // Log metrics in CSV format.
  void LogToCSV(const std::string& name, const std::vector<double>& times,
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
               << max_agg << "," << times.size() << "," << cpu_cycles << "\n";
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
};
