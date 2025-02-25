#include "NetworkProfiler.hpp"

#include <sys/socket.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <numeric>
#include <string_view>
#include <vector>

NetworkProfiler::NetworkProfiler(/* args */) : logger_{""} {}

void NetworkProfiler::Benchmark(int client_socket_fd) {
  const int BUFFER_SIZE{1024};
  std::array<char, BUFFER_SIZE> in_buffer{};
  int nbytes_r{};
  while ((nbytes_r = static_cast<int>(recv(client_socket_fd, &in_buffer,
                                           sizeof(in_buffer) - 1, 0))) > 0) {
    if (nbytes_r > 0) {
      in_buffer.at(nbytes_r) = '\0';  // Ensure null termination
    }

    if (nbytes_r < 0) {
      std::cerr << "Client socket recv error in Benchmark(): "
                << std::strerror(errno) << "\n";
      return;
    }

    if (std::string_view(in_buffer.data()) == "EXIT") {
      std::cout << "Received EXIT signal. Stopping benchmark.\n";
      break;
    }

    std::cout << "\nRunning: " << in_buffer.data() << "\n";
    std::string_view in_buffer_sv = in_buffer.data();
    if (in_buffer_sv == "LATENCY_TEST") {
      BenchmarkLatency(client_socket_fd);
    } else if (in_buffer_sv == "THROUGHPUT_TEST") {
      BenchmarkThroughput(client_socket_fd);
    }
  }
}

void NetworkProfiler::BenchmarkLatency(int client_socket_fd) {
  std::vector<long long> latencies{};
  const int num_events{100000};
  latencies.reserve(num_events);
  const char* out_buffer{"PING"};
  const char* exit_signal{"EXIT"};
  int nbytes_r{};
  int nbytes_s{};

  for (int i{}; i < num_events; ++i) {
    const int BUFFER_SIZE{1024};
    std::array<char, BUFFER_SIZE> in_buffer{};

    // IMPROVEMENT: Read about rdtsc() for minimum latency and cycle-accurate
    // timing
    const auto start{std::chrono::steady_clock::now()};
    // send and then receive from client
    nbytes_s = static_cast<int>(
        send(client_socket_fd, out_buffer, strlen(out_buffer), 0));
    if (nbytes_s < 0) {
      std::cerr << "Client socket send error in BenchmarkLatency(): "
                << std::strerror(errno) << "\n";
      return;
    }

    nbytes_r = static_cast<int>(
        recv(client_socket_fd, in_buffer.data(), sizeof(in_buffer), 0));
    // ensuring null termination in case of buffer overflow
    if (nbytes_r > 0) {
      if (nbytes_r == in_buffer.size()) {
        in_buffer.at(in_buffer.size() - 1) = '\0';
      } else {
        in_buffer.at(nbytes_r) = '\0';
      }
    }

    if (nbytes_r < 0) {
      std::cerr << "Client socket recv error in BenchmarkLatency(): "
                << std::strerror(errno) << "\n";
      return;
    }

    const auto end{std::chrono::steady_clock::now()};
    const auto diff{
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count()};
    latencies.push_back(diff);
  }

  nbytes_s = send(client_socket_fd, exit_signal, strlen(exit_signal), 0);
  if (nbytes_s < 0) {
    std::cerr << "Client socket send error in BenchmarkLatency() when sending "
                 "exit signal: "
              << std::strerror(errno) << "\n";
  }

  std::cout << "Maximum latency: "
            << *std::max_element(latencies.begin(), latencies.end()) << "\n";

  std::cout << "Minimum latency: "
            << *std::min_element(latencies.begin(), latencies.end()) << "\n";
  long long total_latency{0};

  for (const auto latency : latencies) {
    total_latency += latency;
  }
  std::cout << "Average client round-trip-time: " << total_latency / num_events
            << " nanoseconds\n";
}

void NetworkProfiler::BenchmarkThroughput(int client_socket_fd) {
  const int duration{5};
  const char* out_buffer{"PONG"};
  const char* exit_signal{"EXIT"};
  const auto start{std::chrono::steady_clock::now()};
  const auto end{start + std::chrono::seconds(5)};
  double total_messages{};
  const int BUFFER_SIZE{1024};
  int nbytes_r{};
  int nbytes_s{};

  while (std::chrono::steady_clock::now() < end) {
    std::array<char, BUFFER_SIZE> in_buffer{};
    nbytes_s = send(client_socket_fd, out_buffer, strlen(out_buffer), 0);
    if (nbytes_s < 0) {
      std::cerr << "Client socket send error in BenchmarkThroughput(): "
                << std::strerror(errno) << "\n";
    }
    nbytes_r = recv(client_socket_fd, &in_buffer, sizeof(in_buffer), 0);
    // ensuring null termination in case of buffer overflow
    if (nbytes_r > 0) {
      if (nbytes_r == in_buffer.size()) {
        in_buffer.at(in_buffer.size() - 1) = '\0';
      } else {
        in_buffer.at(nbytes_r) = '\0';
      }
    }
    if (nbytes_r < 0) {
      std::cerr << "Client socket recv error in BenchmarkThroughput(): "
                << std::strerror(errno) << "\n";
      return;
    }

    ++total_messages;
  }

  nbytes_s = send(client_socket_fd, exit_signal, strlen(exit_signal), 0);
  if (nbytes_s < 0) {
    std::cerr << "Client socket send error in BenchmarkThroughput() when "
                 "sending exit signal: "
              << std::strerror(errno) << "\n";
  }

  std::cout << "Average client throughput: " << total_messages / duration
            << " round-trip-transmissions\n";
}
