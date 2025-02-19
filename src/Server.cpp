#include "Server.hpp"

Server::Server(){
    Start();
}

Server::~Server(){

}

int Server::Start(){
    // declare socket_address struct
    struct sockaddr_in serv_socket_address{};
    int serv_address_len{sizeof(serv_socket_address)};
    // create Server TCP socket
    int serv_socket_fd{socket(AF_INET, SOCK_STREAM, 0)}; 
    // create message buffer to store client messages
    char message_buffer[1024] = {0};

    int opt{1};

    setsockopt(serv_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    serv_socket_address.sin_family = AF_INET;
    serv_socket_address.sin_port = htons(8080);
    serv_socket_address.sin_addr.s_addr = INADDR_ANY;

    // bind allows socket to listen for incoming client connections on
    // a specific address and port
    // side note: static_cast<> doesn't work because it's meant for "safe"
    // conversions 
    bind(serv_socket_fd, 
         reinterpret_cast<struct sockaddr*>(&serv_socket_address), 
         serv_address_len);
     
    // listent sets the socket listening for client connections
    // 10 is the number of connections that can be queued while the server
    // is busy
    listen(serv_socket_fd, 10);
    
    // accept a client connection on the socket
    int client_socket_fd{accept(serv_socket_fd, 
                         reinterpret_cast<struct sockaddr*>(&serv_socket_address),
                         reinterpret_cast<socklen_t*>(&serv_address_len))};
    
    // read client message into message buffer
    read(client_socket_fd, message_buffer, 1024);
    std::cout << "Message from client: " << message_buffer << "\n"; 
    
    close(client_socket_fd);
    close(serv_socket_fd);
    return 0;
}

int Server::Stop(){
   
    return 0;
}