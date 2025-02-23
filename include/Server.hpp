#pragma once
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "NetworkProfiler.hpp"

class Server {
 public:
  Server();
  ~Server();
  int Start();
  int Stop();

 private:
  NetworkProfiler profiler_{};
};