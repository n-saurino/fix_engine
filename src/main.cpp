#include "Server.hpp"
#include "Client.hpp"
#include <chrono>
#include <thread>

void RunServer(){
    Server server{};
}

void RunClient(){
    Client client{};
}

int main(int, char**){
    std::thread server_thread{RunServer};
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::thread client_thread{RunClient};
    client_thread.join();
    
    server_thread.join();

    return 0;
}
