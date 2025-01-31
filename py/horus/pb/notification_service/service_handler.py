import abc as _abc
from ...rpc.base_handler import BaseRpcHandler, RpcMessage, WebSocket

from ..notification_service.service_pb2 import LogMessageEvent, LogMessageRequest, ProfilingInfoEvent, SensorInfoEvent
from ..rpc_pb2 import DefaultSubscribeRequest, DefaultSubscribeResponse, DefaultUnsubscribeRequest, DefaultUnsubscribeResponse

class NotificationServiceHandler(BaseRpcHandler, metaclass=_abc.ABCMeta):
    """
    Service which receives notifications from services, storing and forwarding them.
    """

    service_full_name = "horus.pb.NotificationService"
    service_id = 2

    def __init__(self, ws: WebSocket):
        super().__init__(ws, {
            1: (LogMessageRequest, None, self.log_message),
            3: (DefaultSubscribeRequest, DefaultSubscribeResponse, self.subscribe),
            4: (DefaultUnsubscribeRequest, DefaultUnsubscribeResponse, self.unsubscribe),
        })

    @_abc.abstractmethod
    async def log_message(self, request: LogMessageRequest) -> None:
        """
        Logs a message, storing it and broadcasting it to subscribers.
        """
        raise NotImplementedError

    @_abc.abstractmethod
    async def subscribe(self, request: DefaultSubscribeRequest) -> DefaultSubscribeResponse:
        """
        Subscribes to new log and profiling events.

        This function has a default implementation in C++, but not in Python.
        """
        raise NotImplementedError

    @_abc.abstractmethod
    async def unsubscribe(self, request: DefaultUnsubscribeRequest) -> DefaultUnsubscribeResponse:
        """
        Unsubscribe following a call to `Subscribe()`.

        This function has a default implementation in C++, but not in Python.
        """
        raise NotImplementedError

class NotificationListenerServiceHandler(BaseRpcHandler, metaclass=_abc.ABCMeta):
    """
    Service which listens for notifications forwarded by the `NotificationService`.
    """

    service_full_name = "horus.pb.NotificationListenerService"
    service_id = 10

    def __init__(self, ws: WebSocket):
        super().__init__(ws, {
            1: (LogMessageEvent, None, self.notify_log_message),
            2: (ProfilingInfoEvent, None, self.notify_profiling_info),
            3: (SensorInfoEvent, None, self.notify_sensor_info),
        })

    @_abc.abstractmethod
    async def notify_log_message(self, request: LogMessageEvent) -> None:
        """
        Received when a log message is received by the `NotificationService`.
        """
        raise NotImplementedError

    @_abc.abstractmethod
    async def notify_profiling_info(self, request: ProfilingInfoEvent) -> None:
        """
        Received when a profiling info is received by the `NotificationService`.
        """
        raise NotImplementedError

    @_abc.abstractmethod
    async def notify_sensor_info(self, request: SensorInfoEvent) -> None:
        """
        Received when a sensor info is received by the `NotificationService`.
        """
        raise NotImplementedError
