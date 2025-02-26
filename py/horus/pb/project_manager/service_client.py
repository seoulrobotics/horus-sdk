from ...rpc.base_client import BaseRpcClient

from ..project_manager.service_pb2 import GetHealthStatusRequest, GetHealthStatusResponse

class ProjectManagerServiceClient(BaseRpcClient):
    """
    Service which stores configuration for all other services and coordinates updates to this configuration.
    """

    service_full_name = "horus.pb.ProjectManagerService"
    service_id = 3

    async def get_health_status(self, request: GetHealthStatusRequest) -> GetHealthStatusResponse:
        """
        Service which stores configuration for all other services and coordinates updates to this configuration.
        """
        if not isinstance(request, GetHealthStatusRequest):
            raise TypeError("request must be a GetHealthStatusRequest")

        response = GetHealthStatusResponse()
        await self._send_rpc(38, request, response)
        return response
