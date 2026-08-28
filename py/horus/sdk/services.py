import asyncio, dataclasses, datetime, logging, time, typing

from google.protobuf.message import DecodeError

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
from horus.sdk.point_frame import PointFrame, PointFrameFields
from horus.sdk._raw_point_message import RawAggregatedPointEvents
from horus.pb.point import point_message_pb2

if typing.TYPE_CHECKING:
    from typing_extensions import override
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


_PointCloudCallable = typing.Callable[[typing.Sequence[PointFrame]], None]

_DROP_WARNING_INTERVAL = datetime.timedelta(seconds=5)
"""How often a subscriber with no `on_drop` callback is warned about dropped broadcasts."""

_PointFrames = typing.Tuple[PointFrame, ...]


@dataclasses.dataclass(eq=False)
class PointCloudSubscriber:
    callback: _PointCloudCallable

    fields: PointFrameFields
    """Per-point arrays this subscriber wants decoded."""

    queue: "typing.Optional[asyncio.Queue[typing.Optional[_PointFrames]]]"
    """
    Bounded queue feeding `callback`, or `None` to call it inline in the WebSocket receive loop.

    A `None` value inside the queue indicates the end of the subscription, terminating the pump.
    """

    on_drop: typing.Callable[[int], None]
    """
    Called with the running dropped-broadcast count whenever one is dropped. Subscribers
    which did not supply one are given `_default_on_drop()`.
    """

    pump: "typing.Optional[asyncio.Task[None]]" = None
    """Task draining `queue` into `callback`."""

    dropped: int = 0
    """Number of broadcasts dropped because `callback` could not keep up."""


class PointAggregatorServiceListener(PointAggregatorSubscriberServiceHandler):
    """
    `PointAggregatorServiceHandler` implementation used by the `Sdk`.
    """

    def __init__(self, ws: WebSocket, logger: logging.Logger) -> None:
        super().__init__(ws)

        # `broadcast_processed_points()` decodes the wire-compatible alias instead of the
        # generated message, materializing only the point arrays subscribers asked for.
        self._decode_request_as(
            self.broadcast_processed_points,
            RawAggregatedPointEvents,
            point_message_pb2.AggregatedPointEvents,
        )

        self._on_occupancy_grid_event: typing.Set[
            typing.Callable[[sensor.OccupancyGridListEvent], None]
        ] = set()

        self._point_cloud_subscribers: typing.Set[PointCloudSubscriber] = set()

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
        if not self._point_cloud_subscribers:
            return

        # `_decode_request_as()` rebound this handler, so the message which actually arrives is
        # the wire-compatible alias; the signature keeps the generated type it overrides.
        raw_request = typing.cast(RawAggregatedPointEvents, request)

        # Decode once for all subscribers, materializing only the arrays somebody asked for.
        # Frames are immutable and their buffers read-only, so they can be shared.
        fields = PointFrameFields.NONE
        for subscriber in self._point_cloud_subscribers:
            fields |= subscriber.fields

        point_frames: typing.List[PointFrame] = []
        for event in raw_request.events:
            try:
                point_frames.append(PointFrame._from_raw(event.point_frame, fields))
            except (ValueError, DecodeError) as e:
                # `DecodeError` comes from re-parsing the frame header, which is carried
                # as opaque bytes by `RawPointFrame`.
                self._logger.error(f"cannot parse PointFrame: {e}")
                continue

        if not point_frames:
            return

        snapshot = tuple(point_frames)
        for subscriber in self._point_cloud_subscribers:
            self._dispatch_point_cloud(subscriber, snapshot)

    def _add_point_cloud_subscriber(
        self,
        callback: _PointCloudCallable,
        fields: PointFrameFields,
        queue_size: int,
        on_drop: typing.Optional[typing.Callable[[int], None]],
    ) -> PointCloudSubscriber:
        """
        Registers `callback`, starting a task to feed it if `queue_size` is positive.

        Must be called from the event loop the SDK runs on.
        """
        if queue_size < 0:
            raise ValueError("queue_size must be positive, or zero to dispatch inline")

        subscriber = PointCloudSubscriber(
            callback=callback,
            fields=fields,
            queue=asyncio.Queue(maxsize=queue_size) if queue_size > 0 else None,
            on_drop=self._default_on_drop() if on_drop is None else on_drop,
        )

        if subscriber.queue is not None:
            subscriber.pump = asyncio.create_task(self._pump_point_clouds(subscriber))

        self._point_cloud_subscribers.add(subscriber)

        return subscriber

    async def _remove_point_cloud_subscriber(
        self, subscriber: PointCloudSubscriber
    ) -> None:
        """Unregisters `subscriber`, awaiting the end of the task feeding it."""
        self._point_cloud_subscribers.discard(subscriber)

        if subscriber.dropped != 0:
            self._logger.warning(
                f"point cloud subscription dropped {subscriber.dropped} broadcasts in "
                f"total because its callback could not keep up"
            )

        queue = subscriber.queue
        pump = subscriber.pump
        subscriber.pump = None

        if queue is None or pump is None:
            return

        await queue.put(None)
        await pump

    def _dispatch_point_cloud(
        self, subscriber: PointCloudSubscriber, frames: _PointFrames
    ) -> None:
        """
        Hands `frames` to `subscriber`.

        Queued subscribers never block the receive loop: if the queue is full, the
        oldest pending broadcast is dropped in favor of the newest one, since a
        subscriber which has fallen behind is better served by fresh data than by a
        backlog.
        """
        queue = subscriber.queue

        if queue is None:
            self._invoke_point_cloud_callback(subscriber, frames)
            return

        if not queue.full():
            queue.put_nowait(frames)
            return

        queue.get_nowait()
        queue.put_nowait(frames)

        subscriber.dropped += 1

        try:
            subscriber.on_drop(subscriber.dropped)
        except Exception:
            self._logger.exception("point cloud on_drop() callback failed")

    def _default_on_drop(self) -> typing.Callable[[int], None]:
        """Returns an `on_drop` callback warning on the SDK logger, at a limited rate."""
        interval = _DROP_WARNING_INTERVAL.total_seconds()
        last_warning: typing.Optional[float] = None

        def warn(dropped: int) -> None:
            nonlocal last_warning

            now = time.monotonic()

            if last_warning is not None and now - last_warning < interval:
                return

            last_warning = now

            self._logger.warning(
                f"dropped a point cloud broadcast because the subscriber's callback "
                f"could not keep up ({dropped} dropped so far); increase queue_size to "
                f"tolerate bursts, pass on_drop to handle this yourself, or reduce the "
                f"work done in the callback"
            )

        return warn

    async def _pump_point_clouds(self, subscriber: PointCloudSubscriber) -> None:
        """Drains `subscriber`'s queue into its callback until the end-of-subscription sentinel."""
        queue = subscriber.queue
        assert queue is not None

        while True:
            item = await queue.get()

            if item is None:
                return

            self._invoke_point_cloud_callback(subscriber, item)

            # Dropped before waiting for the next broadcast, so that the frames are released as
            # soon as the callback returns instead of being held alive by this local.
            del item

    def _invoke_point_cloud_callback(
        self, subscriber: PointCloudSubscriber, frames: _PointFrames
    ) -> None:
        """Calls `subscriber`'s callback, logging any error it raises."""
        try:
            subscriber.callback(frames)
        except Exception:
            self._logger.exception("point cloud subscriber callback failed")

    def has_no_subscriber(self) -> bool:
        return not self._on_occupancy_grid_event and not self._point_cloud_subscribers
