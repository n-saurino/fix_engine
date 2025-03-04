#include "FIXMessageParser.hpp"

/*
Logon (A): Establishes a session between two FIX engines.
 - Example: https://www.onixs.biz/fix-dictionary/4.4/msgtype_a_65.html
8=FIX.4.4^9=102^35=A^49=BuySide^56=SellSide^34=1^52=20190605-11:40:30.392^98=0^108=30^141=Y^553=Username^554=Password^10=104^

Logout (5): Terminates a session.
 - Example: https://www.onixs.biz/fix-dictionary/4.4/msgType_5_5.html
8=FIX.4.4^9=84^35=5^49=SellSide^56=BuySide^34=3^52=20190606-09:33:44.882^58=Logout
acknowledgement^10=055^

Heartbeat (0): Ensures the connection is still active.
 - Example: https://www.onixs.biz/fix-dictionary/4.4/msgType_0_0.html
8=FIX.4.4^9=58^35=0^49=BuySide^56=SellSide^34=3^52=20190605-12:29:20.259^10=172^

NewOrderSingle (D): Submits a new order.
 - Example: https://www.onixs.biz/fix-dictionary/4.4/msgType_D_68.html

ExecutionReport (8): Provides information about the status of an order.
OrderCancelRequest (F): Requests the cancellation of an existing order.
OrderCancelReplaceRequest (G): Modifies an existing order.
Reject (3): Indicates that a message cannot be processed.
BusinessMessageReject (j): Rejects an application-level message.
TestRequest (1): Requests a heartbeat from the counterparty.
*/

FIXMessageParser::FIXMessageParser(/* args */) = default;

void FIXMessageParser::Parse() {
  const int kBufferSize{256};

  std::array<char, kBufferSize> buffer{
      "8=FIX.4.4\x01 9=102\x01 35=A\x0149=BuySide\x0156=SellSide\x0134=1\x0152="
      "20190605-11:40:30.392\x0198=0\x01108=30\x01141=Y\x01553=Username\x01554="
      "Password\x0110="
      "104\x01\0"};

  size_t message_len{strnlen(buffer.data(), kBufferSize)};
  std::cout << message_len << "\n";

  for (size_t i{}; i < message_len; ++i) {
    int ascii{static_cast<int>(buffer[i])};

    if (ascii == 0) {
      break;
    }
    if (ascii == 1) {
      std::cout << " | ";
    } else {
      std::cout << buffer[i];
    }
  }

  std::cout << "\n";
}