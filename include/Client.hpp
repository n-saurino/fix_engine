#pragma once
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

class Client {
 public:
  Client();
  ~Client();
  int Start();

 private:
};