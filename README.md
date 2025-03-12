# Low-latency FIX client engine in C++ 
## Summary
- View basic latency dashboard here (using Prometheus and Grafana): https://fixengine.grafana.net/public-dashboards/f331d66f32ae40a1b7819796679c8bd0
- Articles (Coming soon...)

## Components
- Custom FIX Engine (in progress)
- Benchmarking Pipeline: Google benchmark, Prometheus for capture and serving, Grafana for visualization
- Testing Framework: Google test and ctest

## Features
- Protocol: FIX protocol due to popularity, will move to FIX SBE or similar binary protocols in future enhancements 
- Combo of C++ and C
- Using POSIX raw sockets

## FIX Engine Architecture (IN PROGRESS...)
### Classes
- FIXNetworkHandler: Handles non-blocking TCP sockets (epoll/io_uring)
- FIXSessionManager: Manages session state (logon, heartbeats, reconnections)
- FIXMessageParser: Decodes incoming FIX messages
    - SBE Documentation (for future enhancements): https://github.com/FIXTradingCommunity/fix-simple-binary-encoding/tree/master/v2-0-RC3/doc
- FIXMessage and CRTP Derived FIXMessages: Encodes outgoing FIX messages
- FIXMessageQueue: Lock-free queue for inter-thread communication
- BenchmarkLogger: Google Benchmark custom logger to handle latency benchmarking
- FIXDispatcher: Dispatches FIX messages to application handlers
