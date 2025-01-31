import abc as _abc
from ...rpc.base_handler import BaseRpcHandler, RpcMessage, WebSocket

from ..point.point_message_pb2 import AggregatedPointEvents
from ..preprocessing.messages_pb2 import OccupancyGridEvent
from ..rpc_pb2 import DefaultSubscribeRequest, DefaultSubscribeResponse, DefaultUnsubscribeRequest, DefaultUnsubscribeResponse

class PointAggregatorServiceHandler(BaseRpcHandler, metaclass=_abc.ABCMeta):
    """
    Service for receiving pre-processed point clouds.
    """

    service_full_name = "horus.pb.PointAggregatorService"
    service_id = 6

    def __init__(self, ws: WebSocket):
        super().__init__(ws, {
            5: (DefaultSubscribeRequest, DefaultSubscribeResponse, self.subscribe),
            6: (DefaultUnsubscribeRequest, DefaultUnsubscribeResponse, self.unsubscribe),
        })

    @_abc.abstractmethod
    async def subscribe(self, request: DefaultSubscribeRequest) -> DefaultSubscribeResponse:
        """
        Subscribes to receive future processed point clouds as they are made available.

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

class PointAggregatorSubscriberServiceHandler(BaseRpcHandler, metaclass=_abc.ABCMeta):
    """
    Service exposed by clients of the `PointAggregatorService` that wish to receive updates when new processed points are received.
    """

    service_full_name = "horus.pb.PointAggregatorSubscriberService"
    service_id = 5

    def __init__(self, ws: WebSocket):
        super().__init__(ws, {
            2: (AggregatedPointEvents, None, self.broadcast_processed_points),
            3: (OccupancyGridEvent, None, self.broadcast_occupancy_grid),
        })

    @_abc.abstractmethod
    async def broadcast_processed_points(self, request: AggregatedPointEvents) -> None:
        """
        Notify new processed points input.
        """
        raise NotImplementedError

    @_abc.abstractmethod
    async def broadcast_occupancy_grid(self, request: OccupancyGridEvent) -> None:
        """
        Notify new occupancy grid input.
        """
        raise NotImplementedError
