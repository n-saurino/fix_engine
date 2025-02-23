#pragma once
#include "FIXNetworkHandler.hpp"

class FIXEngine {
 public:
  FIXEngine(/* args */) = default;
  // ~FIXEngine() = default;
  // FIXEngine(const FIXEngine&) = delete;
  // FIXEngine(const FIXEngine&&) = delete;
  // constexpr auto operator=(const FIXEngine&) -> FIXEngine& = delete;
  // constexpr auto operator=(const FIXEngine&&) -> FIXEngine& = delete;

 private:
  FIXNetworkHandler network_handler;
};
