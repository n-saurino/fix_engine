#include <gtest/gtest.h>

#include "FIXMessage.hpp"
#include "FIXNetworkHandler.hpp"
#include "Order.hpp"
#include "SampleOrders.hpp"
#include "unordered_map"

class FIXMessageTest : public ::testing::Test {
 protected:
  FIXNetworkHandler handler_;
  FIXBuffer fix_buffer_;
  SampleNewOrder orig_order_;
  SampleCancelOrder cancel_order_;
  SampleCancelReplaceOrder cancel_replace_order_;

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
  fix_buffer_.Reset();
  FIXLogon logon(fix_buffer_);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);

  // --- Step 2: New Order Single ---
  fix_buffer_.Reset();
  SampleNewOrder sample_new_order;

  FIXNewOrderSingle newOrder(fix_buffer_, sample_new_order);
  newOrder.Serialize();  // This calls SerializeDerived() internally.
  std::string newOrderResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);

  // --- Step 3: Logout ---
  fix_buffer_.Reset();
  FIXLogout logout(fix_buffer_);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
}

TEST_F(FIXMessageTest, TestHeartbeat) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  fix_buffer_.Reset();
  FIXLogon logon(fix_buffer_);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);

  // --- Step 2: Heartbeat ---
  fix_buffer_.Reset();
  FIXHeartbeat heartbeat(fix_buffer_);
  heartbeat.Serialize();
  std::string heartbeatResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Heartbeat Response: " << heartbeatResponse << "\n";
  ASSERT_GE(heartbeatResponse.size(), 1);

  // --- Step 3: Logout ---
  fix_buffer_.Reset();
  FIXLogout logout(fix_buffer_);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
}

TEST_F(FIXMessageTest, TestOrderCancelRequest) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  fix_buffer_.Reset();
  FIXLogon logon(fix_buffer_);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);

  // --- Step 2: New Order Single ---
  fix_buffer_.Reset();
  SampleNewOrder sample_new_order{};

  FIXNewOrderSingle newOrder(fix_buffer_, sample_new_order);
  newOrder.Serialize();  // This calls SerializeDerived() internally.
  std::string newOrderResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);

  // --- Step 2: Order Cancel Request ---
  fix_buffer_.Reset();
  SampleCancelOrder sample_cancel_order{};

  FIXOrderCancelRequest cancelRequest(fix_buffer_, sample_cancel_order);
  cancelRequest.Serialize();
  std::string cancelRequestResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "OrderCancelRequest Response: " << cancelRequestResponse << "\n";
  ASSERT_GE(cancelRequestResponse.size(), 1);

  // --- Step 3: Logout ---
  fix_buffer_.Reset();
  FIXLogout logout(fix_buffer_);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
}

TEST_F(FIXMessageTest, TestOrderCancelReplaceRequest) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  fix_buffer_.Reset();
  FIXLogon logon(fix_buffer_);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);

  // --- Step 2: New Order Single ---
  fix_buffer_.Reset();

  SampleNewOrder sample_new_order_{};

  FIXNewOrderSingle newOrder(fix_buffer_, sample_new_order_);
  newOrder.Serialize();  // This calls SerializeDerived() internally.
  std::string newOrderResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);

  // --- Step 2: Order Cancel Replace Request ---
  fix_buffer_.Reset();

  SampleCancelReplaceOrder sample_cancel_replace_order{};

  FIXOrderCancelReplaceRequest replaceRequest(fix_buffer_,
                                              sample_cancel_replace_order);
  replaceRequest.Serialize();
  std::string replaceRequestResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "OrderCancelReplaceRequest Response: " << replaceRequestResponse
            << "\n";
  ASSERT_GE(replaceRequestResponse.size(), 1);

  // --- Step 3: Logout ---
  fix_buffer_.Reset();
  FIXLogout logout(fix_buffer_);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
}

TEST_F(FIXMessageTest, TestTestRequest) {
  // --- Step 1: Logon ---
  FIXMessageBase::seq_num_generator_ = 1;
  fix_buffer_.Reset();
  FIXLogon logon(fix_buffer_);
  logon.Serialize();
  std::string logonResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Logon Response: " << logonResponse << "\n";
  ASSERT_GE(logonResponse.size(), 1);

  // --- Step 2: Test Request ---
  fix_buffer_.Reset();
  FIXTestRequest testRequest(fix_buffer_);
  testRequest.Serialize();
  std::string testRequestResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "TestRequest Response: " << testRequestResponse << "\n";
  ASSERT_GE(testRequestResponse.size(), 1);

  // --- Step 3: Logout ---
  fix_buffer_.Reset();
  FIXLogout logout(fix_buffer_);
  logout.Serialize();
  std::string logoutResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "Logout Response: " << logoutResponse << "\n";
  ASSERT_GE(logoutResponse.size(), 1);
}
int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
