#include <chrono>
#include <thread>

#include "FIXEngine.hpp"

void RunFIXEngine() { FIXEngine fix_engine{}; }

int main(int argc, char** argv) {
  // std::this_thread::sleep_for(std::chrono::seconds(2));

  // std::thread client_thread{RunFIXEngine};
  // client_thread.join();

  using CharBuffer = char[512];
  constexpr size_t kNumberBlocks{1024};
  MemoryPool<CharBuffer, kNumberBlocks> pool{};
  for (int i{}; i < 10000; ++i) {
    CharBuffer* ptr = pool.Allocate();
    pool.Free(ptr);
  }

  // build FIX message buffer
  // FIXMessageBuilder msg_bldr{};
  // std::cout << "Message: " << msg_bldr.Data() << "\n";

  return 0;
}
