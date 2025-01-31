import abc as _abc
from ...rpc.base_handler import BaseRpcHandler, RpcMessage, WebSocket

from ..detection_service.detection_pb2 import DetectionEvent
from ..rpc_pb2 import DefaultSubscribeRequest, DefaultSubscribeResponse, DefaultUnsubscribeRequest, DefaultUnsubscribeResponse

class DetectionServiceHandler(BaseRpcHandler, metaclass=_abc.ABCMeta):
    """
    Service for receiving pre-processed foreground points and sending detection results.
    """

    service_full_name = "horus.pb.DetectionService"
    service_id = 7

    def __init__(self, ws: WebSocket):
        super().__init__(ws, {
            2: (DefaultSubscribeRequest, DefaultSubscribeResponse, self.subscribe),
            3: (DefaultUnsubscribeRequest, DefaultUnsubscribeResponse, self.unsubscribe),
        })

    @_abc.abstractmethod
    async def subscribe(self, request: DefaultSubscribeRequest) -> DefaultSubscribeResponse:
        """
        Subscribes to receive future detection results as they are made available.

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

class DetectionSubscriberServiceHandler(BaseRpcHandler, metaclass=_abc.ABCMeta):
    """
    Service exposed by clients of the `DetectionService` that wish to receive updates when new detection results are processed.
    """

    service_full_name = "horus.pb.DetectionSubscriberService"
    service_id = 12

    def __init__(self, ws: WebSocket):
        super().__init__(ws, {
            1: (DetectionEvent, None, self.broadcast_detection),
        })

    @_abc.abstractmethod
    async def broadcast_detection(self, request: DetectionEvent) -> None:
        """
        Notify new detection results.
        """
        raise NotImplementedError
