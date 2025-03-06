#pragma once

class FIXMessageBase {
 public:
  FIXMessageBase() : seq_num_{seq_num_generator_++} {}
  static inline int seq_num_generator_{1};

 protected:
  int seq_num_;

 private:
  // static inline int seq_num_generator_{1};
};