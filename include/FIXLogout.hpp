#pragma once
#include "FIXMessage.hpp"

class FIXLogout : public FIXMessage<FIXLogout> {
 public:
  void Serialize();
};