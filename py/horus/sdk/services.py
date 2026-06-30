import logging, typing

from horus.pb.notification_service import service_pb2
from horus.pb.detection_service import detection_pb2
from horus.pb.preprocessing import messages_pb2

from horus.pb.detection_merger.service_handler import (
    DetectionMergerSubscriberServiceHandler,
)
from horus.pb.notification_service.service_handler import (
    NotificationListenerServiceHandler,
)
from horus.pb.point_aggregator.point_aggregator_service_handler import (
    PointAggregatorSubscriberServiceHandler,
)

from horus.rpc.websocket import WebSocket
from horus.sdk.detection import DetectionEvent, ZoneEvent

from horus.sdk.profiling import ProfilingInfo
from horus.sdk import sensor
from horus.sdk.logs import Log
from horus.sdk.point_frame import PointFrame
from horus.pb.point import point_message_pb2

if typing.TYPE_CHECKING:
    from typing_extensions import override, TypeAliasType
else:
    TypeAliasType = lambda name, t: t
    override = lambda f: f

# IMPORTANT: Python attributes, like Python arguments, share their default values across instances.
# We must make sure to only set immutable values as default values, or to only set them in
# `__init__()`.


class DetectionMergerServiceListener(DetectionMergerSubscriberServiceHandler):
    """
    `DetectionMergerSubscriberServiceHandler` implementation used by the `Sdk`.
    """

    def __init__(self, ws: WebSocket, logger: logging.Logger) -> None:
        super().__init__(ws)

        self._on_detection_event: typing.Set[
            typing.Callable[[DetectionEvent], None]
        ] = set()
        self._on_zone_event: typing.Set[typing.Callable[[ZoneEvent], None]] = set()
        self._logger = logger

    @override
    async def broadcast_detection(self, request: detection_pb2.DetectionEvent) -> None:
        if not self._on_detection_event:
            return

        try:
            detection_event = DetectionEvent._from_pb(request)
        except ValueError:
            self._logger.error(
                "cannot parse DetectionEvent",
                extra={"detection_event": request},
            )
            return

        for subscriber in self._on_detection_event:
            subscriber(detection_event)

    @override
    async def broadcast_zone_events(self, request: detection_pb2.ZoneEventList) -> None:
        if not self._on_zone_event:
            return

        for zone_event_pb in request.zone_events:
            try:
                zone_event = ZoneEvent._from_pb(zone_event_pb)
            except ValueError:
                self._logger.error(
                    "cannot parse ZoneEvent",
                    extra={"zone_event": zone_event_pb},
                )
                continue

            for subscriber in self._on_zone_event:
                subscriber(zone_event)

    def has_no_subscriber(self) -> bool:
        return not self._on_detection_event and not self._on_zone_event


class NotificationServiceListener(NotificationListenerServiceHandler):
    """
    `NotificationListenerServiceHandler` implementation used by the `Sdk`.
    """

    def __init__(self, ws: WebSocket, logger: logging.Logger) -> None:
        super().__init__(ws)

        self._on_log_message: typing.Set[typing.Callable[[Log], None]] = set()
        self._on_profiling_info: typing.Set[typing.Callable[[ProfilingInfo], None]] = (
            set()
        )
        self._on_sensor_info_event: typing.Set[
            typing.Callable[[sensor.SensorInfoEvent], None]
        ] = set()

        self._logger = logger

    @override
    async def notify_log_message(self, request: service_pb2.LogMessageEvent) -> None:
        if not self._on_log_message:
            return

        try:
            log = Log._from_pb(request.log_message)
        except ValueError:
            self._logger.error(
                "cannot parse LogMessage", extra={"log_message": request.log_message}
            )
            return

        # `Log`s are frozen so we can safely share them with multiple subscribers.
        for subscriber in self._on_log_message:
            subscriber(log)

    @override
    async def notify_profiling_info(
        self, request: service_pb2.ProfilingInfoEvent
    ) -> None:
        if not self._on_profiling_info:
            return

        try:
            profiling_info = ProfilingInfo._from_pb(request.profiling_info)
        except ValueError:
            self._logger.error(
                f"cannot parse ProfilingInfo",
                extra={"profiling_info": request.profiling_info},
            )
            return

        # `ProfilingInfo`s are frozen so we can safely share them with multiple subscribers.
        for subscriber in self._on_profiling_info:
            subscriber(profiling_info)

    def has_no_subscriber(self) -> bool:
        """Returns whether the listener has any subscriber remaining."""
        return not (self._on_log_message or self._on_profiling_info)

    @override
    async def notify_sensor_info(self, request: service_pb2.SensorInfoEvent) -> None:
        if not self._on_sensor_info_event:
            return

        try:
            sensor_info = sensor.SensorInfoEvent._from_pb(request)
        except ValueError:
            self._logger.error(
                f"cannot parse SensorInfoEvent",
                extra={"sensor_info": request},
            )
            return

        # `SensorInfoEvent`s are frozen so we can safely share them with multiple subscribers.
        for subscriber in self._on_sensor_info_event:
            subscriber(sensor_info)


class PointAggregatorServiceListener(PointAggregatorSubscriberServiceHandler):
    """
    `PointAggregatorServiceHandler` implementation used by the `Sdk`.
    """

    def __init__(self, ws: WebSocket, logger: logging.Logger) -> None:
        super().__init__(ws)

        self._on_occupancy_grid_event: typing.Set[
            typing.Callable[[sensor.OccupancyGridListEvent], None]
        ] = set()
        self._on_aggregated_point_event: typing.Set[
            typing.Callable[[PointFrame], None]
        ] = set()

        self._logger = logger

    @override
    async def broadcast_occupancy_grid(
        self, request: messages_pb2.OccupancyGridEvent
    ) -> None:
        pass  # Deprecated: use broadcast_occupancy_grid_list instead.

    @override
    async def broadcast_occupancy_grid_list(
        self, request: messages_pb2.OccupancyGridListEvent
    ) -> None:
        if not self._on_occupancy_grid_event:
            return

        try:
            occupancy_grids_event = sensor.OccupancyGridListEvent._from_pb(request)
        except ValueError:
            self._logger.error(
                "cannot parse OccupancyGridListEvent",
                extra={"broadcast_occupancy_grid_list": request},
            )
            return

        for subscriber in self._on_occupancy_grid_event:
            subscriber(occupancy_grids_event)

    @override
    async def broadcast_processed_points(
        self, request: point_message_pb2.AggregatedPointEvents
    ) -> None:
        if not self._on_aggregated_point_event:
            return

        for event in request.events:
            try:
                point_frame = PointFrame._from_pb(event.point_frame)
            except ValueError:
                self._logger.error(
                    "cannot parse PointFrame",
                    extra={"processed_points_event": event},
                )
                continue

            for subscriber in self._on_aggregated_point_event:
                subscriber(point_frame)

    def has_no_subscriber(self) -> bool:
        return not self._on_occupancy_grid_event and not self._on_aggregated_point_event
