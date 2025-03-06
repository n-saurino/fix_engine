#pragma once

struct Order {
  char cl_ord_id_[16];
  char orig_cl_ord_id_[16];
  char symbol_[8];
  int side_;
  int order_qty_;
  int ord_type_;
  double price_;
  char transact_time_[21];
};