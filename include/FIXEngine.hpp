#pragma once
#include <thread>

#include "FIXNetworkHandler.hpp"
#include "LFQueue.hpp"
#include "MemoryPool.hpp"
#include "SampleOrders.hpp"

// Main FIXEngine Thread: Receives orders, encodes them, and enqueues messages.
// TX Thread: Continuously polls the LFQueue for messages and sends them over
// the network.
// RX Thread: Listens for incoming network data and processes responses or
// acknowledgments.
// Shared Resources: LFQueues and Memory Pool are managed within the FIXEngine,
// ensuring efficient data handling.
class FIXEngine {
 public:
  FIXEngine(/* args */) = default;
  // ~FIXEngine() = default;
  // FIXEngine(const FIXEngine&) = delete;
  // FIXEngine(const FIXEngine&&) = delete;
  // constexpr auto operator=(const FIXEngine&) -> FIXEngine& = delete;
  // constexpr auto operator=(const FIXEngine&&) -> FIXEngine& = delete;
  using CharBuffer = char[512];

  // setup and startup:
  // launch TX and RX network handler threads, build LFQueues, create thread-
  // local MemoryPool of CharBuffers for FIXMessages, set polling on in_queue_
  void Start() {}

 private:
  static constexpr size_t kNumBlocks{1024};
  LFQueue<CharBuffer*> out_queue_{kNumBlocks};
  LFQueue<CharBuffer*> in_queue_{kNumBlocks};
};
