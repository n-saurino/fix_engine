#pragma once
#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

// Compute mean from a vector of times.
inline double CalcMean(const std::vector<double>& times) {
  return std::accumulate(times.begin(), times.end(), 0.0) / times.size();
}

// Compute median from a vector of times.
inline double CalcMedian(std::vector<double> times) {
  std::sort(times.begin(), times.end());
  size_t mid = times.size() / 2;
  return (times.size() % 2 == 0) ? (times[mid - 1] + times[mid]) / 2.0
                                 : times[mid];
}

// Compute standard deviation from a vector of times.
inline double CalcStdDev(const std::vector<double>& times, double mean) {
  double sum = 0.0;
  for (double t : times) {
    sum += (t - mean) * (t - mean);
  }
  return std::sqrt(sum / times.size());
}

// Add a max calc
inline double CalcMax(const std::vector<double>& times) {
  return *std::max_element(times.begin(), times.end());
}

// Add a min calc
inline double CalcMin(const std::vector<double>& times) {
  return *std::min_element(times.begin(), times.end());
}

// Compute the relative difference between mean and median.
inline double CalcMeanMedian(const std::vector<double>& times) {
  double mean = CalcMean(times);
  double median = CalcMedian(times);
  double max_val = std::max(CalcMean(times), CalcMedian(times));
  return (max_val != 0) ? std::abs(mean - median) / max_val : 0;
}

// Check if the distribution is nearly normal.
inline bool IsNormal(const std::vector<double>& times) {
  return CalcMeanMedian(times) < 0.01;
}
