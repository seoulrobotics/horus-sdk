from horus.rpc.base_client import BaseRpcClient
from horus.rpc.websocket import WebSocket
import logging, typing

C = typing.TypeVar("C", bound=BaseRpcClient)
"Client type."


class Client(typing.Generic[C]):
    """
    A simple wrapper for RPC client, managing underlying websocket connection.

    Acts similarly to `ClientListenerPair`, but without managing subscribers.
    """

    _state: typing.Optional[typing.Tuple[WebSocket, C]] = None
    "Client request tuple if connected/connecting."

    def __init__(
        self,
        url: str,
        logger: logging.Logger,
        client_cls: typing.Callable[[WebSocket], C],
    ) -> None:
        self._url = url
        self._logger = logger
        self._client_cls = client_cls
        self._state = None

    async def connect(self) -> C:
        """Connects to the remote endpoint and returns the client."""

        if self._state is None:
            # Start new connection.
            ws = WebSocket(self._url, self._logger)
            client = self._client_cls(ws)

            self._state = ws, client
        else:
            ws, client = self._state

        return client

    async def disconnect(self) -> None:
        """Disconnects from the remote endpoint (if connected)."""

        if self._state is None:
            return

        ws, _ = self._state

        try:
            await ws.disconnect()
        except:
            pass

        self._state = None
        self._logger.info(f"disconnected service on {self._url}")
