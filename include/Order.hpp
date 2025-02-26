#pragma once
#include <string>

using OrderId = uint64_t;
using AccountId = uint64_t;
using Quantity = uint64_t;
using Symbol = std::string;
using OrderType = std::string;
using Price = int64_t;

enum Side { Bid, Ask };

struct Order {
  OrderId cl_order_id_{};
  AccountId account_id_{};
  Symbol symbol_{};
  Side side_{};
  // IMPROVEMENT: Settle on a convention for Time
  Quantity qty_{};
  Price price_{};
};
