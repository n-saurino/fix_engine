#include "FIXNetworkHandler.hpp"

FIXNetworkHandler::FIXNetworkHandler(/* args */) { Start(); }

FIXNetworkHandler::~FIXNetworkHandler() {}

void FIXNetworkHandler::Test(const char* test_type,
                             const int client_socket_fd) {
  char in_buffer[1024] = {};
  const char* out_buffer{"PONG"};
  const char* exit_signal{"EXIT"};
  // initiate latency test
  int nbytes_s{send(client_socket_fd, test_type, strlen(test_type), 0)};

  int nbytes_r{};
  while ((nbytes_r = recv(client_socket_fd, in_buffer, sizeof(in_buffer), 0)) >
         0) {
    if (!strcmp(exit_signal, in_buffer)) {
      return;
    }
    send(client_socket_fd, out_buffer, strlen(out_buffer), 0);
  }
}

void FIXNetworkHandler::Start() {
  struct sockaddr_in client_socket_address;
  int client_address_len{sizeof(client_socket_address)};
  int client_socket_fd{socket(AF_INET, SOCK_STREAM, 0)};
  const char* latency_test{"LATENCY_TEST"};
  const char* throughput_test{"THROUGHPUT_TEST"};
  const char* out_buffer{"PONG\n"};
  char in_buffer[1024] = {};
  client_socket_address.sin_family = AF_INET;
  client_socket_address.sin_port = htons(8080);

  inet_pton(AF_INET, "127.0.0.1", &client_socket_address.sin_addr);

  connect(client_socket_fd,
          reinterpret_cast<struct sockaddr*>(&client_socket_address),
          static_cast<socklen_t>(client_address_len));

  Test(latency_test, client_socket_fd);
  Test(throughput_test, client_socket_fd);
  Test("EXIT", client_socket_fd);

  close(client_socket_fd);
}
