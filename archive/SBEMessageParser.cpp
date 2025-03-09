#include "SBEMessageParser.hpp"

const int MESSAGE_HEADER_VERSION{0};

auto SBEMessageParser::DecodeHeader(sbe::MessageHeader& header, char* buffer,
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

auto SBEMessageParser::DecodeMessage(sbe::NewOrderSingle& message, char* buffer,
                                     std::uint64_t offset,
                                     std::uint64_t acting_block_len,
                                     std::uint64_t acting_version,
                                     std::uint64_t buffer_len) -> std::size_t {
  message.wrapForDecode(buffer, offset, acting_block_len, acting_version,
                        buffer_len);

  std::cout.setf(std::ios::fixed);

  std::cout << "\nmessage.accountId = " << sbe::NewOrderSingle::accountId();
  std::cout << "\nmessage.clOrdIdId = " << sbe::NewOrderSingle::clOrdIdId();
  std::cout << "\nmessage.symbolId = " << sbe::NewOrderSingle::symbolId();
  std::cout << "\nmessage.sideId = " << sbe::NewOrderSingle::sideId();
  std::cout << "\nmessage.ordTypeId = " << sbe::NewOrderSingle::ordTypeId();
  std::cout << "\nmessage.transactTimeId = "
            << sbe::NewOrderSingle::transactTimeId();
  std::cout << "\nmessage.priceId = " << sbe::NewOrderSingle::priceId();
  std::cout << "\nmessage.orderQtyId = " << sbe::NewOrderSingle::orderQtyId();

  std::cout << "\n";

  std::cout << "\nmessage.account = " << message.account();
  std::cout << "\nmessage.clOrdId = " << message.clOrdId();
  std::cout << "\nmessage.symbol = " << message.symbol();
  std::cout << "\nmessage.side = " << message.side();
  std::cout << "\nmessage.ordType = " << message.ordType();
  std::cout << "\nmessage.transactTime = " << message.transactTime();
  std::cout << "\nmessage.price = " << message.price();
  std::cout << "\nmessage.orderQty = " << message.orderQty();

  std::cout << "\nmessage.encodedLength = "
            << static_cast<int>(message.encodedLength()) << "\n";
  return message.encodedLength();
}