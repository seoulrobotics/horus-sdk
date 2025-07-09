import abc as _abc
from ...rpc.base_handler import BaseRpcHandler, RpcMessage, WebSocket

from ..detection_service.detection_pb2 import DetectionEvent
from ..rpc_pb2 import DefaultSubscribeRequest, DefaultSubscribeResponse, DefaultUnsubscribeRequest, DefaultUnsubscribeResponse

class DetectionMergerServiceHandler(BaseRpcHandler, metaclass=_abc.ABCMeta):
    """
    Service for merging detection results from multiple sources.
    """

    service_full_name = "horus.pb.DetectionMergerService"
    service_id = 11

    def __init__(self, ws: WebSocket):
        super().__init__(ws, {
            1: (DefaultSubscribeRequest, DefaultSubscribeResponse, self.subscribe),
            2: (DefaultUnsubscribeRequest, DefaultUnsubscribeResponse, self.unsubscribe),
        })

    @_abc.abstractmethod
    async def subscribe(self, request: DefaultSubscribeRequest) -> DefaultSubscribeResponse:
        """
        Subscribe to receive merged detection results.

        This function has a default implementation in C++, but not in Python.
        """
        raise NotImplementedError

    @_abc.abstractmethod
    async def unsubscribe(self, request: DefaultUnsubscribeRequest) -> DefaultUnsubscribeResponse:
        """
        Unsubscribe from merged detection results.

        This function has a default implementation in C++, but not in Python.
        """
        raise NotImplementedError

class DetectionMergerSubscriberServiceHandler(BaseRpcHandler, metaclass=_abc.ABCMeta):
    """
    Service for receiving merged detection results.
    """

    service_full_name = "horus.pb.DetectionMergerSubscriberService"
    service_id = 16

    def __init__(self, ws: WebSocket):
        super().__init__(ws, {
            1: (DetectionEvent, None, self.broadcast_detection),
        })

    @_abc.abstractmethod
    async def broadcast_detection(self, request: DetectionEvent) -> None:
        """
        Receive merged detection results.
        """
        raise NotImplementedError
