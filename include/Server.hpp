#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "NetworkProfiler.hpp"

class Server{
public:
    Server();
    ~Server();
    int Start();
    int Stop();

private:
    NetworkProfiler profiler_{};
};