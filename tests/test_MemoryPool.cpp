#include <gtest/gtest.h>

#include "MemoryPool.hpp"

class MemoryPoolTest : public ::testing::Test {
 protected:
  using CharBuffer = char[512];
  static constexpr size_t kNumBlocks{1024};
  MemoryPool<CharBuffer, kNumBlocks> pool_;
};

// Test 1: Ensure we can allocate a block
TEST_F(MemoryPoolTest, AllocateSingleBlock) {
  CharBuffer* block = pool_.Allocate();
  ASSERT_NE(block, nullptr);  // Ensure allocation succeeds
}

// Test 2: Allocate multiple blocks and ensure each block is unique
TEST_F(MemoryPoolTest, AllocateMultipleBlocks) {
  std::vector<CharBuffer*> blocks(pool_.Size());

  for (size_t i = 0; i < pool_.Size(); ++i) {
    blocks[i] = pool_.Allocate();
    ASSERT_NE(blocks[i], nullptr);
  }

  // Ensure blocks are unique
  for (size_t i = 1; i < pool_.Size(); ++i) {
    ASSERT_NE(blocks[i], blocks[i - 1]);
  }
}

// Test 3: Ensure pool reports full when all blocks are allocated
TEST_F(MemoryPoolTest, PoolFullAfterMaxAllocations) {
  for (size_t i = 0; i < pool_.Size(); ++i) {
    pool_.Allocate();
  }
  ASSERT_EQ(pool_.Full(), true);  // Pool should be full after max allocations
}

// Test 4: Ensure freeing a block marks it as available again
TEST_F(MemoryPoolTest, FreeBlock) {
  CharBuffer* block = pool_.Allocate();
  ASSERT_NE(block, nullptr);

  bool success = pool_.Free(block);
  ASSERT_EQ(success, true);  // Free should succeed

  // Allocate again, should be able to reuse freed block
  CharBuffer* new_block = pool_.Allocate();
  ASSERT_EQ(block, new_block);  // Ensure same block is reused
}

// Test 5: Ensure an invalid free (nullptr) returns false
TEST_F(MemoryPoolTest, FreeInvalidPointer) {
  EXPECT_DEATH(pool_.Free(nullptr), "Nullptr passed to Free...");
}

// Test 6: Ensure freeing multiple blocks allows reuse
TEST_F(MemoryPoolTest, FreeMultipleBlocks) {
  std::vector<CharBuffer*> blocks(pool_.Size());

  // Allocate all blocks
  for (size_t i = 0; i < pool_.Size(); ++i) {
    blocks[i] = pool_.Allocate();
    ASSERT_NE(blocks[i], nullptr);
  }

  // Free every other block
  for (size_t i = 0; i < pool_.Size(); i += 2) {
    ASSERT_EQ(pool_.Free(blocks[i]), true);
  }

  // Allocate again and check if freed blocks are reused
  for (size_t i = 0; i < pool_.Size(); i += 2) {
    CharBuffer* new_block = pool_.Allocate();
    ASSERT_NE(new_block, nullptr);
  }
}

// Test 7: Ensure freeing the last allocated block allows reuse
TEST_F(MemoryPoolTest, FreeLastBlock) {
  std::vector<CharBuffer*> blocks(pool_.Size());

  // Allocate all blocks
  for (size_t i = 0; i < pool_.Size(); ++i) {
    blocks[i] = pool_.Allocate();
    ASSERT_NE(blocks[i], nullptr);
  }

  // Free the last block
  ASSERT_EQ(pool_.Free(blocks[pool_.Size() - 1]), true);

  // Ensure we can allocate again
  CharBuffer* new_block = pool_.Allocate();
  ASSERT_NE(new_block, nullptr);
  ASSERT_EQ(new_block,
            blocks[pool_.Size() - 1]);  // Should reuse last freed block
}

// Test 8: Ensure allocating beyond pool capacity triggers an assertion (if
// using assertions)
TEST_F(MemoryPoolTest, AllocateBeyondCapacity) {
  for (size_t i = 0; i < pool_.Size(); ++i) {
    pool_.Allocate();
  }

  // Try one more allocation beyond capacity
  EXPECT_DEATH(pool_.Allocate(), "Memory pool out of space");
}

// int main(int argc, char** argv) {
//   ::testing::InitGoogleTest(&argc, argv);
//   return RUN_ALL_TESTS();
// }