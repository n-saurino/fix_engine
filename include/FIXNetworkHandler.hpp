#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

class FIXNetworkHandler {
 public:
  FIXNetworkHandler(/* args */);
  ~FIXNetworkHandler();
  void Test(const char* test_type, const int client_socket_fd);
  void Start();

 private:
  /* data */
};
