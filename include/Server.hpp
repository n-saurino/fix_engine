#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

class Server{
public:
    Server();
    ~Server();
    int Start();
    int Stop();

private:

};