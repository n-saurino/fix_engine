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
  MemoryPool pool_;

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
  FIXBuffer logon_buffer(pool_.Allocate());
  FIXLogon logon(logon_buffer);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(logon_buffer);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);
  pool_.Free(logon_buffer.Data());

  // --- Step 2: New Order Single ---
  FIXBuffer new_order_single_buffer(pool_.Allocate());
  SampleNewOrder sample_new_order;

  FIXNewOrderSingle newOrder(new_order_single_buffer, sample_new_order);
  newOrder.Serialize();  // This calls SerializeDerived() internally.
  std::string newOrderResponse =
      SendMessageAndGetResponse(new_order_single_buffer);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);
  pool_.Free(new_order_single_buffer.Data());

  // --- Step 3: Logout ---
  FIXBuffer logout_buffer(pool_.Allocate());
  FIXLogout logout(logout_buffer);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(logout_buffer);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
  pool_.Free(logout_buffer.Data());
}

TEST_F(FIXMessageTest, TestHeartbeat) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  FIXBuffer logon_buffer(pool_.Allocate());
  FIXLogon logon(logon_buffer);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(logon_buffer);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);
  pool_.Free(logon_buffer.Data());

  // --- Step 2: Heartbeat ---
  FIXBuffer heartbeat_buffer(pool_.Allocate());
  FIXHeartbeat heartbeat(heartbeat_buffer);
  heartbeat.Serialize();
  std::string heartbeatResponse = SendMessageAndGetResponse(heartbeat_buffer);
  std::cerr << "Heartbeat Response: " << heartbeatResponse << "\n";
  ASSERT_GE(heartbeatResponse.size(), 1);
  pool_.Free(heartbeat_buffer.Data());

  // --- Step 3: Logout ---
  FIXBuffer logout_buffer(pool_.Allocate());
  FIXLogout logout(logout_buffer);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(logout_buffer);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
  pool_.Free(logout_buffer.Data());
}

TEST_F(FIXMessageTest, TestOrderCancelRequest) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  FIXBuffer logon_buffer(pool_.Allocate());
  FIXLogon logon(logon_buffer);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(logon_buffer);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);
  pool_.Free(logon_buffer.Data());

  // --- Step 2: New Order Single ---
  FIXBuffer new_order_buffer(pool_.Allocate());
  SampleNewOrder sample_new_order{};
  FIXNewOrderSingle newOrder(new_order_buffer, sample_new_order);
  newOrder.Serialize();
  std::string newOrderResponse = SendMessageAndGetResponse(new_order_buffer);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);
  pool_.Free(new_order_buffer.Data());

  // --- Step 3: Order Cancel Request ---
  FIXBuffer cancel_buffer(pool_.Allocate());
  SampleCancelOrder sample_cancel_order{};
  FIXOrderCancelRequest cancelRequest(cancel_buffer, sample_cancel_order);
  cancelRequest.Serialize();
  std::string cancelRequestResponse = SendMessageAndGetResponse(cancel_buffer);
  std::cerr << "OrderCancelRequest Response: " << cancelRequestResponse << "\n";
  ASSERT_GE(cancelRequestResponse.size(), 1);
  pool_.Free(cancel_buffer.Data());

  // --- Step 4: Logout ---
  FIXBuffer logout_buffer(pool_.Allocate());
  FIXLogout logout(logout_buffer);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(logout_buffer);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
  pool_.Free(logout_buffer.Data());
}

TEST_F(FIXMessageTest, TestOrderCancelReplaceRequest) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  FIXBuffer logon_buffer(pool_.Allocate());
  FIXLogon logon(logon_buffer);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(logon_buffer);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);
  pool_.Free(logon_buffer.Data());

  // --- Step 2: New Order Single ---
  FIXBuffer new_order_buffer(pool_.Allocate());
  SampleNewOrder sample_new_order{};
  FIXNewOrderSingle newOrder(new_order_buffer, sample_new_order);
  newOrder.Serialize();
  std::string newOrderResponse = SendMessageAndGetResponse(new_order_buffer);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);
  pool_.Free(new_order_buffer.Data());

  // --- Step 3: Order Cancel Replace Request ---
  FIXBuffer replace_buffer(pool_.Allocate());
  SampleCancelReplaceOrder sample_cancel_replace_order{};
  FIXOrderCancelReplaceRequest replaceRequest(replace_buffer,
                                              sample_cancel_replace_order);
  replaceRequest.Serialize();
  std::string replaceRequestResponse =
      SendMessageAndGetResponse(replace_buffer);
  std::cerr << "OrderCancelReplaceRequest Response: " << replaceRequestResponse
            << "\n";
  ASSERT_GE(replaceRequestResponse.size(), 1);
  pool_.Free(replace_buffer.Data());

  // --- Step 4: Logout ---
  FIXBuffer logout_buffer(pool_.Allocate());
  FIXLogout logout(logout_buffer);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(logout_buffer);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
  pool_.Free(logout_buffer.Data());
}

TEST_F(FIXMessageTest, TestTestRequest) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  FIXBuffer logon_buffer(pool_.Allocate());
  FIXLogon logon(logon_buffer);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(logon_buffer);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);
  pool_.Free(logon_buffer.Data());

  // --- Step 2: Test Request ---
  FIXBuffer test_request_buffer(pool_.Allocate());
  FIXTestRequest testRequest(test_request_buffer);
  testRequest.Serialize();
  std::string testRequestResponse =
      SendMessageAndGetResponse(test_request_buffer);
  std::cerr << "TestRequest Response: " << testRequestResponse << "\n";
  ASSERT_GE(testRequestResponse.size(), 1);
  pool_.Free(test_request_buffer.Data());

  // --- Step 3: Logout ---
  FIXBuffer logout_buffer(pool_.Allocate());
  FIXLogout logout(logout_buffer);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(logout_buffer);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
  pool_.Free(logout_buffer.Data());
}