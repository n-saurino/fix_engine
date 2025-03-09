#pragma once
#include <atomic>

class FIXMessageBase {
 public:
  FIXMessageBase()
      : seq_num_{seq_num_generator_.fetch_add(1, std::memory_order_relaxed)} {}
  static inline std::atomic<int> seq_num_generator_{1};

 protected:
  int seq_num_;

 private:
  // static inline int seq_num_generator_{1};
};