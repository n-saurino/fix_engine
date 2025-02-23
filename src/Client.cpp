#include "Client.hpp"

Client::Client() { Start(); }

Client::~Client() {}

int Client::Start() {
  struct sockaddr_in client_socket_address;
  int client_address_len{sizeof(client_socket_address)};
  int client_socket_fd{socket(AF_INET, SOCK_STREAM, 0)};
  const char* hello{"Hello, world\n"};

  client_socket_address.sin_family = AF_INET;
  client_socket_address.sin_port = htons(8080);

  inet_pton(AF_INET, "127.0.0.1", &client_socket_address.sin_addr);

  connect(client_socket_fd,
          reinterpret_cast<struct sockaddr*>(&client_socket_address),
          static_cast<socklen_t>(client_address_len));

  // sending message to server
  send(client_socket_fd, hello, strlen(hello), 0);
  std::cout << "Message sent from client!\n";

  close(client_socket_fd);

  return 0;
}