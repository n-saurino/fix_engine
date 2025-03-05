#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "FIXMessageBuilder.hpp"
#include "FIXMessageParser.hpp"

class FIXNetworkHandler {
 public:
  FIXNetworkHandler(/* args */);
  void Test(const char* test_type, int client_socket_fd);
  void Start();
  static void SendLogon(/* Message, */ const int client_socket_fd);
  static void SendMessage(/* Message, */ const int client_socket_fd);
  static void ReceiveMessage(const int client_socket_fd);

 private:
  /* data */
};
