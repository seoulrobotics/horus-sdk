from ...rpc.base_client import BaseRpcClient

from ..point.point_message_pb2 import AggregatedPointEvents
from ..preprocessing.messages_pb2 import OccupancyGridEvent
from ..rpc_pb2 import DefaultSubscribeRequest, DefaultSubscribeResponse, DefaultUnsubscribeRequest, DefaultUnsubscribeResponse

class PointAggregatorServiceClient(BaseRpcClient):
    """
    Service for receiving pre-processed point clouds.
    """

    service_full_name = "horus.pb.PointAggregatorService"
    service_id = 6

    async def subscribe(self, request: DefaultSubscribeRequest) -> DefaultSubscribeResponse:
        """
        Service for receiving pre-processed point clouds.
        """
        if not isinstance(request, DefaultSubscribeRequest):
            raise TypeError("request must be a DefaultSubscribeRequest")

        response = DefaultSubscribeResponse()
        await self._send_rpc(5, request, response)
        return response

    async def unsubscribe(self, request: DefaultUnsubscribeRequest) -> DefaultUnsubscribeResponse:
        """
        Service for receiving pre-processed point clouds.
        """
        if not isinstance(request, DefaultUnsubscribeRequest):
            raise TypeError("request must be a DefaultUnsubscribeRequest")

        response = DefaultUnsubscribeResponse()
        await self._send_rpc(6, request, response)
        return response

class PointAggregatorSubscriberServiceClient(BaseRpcClient):
    """
    Service exposed by clients of the `PointAggregatorService` that wish to receive updates when new processed points are received.
    """

    service_full_name = "horus.pb.PointAggregatorSubscriberService"
    service_id = 5

    async def broadcast_processed_points(self, request: AggregatedPointEvents) -> None:
        """
        Service exposed by clients of the `PointAggregatorService` that wish to receive updates when new processed points are received.
        """
        if not isinstance(request, AggregatedPointEvents):
            raise TypeError("request must be a AggregatedPointEvents")

        await self._send_rpc(2, request)

    async def broadcast_occupancy_grid(self, request: OccupancyGridEvent) -> None:
        """
        Service exposed by clients of the `PointAggregatorService` that wish to receive updates when new processed points are received.
        """
        if not isinstance(request, OccupancyGridEvent):
            raise TypeError("request must be a OccupancyGridEvent")

        await self._send_rpc(3, request)
