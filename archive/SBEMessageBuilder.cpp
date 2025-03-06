#include "SBEMessageBuilder.hpp"

const int MESSAGE_HEADER_VERSION{0};

SBEMessageBuilder::SBEMessageBuilder() {}

auto SBEMessageBuilder::EncodeHeader(sbe::MessageHeader& header,
                                     sbe::NewOrderSingle& new_order,
                                     char* buffer, std::uint64_t offset,
                                     std::uint64_t buffer_len) -> std::size_t {
  // encode the header
  header.wrap(buffer, offset, MESSAGE_HEADER_VERSION, buffer_len)
      .blockLength(sbe::NewOrderSingle::sbeBlockLength())
      .templateId(sbe::NewOrderSingle::sbeTemplateId())
      .schemaId(sbe::NewOrderSingle::sbeSchemaId())
      .version(sbe::NewOrderSingle::sbeSchemaVersion());

  return header.encodedLength();
}
// builds and returns a FIX SBE message based on string input of fields
auto SBEMessageBuilder::EncodeMessage(sbe::NewOrderSingle& new_order,
                                      char* buffer, std::uint64_t offset,
                                      std::uint64_t buffer_len) -> std::size_t {
  new_order.wrapForEncode(buffer, offset, buffer_len)
      .account(1)
      .clOrdId(1234)
      .symbol(600613)
      .side(sbe::SideEnum::Buy)
      .ordType(1)
      .transactTime(1234567890);

  new_order.price().mantissa(100);

  new_order.orderQty().mantissa(500);

  return new_order.encodedLength();
}