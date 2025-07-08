from ...rpc.base_client import BaseRpcClient

from ..detection_service.detection_pb2 import DetectionEvent
from ..rpc_pb2 import DefaultSubscribeRequest, DefaultSubscribeResponse, DefaultUnsubscribeRequest, DefaultUnsubscribeResponse

class DetectionMergerServiceClient(BaseRpcClient):
    """
    Service for merging detection results from multiple sources.
    """

    service_full_name = "horus.pb.DetectionMergerService"
    service_id = 11

    async def subscribe(self, request: DefaultSubscribeRequest) -> DefaultSubscribeResponse:
        """
        Service for merging detection results from multiple sources.
        """
        if not isinstance(request, DefaultSubscribeRequest):
            raise TypeError("request must be a DefaultSubscribeRequest")

        response = DefaultSubscribeResponse()
        await self._send_rpc(1, request, response)
        return response

    async def unsubscribe(self, request: DefaultUnsubscribeRequest) -> DefaultUnsubscribeResponse:
        """
        Service for merging detection results from multiple sources.
        """
        if not isinstance(request, DefaultUnsubscribeRequest):
            raise TypeError("request must be a DefaultUnsubscribeRequest")

        response = DefaultUnsubscribeResponse()
        await self._send_rpc(2, request, response)
        return response

class DetectionMergerSubscriberServiceClient(BaseRpcClient):
    """
    Service for receiving merged detection results.
    """

    service_full_name = "horus.pb.DetectionMergerSubscriberService"
    service_id = 16

    async def broadcast_detection(self, request: DetectionEvent) -> None:
        """
        Service for receiving merged detection results.
        """
        if not isinstance(request, DetectionEvent):
            raise TypeError("request must be a DetectionEvent")

        await self._send_rpc(1, request)
