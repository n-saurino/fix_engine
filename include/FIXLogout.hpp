#pragma once
#include "FIXMessage.hpp"

class FIXLogout : public FIXMessage<FIXLogout> {
 public:
  FIXLogout(FIXBuffer& buffer) : FIXMessage(buffer) {}
  void SerializeDerived() {
    fix_buffer_.Append(35, 5);
    fix_buffer_.Append(49, CompId::client_comp_id_);
    fix_buffer_.Append(56, CompId::server_comp_id_);
    fix_buffer_.Append(34, seq_num_);
    fix_buffer_.AppendUTC(52);
    fix_buffer_.Append(58, "Logout acknowledgement");
  }
};