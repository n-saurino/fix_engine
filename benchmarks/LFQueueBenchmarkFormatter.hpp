#pragma once
#include "BenchmarkFormatter.hpp"

class LFQueueBenchmarkFormatter : public BenchmarkFormatter {
 public:
  std::string GetCSVHeader() const {
    return "Test Name,Avg Push Time (ns),Median Push Time (ns),StdDev "
           "Push (ns),Min Push (ns),Max Push (ns),"
           "Avg Pop Time (ns),Median Pop Time (ns),StdDev Pop (ns),Min "
           "Pop (ns),Max Pop (ns),Iterations,CPU Cycles";
  }

  void LogToCSV(std::ofstream& log_file, const std::string& name,
                const std::vector<double>& times,
                uint64_t cpu_cycles = 0) const {
    // We expect the times vector to be even in size: first half for push,
    // second half for pop.
    size_t n = times.size();
    if (n % 2 != 0) {
      // Handle error as needed. For now, we'll simply return.
      log_file << name << ",ERROR: Invalid number of timing samples,"
               << cpu_cycles << "\n";
      return;
    }
    size_t half = n / 2;
    std::vector<double> push_times(times.begin(), times.begin() + half);
    std::vector<double> pop_times(times.begin() + half, times.end());

    double push_mean = CalcMean(push_times);
    double push_median = CalcMedian(push_times);
    double push_std = CalcStdDev(push_times, push_mean);
    double push_min = CalcMin(push_times);
    double push_max = CalcMax(push_times);

    double pop_mean = CalcMean(pop_times);
    double pop_median = CalcMedian(pop_times);
    double pop_std = CalcStdDev(pop_times, pop_mean);
    double pop_min = CalcMin(pop_times);
    double pop_max = CalcMax(pop_times);

    log_file << name << "," << push_mean << "," << push_median << ","
             << push_std << "," << push_min << "," << push_max << ","
             << pop_mean << "," << pop_median << "," << pop_std << ","
             << pop_min << "," << pop_max << "," << n
             << ","  // total number of timing measurements
             << cpu_cycles << "\n";
  }

  void LogToPrometheus(std::ofstream& log_file, const std::string& name,
                       const std::vector<double>& times) const {
    size_t n = times.size();
    if (n % 2 != 0) {
      return;  // Optionally, log an error metric or warning.
    }
    size_t half = n / 2;
    std::vector<double> push_times(times.begin(), times.begin() + half);
    std::vector<double> pop_times(times.begin() + half, times.end());

    double push_mean = CalcMean(push_times);
    double push_median = CalcMedian(push_times);
    double push_std = CalcStdDev(push_times, push_mean);
    double push_min = CalcMin(push_times);
    double push_max = CalcMax(push_times);

    double pop_mean = CalcMean(pop_times);
    double pop_median = CalcMedian(pop_times);
    double pop_std = CalcStdDev(pop_times, pop_mean);
    double pop_min = CalcMin(pop_times);
    double pop_max = CalcMax(pop_times);

    if (log_file.is_open()) {
      // Push metrics
      log_file << "# HELP lfqueue_push_mean_ns Average push operation time in "
                  "nanoseconds.\n";
      log_file << "# TYPE lfqueue_push_mean_ns gauge\n";
      log_file << "lfqueue_push_mean_ns{benchmark=\"" << name << "\"} "
               << push_mean << "\n\n";

      log_file << "# HELP lfqueue_push_median_ns Median push operation time "
                  "in nanoseconds.\n";
      log_file << "# TYPE lfqueue_push_median_ns gauge\n";
      log_file << "lfqueue_push_median_ns{benchmark=\"" << name << "\"} "
               << push_median << "\n\n";

      log_file << "# HELP lfqueue_push_std_dev_ns Standard deviation of push "
                  "operation time in nanoseconds.\n";
      log_file << "# TYPE lfqueue_push_std_dev_ns gauge\n";
      log_file << "lfqueue_push_std_dev_ns{benchmark=\"" << name << "\"} "
               << push_std << "\n\n";

      log_file << "# HELP lfqueue_push_min_ns Minimum push operation time in "
                  "nanoseconds.\n";
      log_file << "# TYPE lfqueue_push_min_ns gauge\n";
      log_file << "lfqueue_push_min_ns{benchmark=\"" << name << "\"} "
               << push_min << "\n\n";

      log_file << "# HELP lfqueue_push_max_ns Maximum push operation time in "
                  "nanoseconds.\n";
      log_file << "# TYPE lfqueue_push_max_ns gauge\n";
      log_file << "lfqueue_push_max_ns{benchmark=\"" << name << "\"} "
               << push_max << "\n\n";

      // Pop metrics
      log_file << "# HELP lfqueue_pop_mean_ns Average pop operation time in "
                  "nanoseconds.\n";
      log_file << "# TYPE lfqueue_pop_mean_ns gauge\n";
      log_file << "lfqueue_pop_mean_ns{benchmark=\"" << name << "\"} "
               << pop_mean << "\n\n";

      log_file << "# HELP lfqueue_pop_median_ns Median pop operation time in "
                  "nanoseconds.\n";
      log_file << "# TYPE lfqueue_pop_median_ns gauge\n";
      log_file << "lfqueue_pop_median_ns{benchmark=\"" << name << "\"} "
               << pop_median << "\n\n";

      log_file << "# HELP lfqueue_pop_std_dev_ns Standard deviation of pop "
                  "operation time in nanoseconds.\n";
      log_file << "# TYPE lfqueue_pop_std_dev_ns gauge\n";
      log_file << "lfqueue_pop_std_dev_ns{benchmark=\"" << name << "\"} "
               << pop_std << "\n\n";

      log_file << "# HELP lfqueue_pop_min_ns Minimum pop operation time in "
                  "nanoseconds.\n";
      log_file << "# TYPE lfqueue_pop_min_ns gauge\n";
      log_file << "lfqueue_pop_min_ns{benchmark=\"" << name << "\"} " << pop_min
               << "\n\n";

      log_file << "# HELP lfqueue_pop_max_ns Maximum pop operation time in "
                  "nanoseconds.\n";
      log_file << "# TYPE lfqueue_pop_max_ns gauge\n";
      log_file << "lfqueue_pop_max_ns{benchmark=\"" << name << "\"} " << pop_max
               << "\n\n";

      // Count of measurements (total operations recorded)
      log_file << "# HELP lfqueue_iterations Number of push/pop measurements "
                  "collected.\n";
      log_file << "# TYPE lfqueue_iterations gauge\n";
      log_file << "lfqueue_iterations{benchmark=\"" << name << "\"} " << n
               << "\n\n";
    }
  }
};