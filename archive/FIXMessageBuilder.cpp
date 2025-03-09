#include "FIXMessageBuilder.hpp"

FIXMessageBuilder::FIXMessageBuilder(/* FIXMessage object */) {
  Append(8, "FIX.4.4");
  AppendPlaceHolder();
  Append(35, "A");
  Append(49, "FIX_CLIENT");
  Append(56, "FIX_SERVER");
  Append(34, 1);

  char time_buffer[80];
  GetCurrUTC(time_buffer);
  Append(52, time_buffer);

  Append(98, 0);
  Append(108, 30);
  Append(141, "Y");
  Append(553, "Username");
  Append(554, "Password");
  AppendMetadata();
  std::cout << "Message: " << buffer_ << "\n";
}

FIXMessageBuilder::~FIXMessageBuilder() = default;

// IMPROVEMENT: Let's find a way to use memcpy without snprintf. This is
// redundant.
void FIXMessageBuilder::Append(const int tag, const char* value) {
  char tag_str[10];
  int tag_len{snprintf(tag_str, sizeof(tag_str), "%d=", tag)};
  memcpy(buffer_ + len, tag_str, tag_len);
  len += tag_len;

  int value_len{strlen(value)};
  memcpy(buffer_ + len, value, value_len);
  len += value_len;

  memcpy(buffer_ + len, "\001", 1);
  ++len;
}

// IMPROVEMENT: Let's find a way to use memcpy without snprintf. This is
// redundant.
void FIXMessageBuilder::Append(const int tag, const int value) {
  char tag_str[10];
  int tag_len{snprintf(tag_str, sizeof(tag_str), "%d=", tag)};
  memcpy(buffer_ + len, tag_str, tag_len);
  len += tag_len;

  char val_str[10];
  int val_len{snprintf(val_str, sizeof(val_str), "%d", value)};
  memcpy(buffer_ + len, val_str, val_len);
  len += val_len;

  memcpy(buffer_ + len, "\001", 1);
  ++len;
}

void FIXMessageBuilder::AppendPlaceHolder() {
  body_len_pos = len;
  int tag_len{strlen("9=000\001")};
  memcpy(buffer_ + len, "9=000\001", tag_len);
  len += tag_len;
}

void FIXMessageBuilder::AppendMetadata() {
  AppendMessageLen();

  const int checksum{Checksum()};
  Append(10, checksum);
  buffer_[len++] = '\0';
}

void FIXMessageBuilder::AppendMessageLen() {
  size_t body_len{len - (body_len_pos + 6)};
  char body_len_str[3] = {static_cast<char>('0' + (body_len / 100) % 10),
                          static_cast<char>('0' + (body_len / 10) % 10),
                          static_cast<char>('0' + (body_len % 10))};
  memcpy(buffer_ + body_len_pos + 2, body_len_str, 3);
}

// IMPROVEMENT: Optimize the UTC calculation.
// consider not using an out-parameter?
// consider the C function overhead being
void FIXMessageBuilder::GetCurrUTC(char (&time_buffer)[80]) {
  struct timespec ts;
  // Get the current time with nanosecond resolution.
  clock_gettime(CLOCK_REALTIME, &ts);

  // Convert seconds to UTC time (thread-safe version)
  struct tm utc_tm;
  gmtime_r(&ts.tv_sec, &utc_tm);

  // Format: YearMonthDay-Hour:Minute:Second.microseconds
  // Note: ts.tv_nsec / 1000 converts nanoseconds to microseconds.
  snprintf(time_buffer, 80, "%04d%02d%02d-%02d:%02d:%02d.%03ld",
           utc_tm.tm_year + 1900, utc_tm.tm_mon + 1, utc_tm.tm_mday,
           utc_tm.tm_hour, utc_tm.tm_min, utc_tm.tm_sec, ts.tv_nsec / 1000);
}

// IMPROVEMENT: Assuming branching is slow here, need a better way to pad
// 0s in front of checksum value when it %s to a val < 100
const int FIXMessageBuilder::Checksum() {
  unsigned int checksum{};
  for (size_t i{}; i < len; ++i) {
    checksum += buffer_[i];
  }
  const int mod_val{checksum % 256};
  return mod_val;
}

auto FIXMessageBuilder::Data() const -> const char* { return buffer_; }

auto FIXMessageBuilder::Size() const -> size_t { return len; }