#pragma once

struct Order {
  char cl_ord_id_[16]{};
  char orig_cl_ord_id_[16]{};
  char symbol_[8]{};
  int side_;
  int order_qty_;
  int ord_type_;
  double price_;
  char transact_time_[21]{};

  Order(){};
  Order(char cl_ord_id[16], char orig_cl_ord_id[16], char symbol[8], int side,
        int order_qty, int ord_type, double price, char transact_time[21])
      : side_{side}, order_qty_{order_qty}, ord_type_{ord_type}, price_{price} {
    strncpy(cl_ord_id_, cl_ord_id, sizeof(cl_ord_id));
    cl_ord_id_[sizeof(cl_ord_id_) - 1] = '\0';
    strncpy(orig_cl_ord_id_, orig_cl_ord_id, sizeof(orig_cl_ord_id));
    orig_cl_ord_id_[sizeof(orig_cl_ord_id_) - 1] = '\0';
    strncpy(symbol_, symbol, sizeof(symbol));
    symbol_[sizeof(symbol_) - 1] = '\0';
    strncpy(transact_time_, transact_time, sizeof(transact_time));
    transact_time_[sizeof(transact_time_) - 1] = '\0';
  }
};