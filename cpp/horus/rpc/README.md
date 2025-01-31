# Horus RPC

Horus services communicate using RPC via WebSockets; the SDK similarly uses that
same RPC system to communicate with Horus services.

RPC is implemented using the
[`horus::pb::RpcMessage` type](../../../proto/horus/pb/rpc/message.proto).
Client and handler classes are automatically generated based on Protobuf
services.

Due to the asynchronous nature of RPCs, both clients and handlers are
implemented using [`Future`](../future/README.md)s.

## WebSockets

The WebSocket implementation of `RpcEndpoint` (in [`ws.cpp`](ws.cpp)) currently
uses [IXWebSocket](https://machinezone.github.io/IXWebSocket/), as it has a
small footprint and simple API.

However, it is not a great match for Horus and will be replaced with something
else like [libwebsockets](https://libwebsockets.org),
[wslay](https://github.com/tatsuhiro-t/wslay), or
[Asio](https://think-async.com/Asio/) in the future due to:

- Lack of Autosar compliance.

- Use of threads for all connections which prevent direct integration with the
  event loop, instead connecting these threads to the event loop with channels.

- Lack of per-request retry policy.
