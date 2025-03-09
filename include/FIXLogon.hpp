#pragma once
#include "FIXMessage.hpp"

class FIXLogon : public FIXMessage<FIXLogon> {
 public:
  explicit FIXLogon(FIXBuffer& buffer) : FIXMessage(buffer) {}
  void SerializeDerived() {
    fix_buffer_.Append(35, "A");
    fix_buffer_.Append(49, CompId::client_comp_id_);
    fix_buffer_.Append(56, CompId::server_comp_id_);
    fix_buffer_.Append(34, seq_num_);
    fix_buffer_.AppendUTC(52);
    fix_buffer_.Append(98, 0);
    fix_buffer_.Append(108, 30);
    fix_buffer_.Append(141, "Y");
    fix_buffer_.Append(553, "Username");
    fix_buffer_.Append(554, "Password");
  }
};