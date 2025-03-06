#pragma once
#include "FIXMessage.hpp"

class FIXNewOrderSingle : public FIXMessage<FIXNewOrderSingle> {
 public:
  FIXNewOrderSingle(FIXBuffer& buffer) : FIXMessage(buffer) {}

  void SerializeDerived() {
    fix_buffer_.Append(35, "D");
    fix_buffer_.Append(49, "FIX_CLIENT");
    fix_buffer_.Append(56, "FIX_SERVER");
    fix_buffer_.Append(34, 2);
    fix_buffer_.AppendUTC(52);
    fix_buffer_.Append(11, 1);
    fix_buffer_.Append(21, 1);
    fix_buffer_.Append(38, 100);
    fix_buffer_.Append(40, 2);
    fix_buffer_.Append(44, 250);
    fix_buffer_.Append(54, 1);
    fix_buffer_.Append(55, "NVDA");
    fix_buffer_.AppendUTC(60);
  }
};