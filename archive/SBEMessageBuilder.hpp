#pragma once
#include <cinttypes>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>

#include "MessageHeader.h"
#include "NewOrderSingle.h"

class SBEMessageBuilder {
 public:
  SBEMessageBuilder(/* args */);
  auto EncodeHeader(sbe::MessageHeader& header, sbe::NewOrderSingle& new_order,
                    char* buffer, std::uint64_t offset,
                    std::uint64_t buffer_len) -> std::size_t;
  auto EncodeMessage(sbe::NewOrderSingle& new_order, char* buffer,
                     std::uint64_t offset, std::uint64_t buffer_len)
      -> std::size_t;

 private:
  /* data */
};
