#pragma once

struct FIXMessage {
  const int MAX_FIELDS[1024];
  struct FIXField {
    int tag;
    const char* value;
  };
  FIXField map_[MAX_FIELDS];
};

struct Logon : public FIXMessage {};

struct Logout : public FIXMessage {};

struct NewOrderSingle : public FIXMessage {};