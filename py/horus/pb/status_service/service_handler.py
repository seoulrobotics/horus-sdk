import abc as _abc
from ...rpc.base_handler import BaseRpcHandler, RpcMessage, WebSocket

from ..status_service.service_pb2 import GetVersionRequest, GetVersionResponse

class StatusServiceHandler(BaseRpcHandler, metaclass=_abc.ABCMeta):
    """
    Service running in every binary, returning status information.
    """

    service_full_name = "horus.pb.StatusService"
    service_id = 15

    def __init__(self, ws: WebSocket):
        super().__init__(ws, {
            1: (GetVersionRequest, GetVersionResponse, self.get_version),
        })

    @_abc.abstractmethod
    async def get_version(self, request: GetVersionRequest) -> GetVersionResponse:
        """
        Returns the current version of horus.
        """
        raise NotImplementedError
