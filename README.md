# Concurrent Server in C++ 
## Components
- Custom FIX Engine (in progress)

## Features
- Protocol: Must be binary protocol. Considering FIX/SBE, Custom SBE, ITCH or OUCH
- Combo of C++ and C
- Using POSIX raw sockets

## FIX Engine Architecture
### Classes
- FIXClient: High-level interface for the FIX engine
- FIXNetworkHandler: Handles non-blocking TCP sockets (epoll/io_uring)
    - Needs to define logic for Client (Initiator) and for Server (Acceptor)
- FIXSessionManager: Manages session state (logon, heartbeats, reconnections)
- FIXMessageParser: Decodes incoming FIX messages
- FIXMessageBuilder: Encodes outgoing FIX messages
- FIXMessageQueue: Lock-free queue for inter-thread communication
- FIXLogger: Optimized logging for compliance & debugging
- FIXDispatcher: Dispatches FIX messages to application handlers
- OrderManager: Handles order lifecycle (New, Modify, Cancel, Execution Reports)