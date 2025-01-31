import typing
from google.protobuf.message import Message

from horus.pb.rpc.message_pb2 import RpcMessage
from horus.rpc.websocket import WebSocket


class BaseRpcClient:
    """
    Base class for all generated RPC clients.
    """

    service_full_name: str
    service_id: int

    @property
    def service_name(self) -> str:
        return self.service_full_name[self.service_full_name.rfind(".") + 1 :]

    def __init__(self, ws: WebSocket) -> None:
        self._ws = ws

    async def _send_rpc(
        self,
        method_id: int,
        request: Message,
        response: typing.Optional[Message] = None,
    ) -> None:
        """
        Sends the given RPC `request` to the endpoint. If `response` is not `None`, awaits a
        response which will be serialized to `response`.
        """
        ws = self._ws

        message = RpcMessage(
            version=RpcMessage.VERSION_ONE,
            method_id=method_id,
            service_id=self.service_id,
            message_bytes=request.SerializeToString(),
        )

        await ws.send(message, response)
