#include "Server.hpp"
#include "FIXEngine.hpp"
#include <chrono>
#include <thread>

void RunServer(){
    Server server{};
}

void RunFIXEngine(){
    FIXEngine fix_engine{};
}

int main(int, char**){
    std::thread server_thread{RunServer};
    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::thread client_thread{RunFIXEngine};
    client_thread.join();
    
    server_thread.join();

    return 0;
}
