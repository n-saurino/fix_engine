#include "PerformanceLogger.hpp"

PerformanceLogger::PerformanceLogger(const std::string& filename)
    : file_{filename} {}

void PerformanceLogger::Log(const std::string& test_name, long long avg_rtt,
                            double avg_throughput, double avg_cpu_usage) {}