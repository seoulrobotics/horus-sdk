from ...rpc.base_client import BaseRpcClient

from ..detection_service.detection_pb2 import DetectionEvent
from ..rpc_pb2 import DefaultSubscribeRequest, DefaultSubscribeResponse, DefaultUnsubscribeRequest, DefaultUnsubscribeResponse

class DetectionServiceClient(BaseRpcClient):
    """
    Service for receiving pre-processed foreground points and sending detection results.
    """

    service_full_name = "horus.pb.DetectionService"
    service_id = 7

    async def subscribe(self, request: DefaultSubscribeRequest) -> DefaultSubscribeResponse:
        """
        Service for receiving pre-processed foreground points and sending detection results.
        """
        if not isinstance(request, DefaultSubscribeRequest):
            raise TypeError("request must be a DefaultSubscribeRequest")

        response = DefaultSubscribeResponse()
        await self._send_rpc(2, request, response)
        return response

    async def unsubscribe(self, request: DefaultUnsubscribeRequest) -> DefaultUnsubscribeResponse:
        """
        Service for receiving pre-processed foreground points and sending detection results.
        """
        if not isinstance(request, DefaultUnsubscribeRequest):
            raise TypeError("request must be a DefaultUnsubscribeRequest")

        response = DefaultUnsubscribeResponse()
        await self._send_rpc(3, request, response)
        return response

class DetectionSubscriberServiceClient(BaseRpcClient):
    """
    Service exposed by clients of the `DetectionService` that wish to receive updates when new detection results are processed.
    """

    service_full_name = "horus.pb.DetectionSubscriberService"
    service_id = 12

    async def broadcast_detection(self, request: DetectionEvent) -> None:
        """
        Service exposed by clients of the `DetectionService` that wish to receive updates when new detection results are processed.
        """
        if not isinstance(request, DetectionEvent):
            raise TypeError("request must be a DetectionEvent")

        await self._send_rpc(1, request)
