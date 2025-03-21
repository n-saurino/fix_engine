#include <gtest/gtest.h>

#include "FIXMessage.hpp"
#include "FIXNetworkHandler.hpp"
#include "MemoryPool.hpp"
#include "Order.hpp"
#include "SampleOrders.hpp"
#include "unordered_map"

class FIXMessageTest : public ::testing::Test {
 protected:
  FIXNetworkHandler handler_;
  SampleNewOrder orig_order_;
  SampleCancelOrder cancel_order_;
  SampleCancelReplaceOrder cancel_replace_order_;
  using CharBuffer = char[512];
  static constexpr size_t kNumBlocks{1024};
  MemoryPool<CharBuffer, kNumBlocks> pool_;

  void SetUp() override {
    // Setup code here, e.g., initialize network handler
    handler_.Start();
  }

  void TearDown() override {
    // Cleanup code here
    handler_.Stop();
  }

  auto SendMessageAndGetResponse(FIXBuffer& message) -> std::string {
    handler_.Send(message, handler_.GetSocket(0));
    char buffer[1024]{};
    handler_.ReceiveMessage(buffer, handler_.GetSocket(0));
    return static_cast<std::string>(buffer);
  }
};

TEST_F(FIXMessageTest, TestNewOrderSingle) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  CharBuffer* cb_ptr{pool_.Allocate()};
  FIXBuffer logon_buffer(*cb_ptr);
  FIXLogon logon(logon_buffer);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(logon_buffer);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 2: New Order Single ---
  cb_ptr = pool_.Allocate();
  FIXBuffer new_order_single_buffer(*cb_ptr);
  SampleNewOrder sample_new_order;

  FIXNewOrderSingle newOrder(new_order_single_buffer, sample_new_order);
  newOrder.Serialize();  // This calls SerializeDerived() internally.
  std::string newOrderResponse =
      SendMessageAndGetResponse(new_order_single_buffer);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 3: Logout ---
  cb_ptr = pool_.Allocate();
  FIXBuffer logout_buffer(*cb_ptr);
  FIXLogout logout(logout_buffer);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(logout_buffer);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
  pool_.Free(cb_ptr);
}

TEST_F(FIXMessageTest, TestHeartbeat) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  CharBuffer* cb_ptr{pool_.Allocate()};
  FIXBuffer logon_buffer(*cb_ptr);
  FIXLogon logon(logon_buffer);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(logon_buffer);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 2: Heartbeat ---
  cb_ptr = pool_.Allocate();
  FIXBuffer heartbeat_buffer(*cb_ptr);
  FIXHeartbeat heartbeat(heartbeat_buffer);
  heartbeat.Serialize();
  std::string heartbeatResponse = SendMessageAndGetResponse(heartbeat_buffer);
  std::cerr << "Heartbeat Response: " << heartbeatResponse << "\n";
  ASSERT_GE(heartbeatResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 3: Logout ---
  cb_ptr = pool_.Allocate();
  FIXBuffer logout_buffer(*cb_ptr);
  FIXLogout logout(logout_buffer);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(logout_buffer);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
  pool_.Free(cb_ptr);
}

TEST_F(FIXMessageTest, TestOrderCancelRequest) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  CharBuffer* cb_ptr{pool_.Allocate()};
  FIXBuffer logon_buffer(*cb_ptr);
  FIXLogon logon(logon_buffer);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(logon_buffer);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 2: New Order Single ---
  cb_ptr = pool_.Allocate();
  FIXBuffer new_order_buffer(*cb_ptr);
  SampleNewOrder sample_new_order{};
  FIXNewOrderSingle newOrder(new_order_buffer, sample_new_order);
  newOrder.Serialize();
  std::string newOrderResponse = SendMessageAndGetResponse(new_order_buffer);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 3: Order Cancel Request ---
  cb_ptr = pool_.Allocate();
  FIXBuffer cancel_buffer(*cb_ptr);
  SampleCancelOrder sample_cancel_order{};
  FIXOrderCancelRequest cancelRequest(cancel_buffer, sample_cancel_order);
  cancelRequest.Serialize();
  std::string cancelRequestResponse = SendMessageAndGetResponse(cancel_buffer);
  std::cerr << "OrderCancelRequest Response: " << cancelRequestResponse << "\n";
  ASSERT_GE(cancelRequestResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 4: Logout ---
  cb_ptr = pool_.Allocate();
  FIXBuffer logout_buffer(*cb_ptr);
  FIXLogout logout(logout_buffer);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(logout_buffer);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
  pool_.Free(cb_ptr);
}

TEST_F(FIXMessageTest, TestOrderCancelReplaceRequest) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  CharBuffer* cb_ptr{pool_.Allocate()};
  FIXBuffer logon_buffer(*cb_ptr);
  FIXLogon logon(logon_buffer);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(logon_buffer);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 2: New Order Single ---
  cb_ptr = pool_.Allocate();
  FIXBuffer new_order_buffer(*cb_ptr);
  SampleNewOrder sample_new_order{};
  FIXNewOrderSingle newOrder(new_order_buffer, sample_new_order);
  newOrder.Serialize();
  std::string newOrderResponse = SendMessageAndGetResponse(new_order_buffer);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 3: Order Cancel Replace Request ---
  cb_ptr = pool_.Allocate();
  FIXBuffer replace_buffer(*cb_ptr);
  SampleCancelReplaceOrder sample_cancel_replace_order{};
  FIXOrderCancelReplaceRequest replaceRequest(replace_buffer,
                                              sample_cancel_replace_order);
  replaceRequest.Serialize();
  std::string replaceRequestResponse =
      SendMessageAndGetResponse(replace_buffer);
  std::cerr << "OrderCancelReplaceRequest Response: " << replaceRequestResponse
            << "\n";
  ASSERT_GE(replaceRequestResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 4: Logout ---
  cb_ptr = pool_.Allocate();
  FIXBuffer logout_buffer(*cb_ptr);
  FIXLogout logout(logout_buffer);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(logout_buffer);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
  pool_.Free(cb_ptr);
}

TEST_F(FIXMessageTest, TestTestRequest) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  CharBuffer* cb_ptr{pool_.Allocate()};
  FIXBuffer logon_buffer(*cb_ptr);
  FIXLogon logon(logon_buffer);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(logon_buffer);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 2: Test Request ---
  cb_ptr = pool_.Allocate();
  FIXBuffer test_request_buffer(*cb_ptr);
  FIXTestRequest testRequest(test_request_buffer);
  testRequest.Serialize();
  std::string testRequestResponse =
      SendMessageAndGetResponse(test_request_buffer);
  std::cerr << "TestRequest Response: " << testRequestResponse << "\n";
  ASSERT_GE(testRequestResponse.size(), 1);
  pool_.Free(cb_ptr);

  // --- Step 3: Logout ---
  cb_ptr = pool_.Allocate();
  FIXBuffer logout_buffer(*cb_ptr);
  FIXLogout logout(logout_buffer);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(logout_buffer);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
  pool_.Free(cb_ptr);
}