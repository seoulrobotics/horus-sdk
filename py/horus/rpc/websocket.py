import asyncio, logging, typing, websockets.exceptions
from datetime import timedelta
from google.protobuf.message import Message
from websockets.asyncio.client import ClientConnection, connect

from horus.helpers.cached_awaitable import CachedAwaitable
from horus.pb.rpc.message_pb2 import RpcMessage

REQUEST_ID_ONE_WAY = 0
"""
Value of `RpcMessage.request_id` for one-way RPC requests.

Other values of `request_id` indicate a two-way RPC request or an RPC response (determined via
`RpcMessage.method_id`).
"""
METHOD_ID_RESPONSE = 0
"""
Value of `RpcMessage.method_id` for two-way RPC responses.

Other values of `method_id` indicate a one-way RPC request.
"""


class WebSocket:
    """
    Class which implements communication between Horus RPC services via WebSockets.
    """

    _active = True
    "Whether the WebSocket is active; False after calling `disconnect()`."

    _connection: typing.Optional[CachedAwaitable[ClientConnection]] = None
    "The current connection, or None if currently disconnected."

    _available_request_ids: typing.List[int]
    "List of request IDs assigned previously allocated and then freed (by receiving a response)."

    _pending_responses: typing.Dict[int, typing.Tuple[Message, "asyncio.Future[None]"]]
    "Map from request ID to response message and future awaiting the response."

    _services: typing.Dict[int, typing.Callable[[RpcMessage], typing.Awaitable[None]]]
    "Map from service ID to message handler."

    def __init__(
        self,
        url: str,
        logger: logging.Logger,
        on_connected: typing.Callable[[], None] = lambda: None,
    ) -> None:
        self._available_request_ids = []
        self._pending_responses = {}
        self._services = {}

        self._url = url
        self._logger = logger
        self._recv_task = asyncio.create_task(self._receiver())
        self._on_connected = on_connected

    async def disconnect(self) -> None:
        """
        Disconnects from the remote endpoint.

        This is the only way to definitely close the connection, and must be done explicitly by the
        user. Other disconnections (as handled by `_mark_disconnected()`) are unintentional, and
        will be recovered from by reconnecting.
        """
        self._active = False

        if self._connection is None:
            return

        if isinstance(self._connection, ClientConnection):
            await self._connection.close()

        self._mark_disconnected(websockets.exceptions.ConnectionClosedOK(None, None))

    async def send(
        self, message: RpcMessage, response: typing.Optional[Message]
    ) -> None:
        """
        Sends `message` to the endpoint; if `response` is not `None`, a request ID will be assigned
        to `message` and a response will be awaited, then written to `response`.
        """
        connection = await self._connect()

        # Obtain request ID.
        if response is not None:
            if self._available_request_ids:
                req_id = self._available_request_ids.pop()
            else:
                req_id = len(self._available_request_ids) + 1

            message.request_id = req_id
            response_future: asyncio.Future[None] = asyncio.Future()

            self._pending_responses[req_id] = (response, response_future)

        # Send message.
        try:
            await connection.send(message.SerializeToString())
        except websockets.exceptions.ConnectionClosed as e:
            self._mark_disconnected(e)
            raise

        # Await response.
        if response is None:
            return

        assert response_future  # type: ignore

        await response_future

    def add_handler(
        self,
        service_id: int,
        handle: typing.Callable[[RpcMessage], typing.Awaitable[None]],
    ) -> None:
        """
        Indicates that `handle()` should be called whenever a request message with a matching
        `service_id` is received.

        If a handler for the same service was already registered, it will be replaced.
        `remove_handler(service_id)` can later be called to stop calling the handler.
        """
        self._services[service_id] = handle

    def remove_handler(self, service_id: int) -> None:
        """Stops calling the registered with `add_handler(service_id, ...)`."""
        self._services.pop(service_id)

    def log_error(self, error: str) -> None:
        self._logger.error(error)

    async def send_error(self, message: RpcMessage, error: str) -> None:
        """
        Sends an error encountered while processing `message`.

        If `message` is a two-way request, we will reply with an error. If it isn't, there is no
        way for us to notify the endpoint of an error for that particular message, so we instead
        log an error locally.
        """
        if message.request_id == REQUEST_ID_ONE_WAY or self._connection is None:
            self.log_error(error)
        else:
            err_message = RpcMessage(
                version=RpcMessage.VERSION_ONE,
                error=error,
                request_id=message.request_id,
            )

            try:
                await (await self._connect()).send(err_message.SerializeToString())
            except websockets.exceptions.ConnectionClosed as e:
                self.log_error(error)
                self._mark_disconnected(e)
            except Exception as e:
                self.log_error(
                    f"failed to send RPC error {error!r} to {self._url}: {e}"
                )

    async def _connect(self) -> ClientConnection:
        """
        Connects to the remote endpoint; throws if the connection cannot be established. On
        success, further calls will reuse the same connection.
        """
        if self._connection is None:

            async def connect_with_callback() -> ClientConnection:
                ws = await connect(
                    self._url,
                    logger=self._logger.getChild("websockets"),
                    max_size=10 << 20,  # 10 MB
                )

                if self._connection is cached:
                    self._on_connected()

                return ws

            cached = CachedAwaitable(connect_with_callback())
            self._connection = cached

        try:
            result = await self._connection.get()
        except Exception as e:
            self._mark_disconnected(e)
            raise

        return result

    def _mark_disconnected(self, err: Exception) -> None:
        """Marks the WebSocket as disconnected, resetting its connection state."""
        self._connection = None

        for _, future in self._pending_responses.values():
            future.set_exception(err)

        self._pending_responses.clear()
        self._available_request_ids.clear()

    async def _receiver(self) -> None:
        """
        Function which receives and processes requests and responses from the endpoint. Also
        handles reconnections.

        This should only be called once to be assigned to `self._recv`.
        """
        INITIAL_BACKOFF = timedelta(milliseconds=500)
        MAX_BACKOFF = timedelta(seconds=4)

        message = RpcMessage()
        backoff = INITIAL_BACKOFF

        # Keep reconnecting until the user explicitly requests the definite end of the connection
        # with `disconnect()`.
        while self._active:
            try:
                connection = await self._connect()
            except Exception as e:
                self._logger.warning(
                    f"failed to connect to {self._url}, retrying in {backoff.total_seconds()}s: {e}"
                )

                await asyncio.sleep(backoff.total_seconds())

                backoff = min(backoff * 2, MAX_BACKOFF)

                continue

            backoff = INITIAL_BACKOFF

            try:
                async for data in connection:
                    if not isinstance(data, bytes):
                        self.log_error(f"received {type(data)}, but expected bytes")
                        continue

                    try:
                        message.ParseFromString(data)
                    except Exception as e:
                        self.log_error(
                            f"failed to deserialize request as RpcMessage: {e}"
                        )
                        continue

                    if message.cancel:
                        # We can't report cancellations right now so we just ignore them.
                        continue

                    if message.method_id == METHOD_ID_RESPONSE:
                        await self._handle_res(message)
                    else:
                        await self._handle_req(message)

            except websockets.exceptions.ConnectionClosed as e:
                self._mark_disconnected(e)

    async def _handle_req(self, message: RpcMessage) -> None:
        """
        Handles the given request message, forwarding it to the relevant handler registered with
        `add_handler(message.service_id, ...)`.
        """
        service = self._services.get(message.service_id)

        if service is None:
            await self.send_error(
                message, f"no service found for id {message.service_id}"
            )
            return

        await service(message)

    async def _handle_res(self, message: RpcMessage) -> None:
        """
        Handles the given response message, completing the future which was awaiting it.
        """
        response_and_future = self._pending_responses.pop(message.request_id, None)

        if response_and_future is None:
            await self.send_error(
                message, f"no active request found for id {message.request_id}"
            )
            return

        self._available_request_ids.append(message.request_id)

        response, future = response_and_future

        if message.error:
            future.set_exception(RuntimeError(message.error))
            return

        try:
            response.ParseFromString(message.message_bytes)
        except Exception as e:
            future.set_exception(e)

        future.set_result(None)
