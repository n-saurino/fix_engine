#include "NetworkProfiler.hpp"

#include <sys/socket.h>

#include <algorithm>
#include <chrono>
#include <cstring>
#include <iostream>
#include <numeric>
#include <vector>

NetworkProfiler::NetworkProfiler(/* args */) : logger_{""} {}

NetworkProfiler::~NetworkProfiler() {}

void NetworkProfiler::Benchmark(int client_socket_fd) {
  char in_buffer[1024] = {};
  int nbytes_r{};
  while ((nbytes_r = recv(client_socket_fd, in_buffer, sizeof(in_buffer) - 1,
                          0)) > 0) {
    if (nbytes_r > 0) {
      in_buffer[nbytes_r] = '\0';  // Ensure null termination
    }

    if (!strcmp(in_buffer, "EXIT")) {
      std::cout << "Received EXIT signal. Stopping benchmark.\n";
      break;
    }

    std::cout << "\nRunning: " << in_buffer << "\n";
    if (!strcmp(in_buffer, "LATENCY_TEST")) {
      BenchmarkLatency(client_socket_fd);
    } else if (!strcmp(in_buffer, "THROUGHPUT_TEST")) {
      BenchmarkThroughput(client_socket_fd);
    }
  }
}

void NetworkProfiler::BenchmarkLatency(int client_socket_fd) {
  std::vector<long long> latencies{};
  int num_events{100000};
  latencies.reserve(num_events);
  const char* out_buffer{"PING"};
  const char* exit_signal{"EXIT"};

  for (int i{}; i < num_events; ++i) {
    char in_buffer[1024] = {};
    const auto start{std::chrono::high_resolution_clock::now()};
    // send and then receive from client
    send(client_socket_fd, out_buffer, strlen(out_buffer), 0);
    recv(client_socket_fd, in_buffer, sizeof(in_buffer), 0);
    const auto end{std::chrono::high_resolution_clock::now()};
    const auto diff{
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start)
            .count()};
    latencies.push_back(diff);
  }

  send(client_socket_fd, exit_signal, strlen(exit_signal), 0);

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
  int duration{5};
  const char* out_buffer{"PONG"};
  const char* exit_signal{"EXIT"};
  const auto start{std::chrono::high_resolution_clock::now()};
  const auto end{start + std::chrono::seconds(5)};
  double total_messages{};

  while (std::chrono::high_resolution_clock::now() < end) {
    char in_buffer[1024] = {};
    send(client_socket_fd, out_buffer, strlen(out_buffer), 0);
    recv(client_socket_fd, in_buffer, sizeof(in_buffer), 0);
    ++total_messages;
  }

  send(client_socket_fd, exit_signal, strlen(exit_signal), 0);

  std::cout << "Average client throughput: " << total_messages / duration
            << " round-trip-transmissions\n";
}
