#pragma once

#include <sanitizer/tsan_interface.h>

#include <atomic>
#include <cassert>
#include <memory>
#include <new>

/* Thread-safe, efficient LF Queue based on talk and example from Charles Frasch
(IEX Senior Dev). Followed along and built the queue from scratch based on his
implementation.
*/

// Planning to pass next iteration of FIXMessage struct
template <typename T, typename Alloc = std::allocator<T>>
class LFQueue : private Alloc {
 public:
  using ValueType = T;
  using allocator_traits = std::allocator_traits<Alloc>;
  using SizeType = typename allocator_traits::size_type;

  // capacity MUST be a power of 2 to take advantage of the fast indexing
  // from bitwise operations
  explicit LFQueue(SizeType capacity, Alloc const& alloc = Alloc{})
      : Alloc{alloc},
        bit_mask_{capacity - 1},
        ring_buffer_{allocator_traits::allocate(*this, capacity)},
        push_cursor_{},
        pop_cursor_{},
        cached_push_cursor_{},
        cached_pop_cursor_{} {
    ASSERT((capacity & (capacity - 1)) == 0,
           "Capacity must be a power of 2...");
  }

  ~LFQueue() {
    while (!Empty()) {
      Element(pop_cursor_)->~T();
      ++pop_cursor_;
    }
    allocator_traits::deallocate(*this, ring_buffer_, Capacity());
  }

  auto Size() const noexcept {
    auto push_cursor = push_cursor_.load(std::memory_order_relaxed);
    auto pop_cursor = pop_cursor_.load(std::memory_order_relaxed);
    ASSERT(pop_cursor <= push_cursor,
           "Pop cursor is larger than Push cursor when trying to evaluate ring "
           "buffer size...");
    return push_cursor - pop_cursor;
  }

  // push a T object onto the queue. If full -> false : true
  // consider using std::move and an rvalue as an enhancement? Probably
  // negligible
  auto Push(const T& val) {
    auto push_cursor = push_cursor_.load(std::memory_order_relaxed);

    if (Full(push_cursor, cached_pop_cursor_)) {
      cached_pop_cursor_ = pop_cursor_.load(std::memory_order_acquire);
      if (Full(push_cursor, cached_pop_cursor_)) return false;
    }

    // could potentially be a std::move instead of a copy?
    new (Element(push_cursor)) T(val);
    push_cursor_.store(push_cursor + 1, std::memory_order_release);
    return true;
  }

  // pop an element from the queue, using val as an out parameter that's passed
  // by reference
  // make sure to call T destructor on object in queue to prepare preallocated
  // ring buffer memory for reuse
  auto Pop(T& val) {
    auto pop_cursor = pop_cursor_.load(std::memory_order_relaxed);

    if (Empty(cached_push_cursor_, pop_cursor)) {
      cached_push_cursor_ = push_cursor_.load(std::memory_order_acquire);
      if (Empty(cached_push_cursor_, pop_cursor)) return false;
    }

    val = *Element(pop_cursor);
    Element(pop_cursor)->~T();
    pop_cursor_.store(pop_cursor + 1, std::memory_order_release);
    return true;
  }

  auto Full() const noexcept { return Size() == Capacity(); }

  auto Empty() const noexcept { return Size() == 0; }

  auto Capacity() { return bit_mask_ + 1; }

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
    return (push_cursor - pop_cursor) == Capacity();
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