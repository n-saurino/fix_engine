#pragma once
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <new>
#include <vector>

#include "Macros.hpp"

/* Refactoring to make templated and integrate some things I learned from
 * Charles Frasch's SPSC LFQueue implementation.*/

// this MemoryPool is intended to be instantiated with a char[512] buffer for T.
// opted to template the Memory Pool on Capacity to make it a compile time
// value and allow us to have a stack allocated free_list_ instead of using
// a preallocated std::vector with reserve() that requires indirection to the
// heap
template <typename T, size_t Capacity, typename Alloc = std::allocator<T>>
class MemoryPool : private Alloc {
 public:
  using ValueType = T;
  using allocator_traits = std::allocator_traits<Alloc>;
  using SizeType = typename allocator_traits::size_type;

  explicit MemoryPool(Alloc const& alloc = Alloc{})
      : Alloc{alloc},
        kTotalBlocks{Capacity},
        kBlockSize{sizeof(T)},
        kMemPoolCapacity{kTotalBlocks * kBlockSize},
        curr_idx_{Capacity - 1},
        pool_{allocator_traits::allocate(*this, Capacity)} {
    ASSERT((Capacity & (Capacity - 1)) == 0,
           "MemoryPool total blocks must be a power of 2...");
    ASSERT((kBlockSize & (kBlockSize - 1)) == 0,
           "Template type T size must be a power of 2...");

    // initialize free_list_ stack
    for (int i{}; i < kTotalBlocks; ++i) {
      free_list_[i] = i;
    }

    // initialize memory pool
    memset(pool_, '\0', kMemPoolCapacity);
  }

  ~MemoryPool() { allocator_traits::deallocate(*this, pool_, kTotalBlocks); }
  //   MemoryPool(const MemoryPool&) = delete;
  //   MemoryPool(MemoryPool&&) = delete;
  //   MemoryPool& operator=(const MemoryPool&) = delete;
  //   MemoryPool& operator=(MemoryPool&&) = delete;

  inline T* Allocate() {
    ASSERT(
        !Full(),
        "Memory pool out of space! Allocation of new memory block failed...\n");

    // return memory address of T block
    size_t pool_idx = free_list_[curr_idx_];
    --curr_idx_;
    return &pool_[pool_idx];
  }

  inline bool Free(const T* buffer_ptr) {
    ASSERT(buffer_ptr, "Nullptr passed to Free()...");

    // intentionally ignoring ptr alignment with block size for speed
    size_t idx = buffer_ptr - pool_;

    ASSERT(idx < kTotalBlocks,
           "Tried to free memory that is outside of the Memory Pool! Please "
           "look into the pointer that you are passing to Free().\n");

    ASSERT(!Empty(), "Trying to Free() an empty Memory Pool...");

    ++curr_idx_;
    free_list_[curr_idx_] = idx;

    return true;
  }

  auto Empty() -> const bool { return curr_idx_ == kTotalBlocks - 1; };
  auto Full() -> const bool { return curr_idx_ == 0; }
  auto Size() -> const size_t { return kTotalBlocks; }
  void Reset() {
    for (int i{}; i < kTotalBlocks; ++i) {
      free_list_[i] = i;
    }
    curr_idx_ = kTotalBlocks - 1;
  }

 private:
  size_t kTotalBlocks{};
  size_t kBlockSize{};
  size_t kMemPoolCapacity{};
  T* pool_;
  SizeType free_list_[Capacity];
  SizeType curr_idx_{};
};