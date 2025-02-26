#pragma once
#include <iostream>
#include <string>

#include "MessageHeader.h"
#include "NewOrderSingle.h"
#include "Order.hpp"

class FIXMessageParser {
 public:
  FIXMessageParser(/* args */) = default;
  auto DecodeHeader(sbe::MessageHeader& header, char* buffer,
                    std::uint64_t offset, std::uint64_t buffer_len)
      -> std::size_t;
  auto DecodeMessage() -> std::size_t;

 private:
  /* data */
};
