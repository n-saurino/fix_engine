#pragma once

#include <sanitizer/tsan_interface.h>

#include <atomic>
#include <cassert>
#include <memory>
#include <new>

/* Thread-safe, efficient LF Queue based on talk and example from Charles Frasch
(IEX Senior Dev). Followed along and build the queue from scratch based on his
implementation.
*/

template <typename T, typename Alloc = std::allocator<T>>
class LFQueue : private Alloc {
 public:
  using ValueType = T;
  using AllocatorTraits = std::allocator_traits<Alloc>;
  using SizeType = typename AllocatorTraits::size_type;

 private:
  // bit mask to make indexing ring buffer fast ~ 1 cycle by using bitwise &
  SizeType bit_mask_;
  // pointer access to underlying ring buffer used for our lock-free queue
  T* ring_buffer_;

  using CursorType = std::atomic<SizeType>;
  static_assert(CursorType::is_always_lock_free);

  // setting cache line size for Mac M2 = 128 bytes
  static constexpr auto kHardwareDestructiveInterferenceSize = SizeType{128};

  // loaded and stored by the push thread
  // only Loaded by the pop thread
  alignas(kHardwareDestructiveInterferenceSize) CursorType push_cursor_;

  // loaded and stored by the pop thread
  // only Loaded by the push thread
  alignas(kHardwareDestructiveInterferenceSize) CursorType pop_cursor_;

  // cached push cursor value loaded and rechecked for an empty queue
  // only present in the pop thread to avoid repeat loads of atomic push cursor
  alignas(kHardwareDestructiveInterferenceSize) SizeType cached_push_cursor_;

  // cached pop cursor value that will be loaded and rechecked as a flag to
  // check for a full queue.
  // only present in the push thread as a way to avoid repeat loads of the
  // atomic pop cursor
  alignas(kHardwareDestructiveInterferenceSize) SizeType cached_pop_cursor_;

  // padding to avoid false sharing with adjacent objects
  char padding_[kHardwareDestructiveInterferenceSize - sizeof(SizeType)];

  // is queue full?
  auto Full(SizeType push_cursor, SizeType pop_cursor) const noexcept {
    // return (push_cursor - pop_cursor) == Capacity();
  }

  // is queue empty?
  static auto Empty(SizeType push_cursor, SizeType pop_cursor) noexcept {
    return push_cursor == pop_cursor;
  }

  // single cycle cursor to ring buffer index conversion
  auto Element(SizeType cursor) noexcept {
    return &ring_buffer_[cursor & bit_mask_];
  }
};