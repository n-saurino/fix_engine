#include "Server.hpp"

Server::Server() { Start(); }

auto Server::Start() -> int {
  const int BUFFER_SIZE{1024};
  const int PORT_NUM{8080};
  // declare socket_address struct
  struct sockaddr_in serv_socket_address{};
  int serv_address_len{sizeof(serv_socket_address)};
  // create Server TCP socket
  int serv_socket_fd{socket(AF_INET, SOCK_STREAM, 0)};
  // REFACTOR: need to add proper exception handling
  if (serv_socket_fd < 0) {
    std::cerr << "Server socket creation error: " << std::strerror(errno)
              << "\n";
    return -1;
  }
  // create message buffer to store client messages
  std::array<char, BUFFER_SIZE> message_buffer{0};

  int opt{1};

  setsockopt(serv_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
  serv_socket_address.sin_family = AF_INET;
  serv_socket_address.sin_port = htons(PORT_NUM);
  serv_socket_address.sin_addr.s_addr = INADDR_ANY;

  // bind allows socket to listen for incoming client connections on
  // a specific address and port
  // side note: static_cast<> doesn't work because it's meant for "safe"
  // conversions
  if (bind(serv_socket_fd,
           reinterpret_cast<struct sockaddr*>(&serv_socket_address),
           serv_address_len) < 0) {
    std::cerr << "Server socket bind error: " << std::strerror(errno) << "\n";
    close(serv_socket_fd);
    return -1;
  }

  // listen sets the socket listening for client connections
  // 10 is the number of connections that can be queued while the server
  // is busy
  const int QUEUED_SOCKET_NUM{10};
  if (listen(serv_socket_fd, QUEUED_SOCKET_NUM) < 0) {
    std::cerr << "Server socket listen error: " << std::strerror(errno) << "\n";
    close(serv_socket_fd);
    return -1;
  }

  // accept a client connection on the socket
  int client_socket_fd{accept(
      serv_socket_fd, reinterpret_cast<struct sockaddr*>(&serv_socket_address),
      reinterpret_cast<socklen_t*>(&serv_address_len))};

  if (client_socket_fd < 0) {
    std::cerr << "Client socket accept error: " << std::strerror(errno) << "\n";
    close(serv_socket_fd);
    return -1;
  }
  // simple benchmarking tools
  profiler_.Benchmark(client_socket_fd);

  close(client_socket_fd);
  close(serv_socket_fd);
  return 0;
}

auto Server::Stop() -> int { return 0; }