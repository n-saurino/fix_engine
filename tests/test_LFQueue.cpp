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
  EXPECT_TRUE(queue.Empty());
  EXPECT_FALSE(queue.Full());

  EXPECT_TRUE(queue.Push(value));
  EXPECT_EQ(queue.Size(), 1);
  EXPECT_FALSE(queue.Empty());

  int popped = 0;
  EXPECT_TRUE(queue.Pop(popped));
  EXPECT_EQ(popped, value);
  EXPECT_EQ(queue.Size(), 0);
  EXPECT_TRUE(queue.Empty());
}

// Test that the queue reports being full when capacity is reached.
TEST_F(LFQueueTest, QueueFullTest) {
  // Fill the queue to capacity.
  for (int i = 0; i < kCapacity; ++i) {
    EXPECT_TRUE(queue.Push(i));
  }
  EXPECT_EQ(queue.Size(), kCapacity);
  EXPECT_TRUE(queue.Full());

  // Attempting one more push should return false.
  EXPECT_FALSE(queue.Push(999));
}

// Test that popping from an empty queue fails.
TEST_F(LFQueueTest, QueueEmptyTest) {
  int value = 0;
  EXPECT_TRUE(queue.Empty());
  EXPECT_FALSE(queue.Pop(value));
}

// Test that the queue maintains FIFO (first-in, first-out) order.
TEST_F(LFQueueTest, FIFOOrder) {
  std::vector<int> inputs = {1, 2, 3, 4, 5};
  // Enqueue elements.
  for (int x : inputs) {
    EXPECT_TRUE(queue.Push(x));
  }
  // Dequeue elements and check order.
  for (int expected : inputs) {
    int output = 0;
    EXPECT_TRUE(queue.Pop(output));
    EXPECT_EQ(output, expected);
  }
  EXPECT_TRUE(queue.Empty());
}

// Test that the Size() function reflects the correct number of elements.
TEST_F(LFQueueTest, SizeTest) {
  EXPECT_EQ(queue.Size(), 0);
  // Push a few items.
  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(queue.Push(i));
  }
  EXPECT_EQ(queue.Size(), 3);

  int value = 0;
  EXPECT_TRUE(queue.Pop(value));
  EXPECT_EQ(queue.Size(), 2);
}