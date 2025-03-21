#include <chrono>
#include <thread>

#include "FIXEngine.hpp"

void RunFIXEngine() { FIXEngine fix_engine{}; }

int main(int argc, char** argv) {
  // std::this_thread::sleep_for(std::chrono::seconds(2));

  // std::thread client_thread{RunFIXEngine};
  // client_thread.join();

  using CharBuffer = char[512];
  constexpr size_t kNumberBlocks{4};
  MemoryPool<CharBuffer, kNumberBlocks> pool{};
  std::vector<CharBuffer*> blocks(pool.Size());

  for (size_t i = 0; i < pool.Size(); ++i) {
    blocks[i] = pool.Allocate();
    ASSERT(blocks[i] != nullptr, "Pointer from Allocate() is a nullptr");
  }

  // Ensure blocks are unique
  for (size_t i = 1; i < pool.Size(); ++i) {
    ASSERT(blocks[i] != blocks[i - 1],
           "Pointer to blocks from Allocate are not unique");
  }

  // build FIX message buffer
  // FIXMessageBuilder msg_bldr{};
  // std::cout << "Message: " << msg_bldr.Data() << "\n";

  return 0;
}
