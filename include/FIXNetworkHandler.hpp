#pragma once
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

class FIXNetworkHandler
{
public:
    FIXNetworkHandler(/* args */);
    ~FIXNetworkHandler();
    void Start();
private:
    /* data */
};

