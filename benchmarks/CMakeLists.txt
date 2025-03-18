cmake_minimum_required(VERSION 3.16)
project(FIXBenchmarks)

set(CMAKE_CXX_STANDARD 20)

find_package(benchmark REQUIRED)

add_executable(FIXEncoderBenchmark FIXMessageBenchmark.cpp main_FIXMessageBenchmark.cpp)
target_include_directories(FIXEncoderBenchmark PRIVATE ../include)
target_link_libraries(FIXEncoderBenchmark PRIVATE benchmark::benchmark)

add_executable(LFQueueBenchmark LFQueueBenchmark.cpp main_LFQueueBenchmark.cpp)
target_include_directories(LFQueueBenchmark PRIVATE ../include)
target_link_libraries(LFQueueBenchmark PRIVATE benchmark::benchmark)
