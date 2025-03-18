cmake_minimum_required(VERSION 3.16)
project(FIXBenchmarks)

set(CMAKE_CXX_STANDARD 17)

find_package(benchmark REQUIRED)

add_executable(Benchmarks FIXMessageBenchmark.cpp BenchmarkRunner.cpp LFQueueBenchmark.cpp)
target_include_directories(Benchmarks PRIVATE ../include)
target_link_libraries(Benchmarks PRIVATE benchmark::benchmark)
