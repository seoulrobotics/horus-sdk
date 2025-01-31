import typing
from google.protobuf.message import Message

from horus.pb.rpc.message_pb2 import RpcMessage
from horus.rpc.websocket import REQUEST_ID_ONE_WAY, WebSocket

_M = typing.TypeVar("_M", bound=Message)

_Handler = typing.Tuple[
    typing.Type[_M],  # Request.
    typing.Optional[typing.Type[_M]],  # Response.
    typing.Callable[[_M], typing.Awaitable[typing.Optional[_M]]],  # Handler.
]


class BaseRpcHandler:
    """
    Base class for all generated RPC handler classes.
    """

    service_full_name: str
    service_id: int

    @property
    def service_name(self) -> str:
        return self.service_full_name[self.service_full_name.rfind(".") + 1 :]

    def __init__(
        self, ws: WebSocket, handlers: typing.Dict[int, _Handler[typing.Any]]
    ) -> None:
        """
        Initializes the handler, setting up `ws` such that requests are forwarded to their
        corresponding handler in `handlers` keyed by `RpcMessage.method_id`.
        """
        self._ws = ws
        self._handlers = handlers

        self._ws.add_handler(self.service_id, self._handle_req)

    def __del__(self) -> None:
        self._ws.remove_handler(self.service_id)

    async def _handle_req(self, message: RpcMessage) -> None:
        """
        Calls the handler for the given message, handling errors and sending a response if the
        handler returns a value.
        """
        handler = self._handlers.get(message.method_id)
        ws = self._ws

        if handler is None:
            # No handler found for this request, but the SDK may simply not care about it, which
            # isn't an error.
            if message.request_id != REQUEST_ID_ONE_WAY:
                await ws.send_error(
                    message, f"no handler found for method {message.method_id}"
                )
            return

        req_type, res_type, handle = handler
        req = req_type()

        try:
            req.ParseFromString(message.message_bytes)
        except Exception as e:
            await ws.send_error(
                message,
                f"failed to deserialize request bytes as {req.DESCRIPTOR.name}: {e}",
            )
            return

        try:
            res = await handle(req)
        except Exception as e:
            await ws.send_error(message, f"handler failed: {e}")
            return

        if res is None:
            if res_type is not None:
                await ws.send_error(message, f"handler failed to return a response")
                return

            if message.request_id != REQUEST_ID_ONE_WAY:
                await ws.send_error(
                    message, f"request has request_id, but handler expects no response"
                )
                return
        else:
            if res_type is None:
                await ws.send_error(
                    message, f"handler unexpectedly returned a response"
                )
                return
            if not isinstance(res, res_type):
                await ws.send_error(
                    message, f"handler returned an invalid response of type {type(res)}"
                )
                return
            if message.request_id == REQUEST_ID_ONE_WAY:
                await ws.send_error(
                    message,
                    f"request has no request_id, but handler expected to respond",
                )
                return

            res_message = RpcMessage(
                version=RpcMessage.VERSION_ONE,
                message_bytes=res.SerializeToString(),
                request_id=message.request_id,
            )

            await ws.send(res_message, response=None)
