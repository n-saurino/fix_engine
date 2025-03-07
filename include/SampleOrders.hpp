#pragma once
#include "Order.hpp"

struct SampleNewOrder : public Order {
  // Populate the test order.
  SampleNewOrder()
      : Order("CLORDID001", "", "NVDA", 1, 100, 2, 99.99,
              "20250306-12:34:56.000000") {}
};

struct SampleCancelOrder : public Order {
  SampleCancelOrder()
      : Order("CLORDID002", "CLORDID001", "NVDA", 1, 100, 2, 99.99,
              "20250306-12:34:56.000000") {}
};

struct SampleCancelReplaceOrder : public Order {
  SampleCancelReplaceOrder()
      : Order("CLORDID003", "CLORDID001", "NVDA", 1, 200, 2, 98.99,
              "20250306-12:34:56.000000") {}
};