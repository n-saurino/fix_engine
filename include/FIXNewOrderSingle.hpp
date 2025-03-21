#pragma once
#include "FIXMessage.hpp"
#include "Order.hpp"

class FIXNewOrderSingle : public FIXMessage<FIXNewOrderSingle> {
 public:
  FIXNewOrderSingle(FIXBuffer& buffer, const Order& order)
      : FIXMessage(buffer), order_{order} {}

  void SerializeDerived() {
    fix_buffer_.Append(35, "D");
    fix_buffer_.Append(49, CompId::client_comp_id_);
    fix_buffer_.Append(56, CompId::server_comp_id_);
    fix_buffer_.Append(34, seq_num_);
    fix_buffer_.AppendUTC(52);
    fix_buffer_.Append(11, order_.cl_ord_id_);
    fix_buffer_.Append(21, 1);
    fix_buffer_.Append(38, order_.order_qty_);
    fix_buffer_.Append(40, order_.ord_type_);
    fix_buffer_.Append(44, order_.price_);
    fix_buffer_.Append(54, order_.side_);
    fix_buffer_.Append(55, order_.symbol_);
    fix_buffer_.AppendUTC(60);
  }

 private:
  const Order& order_;
};