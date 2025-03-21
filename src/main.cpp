#include <chrono>
#include <thread>

#include "FIXEngine.hpp"

void RunFIXEngine() { FIXEngine fix_engine{}; }

int main(int argc, char** argv) {
  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::thread client_thread{RunFIXEngine};
  client_thread.join();
  return 0;
}
