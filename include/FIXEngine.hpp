#pragma once
#include "FIXNetworkHandler.hpp"

class FIXEngine {
 public:
  FIXEngine(/* args */);
  ~FIXEngine();

 private:
  FIXNetworkHandler network_handler{};
  /* data */
};
