from ...rpc.base_client import BaseRpcClient

from ..status_service.service_pb2 import GetVersionRequest, GetVersionResponse

class StatusServiceClient(BaseRpcClient):
    """
    Service running in every binary, returning status information.
    """

    service_full_name = "horus.pb.StatusService"
    service_id = 15

    async def get_version(self, request: GetVersionRequest) -> GetVersionResponse:
        """
        Service running in every binary, returning status information.
        """
        if not isinstance(request, GetVersionRequest):
            raise TypeError("request must be a GetVersionRequest")

        response = GetVersionResponse()
        await self._send_rpc(1, request, response)
        return response
