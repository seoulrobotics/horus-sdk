from ...rpc.base_client import BaseRpcClient

from ..notification_service.service_pb2 import LogMessageEvent, LogMessageRequest, ProfilingInfoEvent, SensorInfoEvent
from ..rpc_pb2 import DefaultSubscribeRequest, DefaultSubscribeResponse, DefaultUnsubscribeRequest, DefaultUnsubscribeResponse

class NotificationServiceClient(BaseRpcClient):
    """
    Service which receives notifications from services, storing and forwarding them.
    """

    service_full_name = "horus.pb.NotificationService"
    service_id = 2

    async def log_message(self, request: LogMessageRequest) -> None:
        """
        Service which receives notifications from services, storing and forwarding them.
        """
        if not isinstance(request, LogMessageRequest):
            raise TypeError("request must be a LogMessageRequest")

        await self._send_rpc(1, request)

    async def subscribe(self, request: DefaultSubscribeRequest) -> DefaultSubscribeResponse:
        """
        Service which receives notifications from services, storing and forwarding them.
        """
        if not isinstance(request, DefaultSubscribeRequest):
            raise TypeError("request must be a DefaultSubscribeRequest")

        response = DefaultSubscribeResponse()
        await self._send_rpc(3, request, response)
        return response

    async def unsubscribe(self, request: DefaultUnsubscribeRequest) -> DefaultUnsubscribeResponse:
        """
        Service which receives notifications from services, storing and forwarding them.
        """
        if not isinstance(request, DefaultUnsubscribeRequest):
            raise TypeError("request must be a DefaultUnsubscribeRequest")

        response = DefaultUnsubscribeResponse()
        await self._send_rpc(4, request, response)
        return response

class NotificationListenerServiceClient(BaseRpcClient):
    """
    Service which listens for notifications forwarded by the `NotificationService`.
    """

    service_full_name = "horus.pb.NotificationListenerService"
    service_id = 10

    async def notify_log_message(self, request: LogMessageEvent) -> None:
        """
        Service which listens for notifications forwarded by the `NotificationService`.
        """
        if not isinstance(request, LogMessageEvent):
            raise TypeError("request must be a LogMessageEvent")

        await self._send_rpc(1, request)

    async def notify_profiling_info(self, request: ProfilingInfoEvent) -> None:
        """
        Service which listens for notifications forwarded by the `NotificationService`.
        """
        if not isinstance(request, ProfilingInfoEvent):
            raise TypeError("request must be a ProfilingInfoEvent")

        await self._send_rpc(2, request)

    async def notify_sensor_info(self, request: SensorInfoEvent) -> None:
        """
        Service which listens for notifications forwarded by the `NotificationService`.
        """
        if not isinstance(request, SensorInfoEvent):
            raise TypeError("request must be a SensorInfoEvent")

        await self._send_rpc(3, request)
