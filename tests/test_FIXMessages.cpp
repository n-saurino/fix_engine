#include <gtest/gtest.h>

#include "FIXMessage.hpp"
#include "FIXNetworkHandler.hpp"
#include "Order.hpp"
#include "unordered_map"

class FIXMessageTest : public ::testing::Test {
 protected:
  FIXNetworkHandler handler_;
  FIXBuffer fix_buffer_;
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
    char buffer[1024];
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
  Order origOrder;
  // Populate the test order.
  std::strncpy(origOrder.cl_ord_id_, "CLORDID001",
               sizeof(origOrder.cl_ord_id_));
  // For a new order, orig_cl_ord_id_ may be empty.
  std::strncpy(origOrder.orig_cl_ord_id_, "",
               sizeof(origOrder.orig_cl_ord_id_));
  std::strncpy(origOrder.symbol_, "NVDA", sizeof(origOrder.symbol_));
  origOrder.side_ = 1;         // '1' for Buy.
  origOrder.order_qty_ = 100;  // Order quantity.
  origOrder.ord_type_ = 2;     // '2' for Limit order.
  origOrder.price_ = 99.99;    // Limit price.
  std::strncpy(origOrder.transact_time_, "20250306-12:34:56.000000",
               sizeof(origOrder.transact_time_));

  FIXNewOrderSingle newOrder(fix_buffer_, origOrder);
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
  Order origOrder;
  // Populate the test order.
  std::strncpy(origOrder.cl_ord_id_, "CLORDID001",
               sizeof(origOrder.cl_ord_id_));
  // For a new order, orig_cl_ord_id_ may be empty.
  std::strncpy(origOrder.orig_cl_ord_id_, "",
               sizeof(origOrder.orig_cl_ord_id_));
  std::strncpy(origOrder.symbol_, "NVDA", sizeof(origOrder.symbol_));
  origOrder.side_ = 1;         // '1' for Buy.
  origOrder.order_qty_ = 100;  // Order quantity.
  origOrder.ord_type_ = 2;     // '2' for Limit order.
  origOrder.price_ = 99.99;    // Limit price.
  std::strncpy(origOrder.transact_time_, "20250306-12:34:56.000000",
               sizeof(origOrder.transact_time_));

  FIXNewOrderSingle newOrder(fix_buffer_, origOrder);
  newOrder.Serialize();  // This calls SerializeDerived() internally.
  std::string newOrderResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);

  // --- Step 2: Order Cancel Request ---
  fix_buffer_.Reset();
  Order testOrder;
  // Populate the test order.
  std::strncpy(testOrder.cl_ord_id_, "CLORDID002",
               sizeof(testOrder.cl_ord_id_));
  std::strncpy(testOrder.orig_cl_ord_id_, "CLORDID001",
               sizeof(testOrder.orig_cl_ord_id_));
  std::strncpy(testOrder.symbol_, "NVDA", sizeof(testOrder.symbol_));
  testOrder.side_ = 1;         // '1' for Buy.
  testOrder.order_qty_ = 100;  // Order quantity.

  FIXOrderCancelRequest cancelRequest(fix_buffer_, testOrder);
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
  Order origOrder;
  // Populate the test order.
  std::strncpy(origOrder.cl_ord_id_, "CLORDID001",
               sizeof(origOrder.cl_ord_id_));
  // For a new order, orig_cl_ord_id_ may be empty.
  std::strncpy(origOrder.orig_cl_ord_id_, "",
               sizeof(origOrder.orig_cl_ord_id_));
  std::strncpy(origOrder.symbol_, "NVDA", sizeof(origOrder.symbol_));
  origOrder.side_ = 1;         // '1' for Buy.
  origOrder.order_qty_ = 100;  // Order quantity.
  origOrder.ord_type_ = 2;     // '2' for Limit order.
  origOrder.price_ = 99.99;    // Limit price.
  std::strncpy(origOrder.transact_time_, "20250306-12:34:56.000000",
               sizeof(origOrder.transact_time_));

  FIXNewOrderSingle newOrder(fix_buffer_, origOrder);
  newOrder.Serialize();  // This calls SerializeDerived() internally.
  std::string newOrderResponse = SendMessageAndGetResponse(fix_buffer_);
  std::cerr << "NewOrderSingle Response: " << newOrderResponse << "\n";
  ASSERT_GE(newOrderResponse.size(), 1);

  // --- Step 2: Order Cancel Replace Request ---
  fix_buffer_.Reset();
  Order testOrder;
  // Populate the test order.
  std::strncpy(testOrder.cl_ord_id_, "CLORDID003",
               sizeof(testOrder.cl_ord_id_));
  std::strncpy(testOrder.orig_cl_ord_id_, "CLORDID001",
               sizeof(testOrder.orig_cl_ord_id_));
  std::strncpy(testOrder.symbol_, "NVDA", sizeof(testOrder.symbol_));
  testOrder.side_ = 1;         // '1' for Buy.
  testOrder.order_qty_ = 200;  // New order quantity.
  testOrder.ord_type_ = 2;     // '2' for Limit order.
  testOrder.price_ = 98.99;    // New limit price.

  FIXOrderCancelReplaceRequest replaceRequest(fix_buffer_, testOrder);
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
