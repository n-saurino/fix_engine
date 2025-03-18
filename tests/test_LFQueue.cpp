#include <vector>

#include "LFQueue.hpp"
#include "gtest/gtest.h"

// Test fixture for LFQueue using int as the value type and a small capacity.
class LFQueueTest : public ::testing::Test {
 protected:
  // We'll use a small capacity (must be a power of 2).
  inline static const size_t kCapacity{8};
  LFQueue<int> queue;

  LFQueueTest() : queue(kCapacity) {}

  void SetUp() override {
    // No additional setup needed.
  }

  void TearDown() override {
    // No cleanup required as LFQueue manages its own memory.
  }
};

// Test that a push followed by a pop yields the pushed value.
TEST_F(LFQueueTest, BasicPushPop) {
  int value = 42;
  EXPECT_EQ(queue.Empty(), true);
  EXPECT_EQ(queue.Full(), false);

  EXPECT_EQ(queue.Push(value), true);
  EXPECT_EQ(queue.Size(), 1);
  EXPECT_EQ(queue.Empty(), false);

  int popped = 0;
  EXPECT_EQ(queue.Pop(popped), true);
  EXPECT_EQ(popped, value);
  EXPECT_EQ(queue.Size(), 0);
  EXPECT_EQ(queue.Empty(), true);
}

// Test that the queue reports being full when capacity is reached.
TEST_F(LFQueueTest, QueueFullTest) {
  // Fill the queue to capacity.
  for (int i = 0; i < kCapacity; ++i) {
    EXPECT_EQ(queue.Push(i), true);
  }
  EXPECT_EQ(queue.Size(), kCapacity);
  EXPECT_EQ(queue.Full(), true);

  // Attempting one more push should return false.
  EXPECT_EQ(queue.Push(999), false);
}

// Test that popping from an empty queue fails.
TEST_F(LFQueueTest, QueueEmptyTest) {
  int value = 0;
  EXPECT_EQ(queue.Empty(), true);
  EXPECT_EQ(queue.Pop(value), false);
}

// Test that the queue maintains FIFO (first-in, first-out) order.
TEST_F(LFQueueTest, FIFOOrder) {
  std::vector<int> inputs = {1, 2, 3, 4, 5};
  // Enqueue elements.
  for (int x : inputs) {
    EXPECT_EQ(queue.Push(x), true);
  }
  // Dequeue elements and check order.
  for (int expected : inputs) {
    int output = 0;
    EXPECT_EQ(queue.Pop(output), true);
    EXPECT_EQ(output, expected);
  }
  EXPECT_EQ(queue.Empty(), true);
}

// Test that the Size() function reflects the correct number of elements.
TEST_F(LFQueueTest, SizeTest) {
  EXPECT_EQ(queue.Size(), 0);
  // Push a few items.
  for (int i = 0; i < 3; ++i) {
    EXPECT_EQ(queue.Push(i), true);
  }
  EXPECT_EQ(queue.Size(), 3);

  int value = 0;
  EXPECT_EQ(queue.Pop(value), true);
  EXPECT_EQ(queue.Size(), 2);
}