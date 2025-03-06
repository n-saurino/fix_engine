#pragma once
#include "FIXMessage.hpp"
#include "Order.hpp"

class FIXOrderCancelRequest : public FIXMessage<FIXOrderCancelRequest> {
 public:
  FIXOrderCancelRequest(FIXBuffer& buffer, const Order& order)
      : FIXMessage{buffer}, order_{order} {}

  void SerializeDerived() {
    fix_buffer_.Append(35, "F");
    fix_buffer_.Append(49, "FIX_CLIENT");
    fix_buffer_.Append(56, "FIX_SERVER");
    fix_buffer_.Append(34, seq_num_);
    fix_buffer_.AppendUTC(52);
    fix_buffer_.Append(11, order_.cl_ord_id_);
    fix_buffer_.Append(41, order_.orig_cl_ord_id_);
    fix_buffer_.Append(54, order_.side_);
    fix_buffer_.Append(55, order_.symbol_);
    fix_buffer_.Append(38, order_.order_qty_);
    fix_buffer_.AppendUTC(60);
  }

 private:
  const Order& order_;
};