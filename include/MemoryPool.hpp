#pragma once
#include <cstddef>
#include <cstring>
#include <iostream>

#include "Macros.hpp"

class MemoryPool {
 public:
  MemoryPool() : curr_idx_{0} {
    // initialize the array storing the flags for which blocks are free
    for (int i{}; i < kTotalBlocks; ++i) {
      free_block_arr_[i] = false;
    }

    // initialize memory pool
    for (int i{}; i < kMemPoolCapacity; ++i) {
      memset(pool_ + i, '\0', 0);
    }
  }

  //   ~MemoryPool() = delete;
  //   MemoryPool(const MemoryPool&) = delete;
  //   MemoryPool(MemoryPool&&) = delete;
  //   MemoryPool& operator=(const MemoryPool&) = delete;
  //   MemoryPool& operator=(MemoryPool&&) = delete;

  char* Allocate() {
    ASSERT(
        !Full(),
        "Memory pool out of space! Allocation of new memory block failed...\n");

    size_t buffer_idx{curr_idx_};
    // set the block to 1 in free_bloc_arr_
    free_block_arr_[curr_idx_++] = true;
    UpdateNextFreeIdx();
    // return memory address of the first char of the block
    return &pool_[buffer_idx * kBlockSize];
  }

  bool Free(const char* buffer_ptr) {
    if (!buffer_ptr) {
      return false;
    }

    // intentionally ignoring ptr alignment with block size for speed
    size_t idx = (&(*buffer_ptr) - pool_) / kBlockSize;

    ASSERT(idx < kTotalBlocks,
           "Tried to free memory that is outside of the Memory Pool! Please "
           "look into the pointer that you are passing to Free().\n");

    // set the block to 0 in free_block_arr_
    free_block_arr_[idx] = 0;

    if (idx < curr_idx_) {
      curr_idx_ = idx;
    }
    return true;
  }

  auto Full() -> const bool { return curr_idx_ == kTotalBlocks; }
  auto Size() -> const size_t { return kTotalBlocks; }

 private:
  static constexpr inline size_t kTotalBlocks{10};
  static constexpr inline size_t kBlockSize{512};
  static constexpr inline size_t kMemPoolCapacity{kTotalBlocks * kBlockSize};
  char pool_[kMemPoolCapacity];
  // do we need a fast lookup for free blocks? Should it be faster than linear?
  // linear on small arrays is extremely fast
  bool free_block_arr_[kTotalBlocks];
  size_t curr_idx_{0};

  void UpdateNextFreeIdx() {
    while (free_block_arr_[curr_idx_] && curr_idx_ < kTotalBlocks) {
      ++curr_idx_;
    }
  }
};