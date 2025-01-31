import asyncio, logging, typing, websockets.exceptions
from google.protobuf.message import Message

from horus.rpc.base_client import BaseRpcClient
from horus.rpc.base_handler import BaseRpcHandler
from horus.rpc.websocket import WebSocket

S = typing.TypeVar("S", bound=Message)
"Subscription request type."

U = typing.TypeVar("U", bound=Message)
"Unsubscription request type."

C = typing.TypeVar("C", bound=BaseRpcClient)
"Client type."

L = typing.TypeVar("L", bound=BaseRpcHandler)
"Listener type."


class ClientListenerPair(typing.Generic[C, L, S, U]):
    """
    A pair between an RPC client and a corresponding RPC listener handler.
    """

    _state: typing.Optional[typing.Tuple[WebSocket, C, L, S]] = None
    "Client/listener/subscription request tuple if connected/connecting."

    def __init__(
        self,
        url: str,
        logger: logging.Logger,
        client_cls: typing.Callable[[WebSocket], C],
        listener_cls: typing.Callable[[WebSocket, logging.Logger], L],
        subscribe: typing.Callable[[C, S], typing.Awaitable[Message]],
        unsubscribe: typing.Callable[[C, U], typing.Awaitable[Message]],
    ) -> None:
        self._url = url
        self._logger = logger
        self._client_cls = client_cls
        self._listener_cls = listener_cls
        self._subscribe = subscribe
        self._unsubscribe = unsubscribe

    @typing.overload
    async def set_subscribe_request(
        self, subscribe_request: None, unsubscribe_request: None
    ) -> None:
        """
        Updates the current subscription request to None, disconnecting.
        """

    @typing.overload
    async def set_subscribe_request(
        self, subscribe_request: S, unsubscribe_request: U
    ) -> typing.Tuple[C, L]:
        """
        Updates the current subscription request.

        If it is different from the previous request (or if the previous request was None), the
        client will re-subscribe (connecting first if necessary). Otherwise, does nothing.
        """

    async def set_subscribe_request(
        self,
        subscribe_request: typing.Optional[S],
        unsubscribe_request: typing.Optional[U],
    ) -> typing.Optional[typing.Tuple[C, L]]:
        if subscribe_request is None:
            assert unsubscribe_request is None

            await self.disconnect()

            return None
        else:
            assert unsubscribe_request is not None

            return await self.connect_and_subscribe(
                subscribe_request, unsubscribe_request
            )

    async def connect_and_subscribe(
        self, subscribe_request: S, unsubscribe_request: U
    ) -> typing.Tuple[C, L]:
        """Connects to the remote endpoint and returns the client/listener pair."""

        if self._state is None:
            # Start new connection.
            ws = WebSocket(self._url, self._logger, self._subscribe_on_connected)
            client = self._client_cls(ws)
            listener = self._listener_cls(ws, self._logger)
            prev_subscribe_request = None

            self._state = ws, client, listener, subscribe_request
        else:
            ws, client, listener, prev_subscribe_request = self._state

        if (
            prev_subscribe_request is not None
            and subscribe_request != prev_subscribe_request
        ):
            # Re-subscribe.
            self._state = ws, client, listener, subscribe_request

            try:
                await self._unsubscribe(client, unsubscribe_request)
                await self._subscribe(client, subscribe_request)

                self._logger.info(
                    f"re-subscribed to {client.service_name} on {self._url}"
                )
            except websockets.exceptions.ConnectionClosed:
                # `_on_connected()` will handle the reconnection.
                pass

        return client, listener

    async def disconnect(self) -> None:
        """Disconnects from the remote endpoint (if connected)."""

        if self._state is None:
            return

        ws, client, _, _ = self._state

        try:
            await ws.disconnect()
        except:
            pass

        self._state = None
        self._logger.info(f"unsubscribed {client.service_name} on {self._url}")

    def _subscribe_on_connected(self) -> None:
        """
        Called by the WebSocket when connected (incl. after reconnections).

        This will immediately subscribe to events.
        """

        assert self._state is not None

        _, client, _, request = self._state

        async def subscribe_in_bg() -> None:
            await self._subscribe(client, request)

            self._logger.info(f"subscribed to {client.service_name} on {self._url}")

        # Note that this method cannot be async, as the WebSocket would wait for it to complete
        # during its creation, which would prevent it from handling events, and notably the
        # reception of the subscription response, thus causing a deadlock.
        #
        # Instead we spawn a background task.
        asyncio.create_task(subscribe_in_bg())
