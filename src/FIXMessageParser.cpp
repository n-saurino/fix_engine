#include "FIXMessageParser.hpp"

const int MESSAGE_HEADER_VERSION{0};

auto FIXMessageParser::DecodeHeader(sbe::MessageHeader& header, char* buffer,
                                    uint64_t offset, uint64_t buffer_len)
    -> std::size_t {
  header.wrap(buffer, offset, MESSAGE_HEADER_VERSION, buffer_len);

  // decode the header
  std::cout << "message_header.blockLength = " << header.blockLength() << "\n";
  std::cout << "message_header.templateId = " << header.templateId() << "\n";
  std::cout << "message_header.schemaId = " << header.schemaId() << "\n";
  std::cout << "message_header.schemaVersion = " << header.version() << "\n";
  std::cout << "message_header.encodedLength = " << header.encodedLength()
            << "\n";

  return header.encodedLength();
}

// IMPLEMENT
auto FIXMessageParser::DecodeMessage() -> std::size_t { return 0; }