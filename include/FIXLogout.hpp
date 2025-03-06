#pragma once
#include "FIXMessage.hpp"

class FIXLogout : public FIXMessage<FIXLogout> {
 public:
  FIXLogout(FIXBuffer& buffer) : FIXMessage(buffer) {}
  void SerializeDerived() {
    fix_buffer_.Append(35, 5);
    fix_buffer_.Append(49, "FIX_CLIENT");
    fix_buffer_.Append(56, "FIX_SERVER");
    fix_buffer_.Append(34, seq_num_);
    fix_buffer_.AppendUTC(52);
    fix_buffer_.Append(58, "Logout acknowledgement");
  }
};