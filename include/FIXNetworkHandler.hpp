#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "MessageHeader.h"
#include "NewOrderSingle.h"

class FIXNetworkHandler {
 public:
  FIXNetworkHandler(/* args */);
  void Test(const char* test_type, int client_socket_fd);
  void Start();
  void SendMessage(/* Message objects*/);

 private:
  /* data */
};
