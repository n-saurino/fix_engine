cmake_minimum_required(VERSION 3.16)
project(FIXBenchmarks)

set(CMAKE_CXX_STANDARD 17)

find_package(benchmark REQUIRED)

add_executable(FIXMessageBenchmark FIXMessageBenchmark.cpp BenchmarkRunner.cpp)
target_include_directories(FIXMessageBenchmark PRIVATE ../include)
target_link_libraries(FIXMessageBenchmark PRIVATE benchmark::benchmark)
