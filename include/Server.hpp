#pragma once
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <array>
#include <cstring>
#include <iostream>
#include <sstream>

#include "NetworkProfiler.hpp"

class Server {
 public:
  Server();
  auto Start() -> int;
  auto Stop() -> int;

 private:
  NetworkProfiler profiler_;
};