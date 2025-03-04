#include "FIXNetworkHandler.hpp"

#include <array>
#include <string_view>

FIXNetworkHandler::FIXNetworkHandler(/* args */) { Start(); }

void FIXNetworkHandler::Test(const char* test_type,
                             const int client_socket_fd) {
  const int BUFFER_SIZE{1024};
  std::array<char, BUFFER_SIZE> in_buffer{};
  const char* out_buffer{"PONG"};
  const char* exit_signal{"EXIT"};
  // initiate latency test
  int nbytes_s =
      static_cast<int>(send(client_socket_fd, test_type, strlen(test_type), 0));

  int nbytes_r{};
  while ((nbytes_r = static_cast<int>(
              recv(client_socket_fd, &in_buffer, sizeof(in_buffer), 0))) > 0) {
    if (exit_signal == std::string_view(in_buffer.data())) {
      return;
    }
    send(client_socket_fd, out_buffer, strlen(out_buffer), 0);
  }
}

void FIXNetworkHandler::Start() {
  const int BUFFER_SIZE{1024};
  const int PORT_NUM{5001};
  struct sockaddr_in client_socket_address{};
  int client_address_len{sizeof(client_socket_address)};
  int client_socket_fd{socket(AF_INET, SOCK_STREAM, 0)};
  const char* latency_test{"LATENCY_TEST"};
  const char* throughput_test{"THROUGHPUT_TEST"};
  const char* out_buffer{"PONG\n"};
  std::array<char, BUFFER_SIZE> in_buffer{};
  client_socket_address.sin_family = AF_INET;
  client_socket_address.sin_port = htons(PORT_NUM);

  inet_pton(AF_INET, "172.18.0.3", &client_socket_address.sin_addr);

  int connect_result{};

  // choosing to violate the C++ guidelines and use reinterpret cast
  // because this is a well-defined conversion in the POSIX socket API
  connect_result =
      connect(client_socket_fd,
              reinterpret_cast<struct sockaddr*>(&client_socket_address),
              static_cast<socklen_t>(client_address_len));
  if (connect_result == -1) {
    std::cerr << "Client's network handler failed to connect: "
              << strerror(errno) << "\n";
    close(client_socket_fd);
    return;
  }

  // test protocol
  // Test(latency_test, client_socket_fd);
  // Test(throughput_test, client_socket_fd);
  // Test("EXIT", client_socket_fd);

  // test FIX SBE protocol communication
  SendMessage(client_socket_fd);

  close(client_socket_fd);
}

// Improvement: Consider overloading or templating for different message
// objects?
void FIXNetworkHandler::SendMessage(/* Message, */ const int client_socket_fd) {
  const int kBufferSize{2048};
  std::array<char, kBufferSize> buffer{
      "8=FIX.4.4\x019=102\x0135=A\x0149=BuySide\x0156=SellSide\x0134=1\x0152="
      "20190605-11:40:30."
      "392\x0198=0\x01108=30\x01141=Y\x01553=Username\x01554=Password\x0110="
      "104\x01"};

  int nbytes_s = static_cast<int>(
      send(client_socket_fd, buffer.data(), sizeof(buffer), 0));

  if (nbytes_s < 0) {
    std::cerr << "FIXNetworkHandler failed to send: " << strerror(errno)
              << "\n";
  }
  /*
    const int kBufferSize{2048};
    std::array<char, kBufferSize> buffer{};
    std::array<char, kBufferSize> in_buffer{};
    FIXMessageBuilder fix_msg_bldr;
    sbe::MessageHeader hdr;
    sbe::NewOrderSingle new_order_single;

    std::size_t encode_hdr_len{fix_msg_bldr.EncodeHeader(
        hdr, new_order_single, buffer.data(), 0, sizeof(buffer))};

    std::size_t encode_msg_len{fix_msg_bldr.EncodeMessage(
        new_order_single, buffer.data(), hdr.encodedLength(), sizeof(buffer))};

    std::cout << "Encoded Lengths are " << encode_hdr_len << " + "
              << encode_msg_len << "\n";

    // IMPROVEMENT: need to send message over the wire
    int nbytes_s =
        static_cast<int>(send(client_socket_fd, new_order_single.buffer(),
                              new_order_single.bufferLength(), 0));

    if (nbytes_s < 0) {
      std::cerr << "FIXNetworkHandler failed to send message to client: "
                << strerror(errno) << "\n";
    }

    int nbytes_r = static_cast<int>(
        recv(client_socket_fd, &in_buffer, sizeof(in_buffer), 0));

    if (nbytes_r < 0) {
      std::cerr
          << "FIXNetworkHandler failed to receive message from test server: "
          << strerror(errno) << "\n";
    }
  */
}
