#pragma once
#include "BenchmarkFormatter.hpp"

class FIXMessageBuilderBenchmarkFormatter : public BenchmarkFormatter {
 public:
  std::string GetCSVHeader() const {
    return "Test Name,Mean-Median Ratio,Normal Distribution,Mean "
           "(ns),Median (ns),Std Dev (ns),Min (ns),Max "
           "(ns),Iterations,CPU Cycles\n";
  }

  void LogToCSV(std::ofstream& log_file, const std::string& name,
                const std::vector<double>& times,
                uint64_t cpu_cycles = 0) const {
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

  void LogToPrometheus(std::ofstream& log_file, const std::string& name,
                       const std::vector<double>& times) const {
    double mean_median{CalcMeanMedian(times)};
    double mean{CalcMean(times)};
    double median{CalcMedian(times)};
    double std_dev{CalcStdDev(times, mean)};
    double max_agg{CalcMax(times)};
    double min_agg{CalcMin(times)};
    bool is_normal{IsNormal(times)};

    if (log_file.is_open()) {
      log_file
          << "# HELP fix_message_encoding_latency_mean_median_test result of "
          << "mean-median test to check normal distribution.\n";
      log_file
          << "# TYPE fix_message_encoding_latency_mean_median_test gauge\n";
      log_file << "fix_message_encoding_latency_mean_median_test{benchmark=\""
               << name << "\"} " << mean_median << "\n\n";

      log_file << "# HELP fix_message_encoding_latency_normal_distribution 1 "
                  "if normally distributed.\n";
      log_file
          << "# TYPE fix_message_encoding_latency_normal_distribution gauge\n";
      log_file << "fix_message_encoding_latency_is_normal{benchmark=\"" << name
               << "\"} " << is_normal << "\n\n";

      log_file
          << "# HELP fix_message_encoding_latency_mean_ns Mean time taken to "
          << "build a FIX message in nanoseconds.\n";
      log_file << "# TYPE fix_message_encoding_latency_mean_ns gauge\n";
      log_file << "fix_message_encoding_latency_mean_ns{benchmark=\"" << name
               << "\"} " << mean << "\n\n";

      log_file << "# HELP fix_message_encoding_latency_median_ns Median "
               << "time taken to "
               << "build a FIX message in nanoseconds.\n";
      log_file << "# TYPE fix_message_encoding_latency_median_ns gauge\n";
      log_file << "fix_message_encoding_latency_median_ns{benchmark=\"" << name
               << "\"} " << median << "\n\n";

      log_file << "# HELP fix_message_encoding_latency_std_dev_ns Std. dev of "
               << "time taken to "
               << "build a FIX message in nanoseconds.\n";
      log_file << "# TYPE fix_message_encoding_latency_std_dev_ns gauge\n";
      log_file << "fix_message_encoding_latency_std_dev_ns{benchmark=\"" << name
               << "\"} " << std_dev << "\n\n";

      log_file
          << "# HELP fix_message_encoding_latency_min_ns Min time taken to "
          << "build a FIX message in nanoseconds.\n";
      log_file << "# TYPE fix_message_encoding_latency_min_ns gauge\n";
      log_file << "fix_message_encoding_latency_min_ns{benchmark=\"" << name
               << "\"} " << min_agg << "\n\n";

      log_file
          << "# HELP fix_message_encoding_latency_max_ns Max time taken to "
          << "build a FIX message in nanoseconds.\n";
      log_file << "# TYPE fix_message_encoding_latency_max_ns gauge\n";
      log_file << "fix_message_encoding_latency_max_ns{benchmark=\"" << name
               << "\"} " << max_agg << "\n\n";

      log_file << "# HELP fix_message_encoding_latency_runs Number of "
               << "benchmarks runs composing this result.\n";
      log_file << "# TYPE fix_message_encoding_latency_run gauge\n";
      log_file << "fix_message_encoding_iterations{benchmark=\"" << name
               << "\"} " << times.size() << "\n\n";
    }
  }

 private:
  /* data */
};
