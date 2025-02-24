#include "FIXNetworkHandler.hpp"

#include <array>
#include <string_view>

FIXNetworkHandler::FIXNetworkHandler(/* args */) { Start(); }

void FIXNetworkHandler::Test(const char* test_type,
                             const int client_socket_fd) {
  const int BUFFER_SIZE{1024};
  std::array<char, BUFFER_SIZE> in_buffer{};
  const char* out_buffer{"PONG"};
  const char* exit_signal{"EXIT"};
  // initiate latency test
  int nbytes_s =
      static_cast<int>(send(client_socket_fd, test_type, strlen(test_type), 0));

  int nbytes_r{};
  while ((nbytes_r = static_cast<int>(
              recv(client_socket_fd, &in_buffer, sizeof(in_buffer), 0))) > 0) {
    if (exit_signal == std::string_view(in_buffer.data())) {
      return;
    }
    send(client_socket_fd, out_buffer, strlen(out_buffer), 0);
  }
}

void FIXNetworkHandler::Start() {
  const int BUFFER_SIZE{1024};
  const int PORT_NUM{8080};
  struct sockaddr_in client_socket_address{};
  int client_address_len{sizeof(client_socket_address)};
  int client_socket_fd{socket(AF_INET, SOCK_STREAM, 0)};
  const char* latency_test{"LATENCY_TEST"};
  const char* throughput_test{"THROUGHPUT_TEST"};
  const char* out_buffer{"PONG\n"};
  std::array<char, BUFFER_SIZE> in_buffer{};
  client_socket_address.sin_family = AF_INET;
  client_socket_address.sin_port = htons(PORT_NUM);

  inet_pton(AF_INET, "127.0.0.1", &client_socket_address.sin_addr);

  int connect_result{};

  // choosing to violate the C++ guidelines and use reinterpret cast
  // because this is a well-defined conversion in the POSIX socket API
  connect_result =
      connect(client_socket_fd,
              reinterpret_cast<struct sockaddr*>(&client_socket_address),
              static_cast<socklen_t>(client_address_len));
  if (connect_result == -1) {
    std::cerr << "Client's network handler failed to connect: "
              << std::strerror(errno) << "\n";
    close(client_socket_fd);
    return;
  }

  Test(latency_test, client_socket_fd);
  Test(throughput_test, client_socket_fd);
  Test("EXIT", client_socket_fd);

  close(client_socket_fd);
}

// Improvement: Consider overloading or templating for different message
// objects?
void FIXNetworkHandler::SendMessage(/* Message Object */) {}
