#pragma once
#include "PerformanceLogger.hpp"

class NetworkProfiler
{
public:
    NetworkProfiler(/* args */);
    ~NetworkProfiler();
    void Benchmark(int client_socket_fd);
    void BenchmarkLatency(int client_socket_fd);
    void BenchmarkThroughput(int client_socket_fd);

private:
    PerformanceLogger logger_{};    
};

