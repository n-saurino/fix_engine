#pragma once
#include <cinttypes>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <string>

#include "MessageHeader.h"
#include "NewOrderSingle.h"

class FIXMessageBuilder {
 public:
  FIXMessageBuilder(/* args */);
  auto BuildMessage() -> int;

 private:
  /* data */
};
