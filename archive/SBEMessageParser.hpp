#pragma once
#include <iostream>
#include <string>

#include "MessageHeader.h"
#include "NewOrderSingle.h"

class SBEMessageParser {
 public:
  SBEMessageParser(/* args */) = default;
  auto DecodeHeader(sbe::MessageHeader& header, char* buffer,
                    std::uint64_t offset, std::uint64_t buffer_len)
      -> std::size_t;

  auto DecodeMessage(sbe::NewOrderSingle& message, char* buffer,
                     std::uint64_t offset, std::uint64_t acting_block_len,
                     std::uint64_t acting_version, std::uint64_t buffer_len)
      -> std::size_t;

 private:
  /* data */
};
