import abc as _abc
from ...rpc.base_handler import BaseRpcHandler, RpcMessage, WebSocket

from ..project_manager.service_pb2 import GetHealthStatusRequest, GetHealthStatusResponse

class ProjectManagerServiceHandler(BaseRpcHandler, metaclass=_abc.ABCMeta):
    """
    Service which stores configuration for all other services and coordinates updates to this configuration.
    """

    service_full_name = "horus.pb.ProjectManagerService"
    service_id = 3

    def __init__(self, ws: WebSocket):
        super().__init__(ws, {
            38: (GetHealthStatusRequest, GetHealthStatusResponse, self.get_health_status),
        })

    @_abc.abstractmethod
    async def get_health_status(self, request: GetHealthStatusRequest) -> GetHealthStatusResponse:
        """
        Provides horus health indicators.
        """
        raise NotImplementedError
