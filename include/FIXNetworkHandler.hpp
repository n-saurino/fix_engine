#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cstring>
#include <iostream>

#include "FIXHeartbeat.hpp"
#include "FIXLogon.hpp"
#include "FIXLogout.hpp"
#include "FIXMessageParser.hpp"
#include "FIXNewOrderSingle.hpp"
#include "FIXOrderCancelReplaceRequest.hpp"
#include "FIXOrderCancelRequest.hpp"
#include "FIXTestRequest.hpp"

class FIXNetworkHandler {
 public:
  FIXNetworkHandler(/* args */);
  void Test(const char* test_type, int client_socket_fd);
  void Start();
  void Stop();
  static void Send(FIXBuffer& buffer, const int client_socket_fd);
  static void SendMessage(/* Message, */ const int client_socket_fd);
  static void ReceiveMessage(char (&buffer)[1024], const int client_socket_fd);
  auto GetSocket(const int) const -> const int;

 private:
  static const int kMaxSockets{10};
  std::array<int, kMaxSockets> sockets_{};
  int socket_idx_{0};
};
