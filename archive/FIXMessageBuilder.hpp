#pragma once
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>

/* IMPROVEMENT:	Hand-Rolled Converters:
Standard library functions like snprintf are avoided in favor of custom routines
that convert integers or floating-point numbers directly to fixed-width ASCII
strings. Techniques like using memcpy for constant parts (such as delimiters)
and inlined number-to-string conversions ensure that every cycle is used
efficiently.
*/
class FIXMessageBuilder {
 public:
  FIXMessageBuilder(/* args */);
  ~FIXMessageBuilder();
  void EncodeFIXMessage();
  void Append(int tag, const char* value);
  void Append(int tag, const int value);
  void AppendPlaceHolder();
  void AppendMessageLen();
  void AppendMetadata();
  void GetCurrUTC(char (&time_buffer)[80]);
  const int Checksum();
  auto Data() const -> const char*;
  auto Size() const -> size_t;

 private:
  static constexpr size_t kBufferSize{1024};
  char buffer_[kBufferSize];
  size_t body_len_pos{0};
  size_t len{0};
};
