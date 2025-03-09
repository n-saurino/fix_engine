#pragma once
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iostream>
/* IMPROVEMENT: could we build a std::array that maps all tags to their string
equivalents and then memcpy those over. Then we just need to figure out how to
efficiently convert an int to a char[] for the value*/
class FIXBuffer {
 public:
  // IMPROVEMENT: Let's find a way to use memcpy without snprintf. This is
  // redundant.
  void Append(const int tag, const char* val) {
    // Not handling bounds checking due to speed drag from branch misprediction
    char tag_str[10]{};
    int tag_len{snprintf(tag_str, sizeof(tag_str), "%d=", tag)};
    memcpy(buffer_ + len_, tag_str, tag_len);
    len_ += tag_len;

    int val_len{strlen(val)};
    memcpy(buffer_ + len_, val, val_len);
    len_ += val_len;

    memcpy(buffer_ + len_, "\001", 1);
    ++len_;
  }

  // IMPROVEMENT: Let's find a way to use memcpy without snprintf. This is
  // redundant.
  void Append(const int tag, const int val) {
    // Not handling bounds checking due to speed drag from branch misprediction
    char tag_str[10]{};
    int tag_len{snprintf(tag_str, sizeof(tag_str), "%d=", tag)};
    memcpy(buffer_ + len_, tag_str, tag_len);
    len_ += tag_len;

    char val_str[10]{};
    int val_len{snprintf(val_str, sizeof(val_str), "%d", val)};
    memcpy(buffer_ + len_, val_str, val_len);
    len_ += val_len;

    memcpy(buffer_ + len_, "\001", 1);
    ++len_;
  }

  void AppendUTC(const int tag) {
    struct timespec ts;
    // Get the current time with nanosecond resolution.
    clock_gettime(CLOCK_REALTIME, &ts);

    // Convert seconds to UTC time (thread-safe version)
    struct tm utc_tm;
    gmtime_r(&ts.tv_sec, &utc_tm);

    // IMPROVEMENT: Consider bounds checking to avoid buffer overflow
    // However, this is in the hot path and could produce branch mispredictions

    // Format: YearMonthDay-Hour:Minute:Second.microseconds
    // Note: ts.tv_nsec / 1000 converts nanoseconds to microseconds.
    int nbytes_w = snprintf(
        buffer_ + len_, 80, "%d=%04d%02d%02d-%02d:%02d:%02d.%03ld\001", tag,
        utc_tm.tm_year + 1900, utc_tm.tm_mon + 1, utc_tm.tm_mday,
        utc_tm.tm_hour, utc_tm.tm_min, utc_tm.tm_sec, ts.tv_nsec / 1000);
    len_ += nbytes_w;
  }

  void Begin(const int tag, const char* val) {
    Append(tag, val);
    body_len_pos_ = len_;
  }

  auto Checksum() -> int {
    int checksum{};
    for (size_t i{}; i < len_; ++i) {
      checksum += buffer_[i];
    }
    const int mod_val{checksum % 256};
    return mod_val;
  }

  void EditMessageLen() {
    size_t body_len{len_ - (body_len_pos_ + 6)};
    char body_len_str[3] = {static_cast<char>('0' + (body_len / 100) % 10),
                            static_cast<char>('0' + (body_len / 10) % 10),
                            static_cast<char>('0' + (body_len % 10))};
    memcpy(buffer_ + body_len_pos_ + 2, body_len_str, 3);
  }

  auto Data() const -> const char* { return buffer_; };
  void End() { buffer_[++len_] = '\0'; }
  void Reset() { len_ = 0; }
  auto Size() -> size_t { return len_; }

 private:
  static constexpr size_t kCapacity{512};
  char buffer_[kCapacity]{};
  size_t len_{};
  size_t body_len_pos_{};
};