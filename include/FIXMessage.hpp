#pragma once
#include <string_view>

#include "FIXBuffer.hpp"
#include "FIXMessageBase.hpp"

struct CompId {
  // string_view instead of char[] so we can use constexpr for compile time
  // evaluation
  // constexpr static inline std::string_view client_comp_id_ = "FIX_CLIENT";
  // constexpr static inline std::string_view server_comp_id_ = "FIX_SERVER";
  // constexpr static inline std::string_view account_id_ = "Nigel";
  constexpr static inline char client_comp_id_[16] = "FIX_CLIENT";
  constexpr static inline char server_comp_id_[16] = "FIX_SERVER";
  constexpr static inline char account_id_[16] = "Nigel";
};

template <typename Derived>
class FIXMessage : public FIXMessageBase {
 public:
  explicit FIXMessage(FIXBuffer& buffer) : fix_buffer_{buffer} {}
  // calls derived class Serialize function
  void Serialize() {
    AppendHeader();
    auto& underlying = static_cast<Derived&>(*this);
    underlying.SerializeDerived();
    AppendTrailer();
  }

 protected:
  FIXMessage() = default;
  // have to FIXBuffer object protected so that it can be accessed from Derived
  // classes
  FIXBuffer& fix_buffer_;

 private:
  // Appends Header to FIXBuffer - same for every FIXMessage
  void AppendHeader() {
    fix_buffer_.Begin(8, "FIX.4.4");
    fix_buffer_.Append(9, "000");
  }

  // Appends Trailer to FIXBuffer - same for every FIXMessage
  void AppendTrailer() {
    fix_buffer_.EditMessageLen();

    const int checksum{fix_buffer_.Checksum()};
    fix_buffer_.Append(10, checksum);
    fix_buffer_.End();
  }
};