import asyncio
import logging
import typing

if typing.TYPE_CHECKING:
    from typing_extensions import Self
else:
    Self = None

from horus.pb.detection_service.detection_service_client import (
    DetectionServiceClient as _DetectionServiceClient,
)
from horus.sdk.detection import DetectionEvent
from horus.pb.detection_service.detection_service_client import (
    DetectionServiceClient as _DetectionServiceClient,
)
from horus.pb.notification_service.service_client import (
    NotificationServiceClient as _NotificationServiceClient,
)
from horus.pb.project_manager.service_pb2 import (
    GetHealthStatusRequest as _GetHealthStatusRequest,
)
from horus.pb.project_manager.service_client import (
    ProjectManagerServiceClient as _ProjectManagerServiceClient,
)
from horus.pb.status_service.service_client import (
    StatusServiceClient as _StatusServiceClient,
    GetVersionRequest as _GetVersionRequest,
)
from horus.pb.rpc_pb2 import DefaultSubscribeRequest as _DefaultSubscribeRequest
from horus.pb.rpc_pb2 import DefaultUnsubscribeRequest as _DefaultUnsubscribeRequest
from horus.rpc.client_listener_pair import ClientListenerPair as _ClientListenerPair
from horus.rpc.client import Client as _Client
from horus.rpc.services import RpcServices
from horus.sdk.detection import DetectionEvent
from horus.sdk.services import DetectionServiceListener as _DetectionServiceListener
from horus.sdk.logs import Log, SensorInfoEvent
from horus.sdk.services import (
    NotificationServiceListener as _NotificationServiceListener,
)
from horus.sdk.services import (
    PointAggregatorServiceListener as _PointAggregatorServiceListener,
)
from horus.pb.point_aggregator.point_aggregator_service_client import (
    PointAggregatorServiceClient as _PointAggregatorServiceClient,
)
from horus.sdk.profiling import ProfilingInfo
from horus.sdk.sensor import OccupancyGridEvent
from horus.proto import *
from horus.sdk.health import (
    HealthStatus,
)
from horus.sdk.version import Version

_T = typing.TypeVar("_T")

_UnsubscribeCallable = typing.Callable[[], typing.Awaitable[None]]
"See `Subscription.__init__()`."


class Sdk:
    """
    The Horus SDK object, entry point to interact with Horus services.
    """

    def __init__(
        self,
        services: RpcServices = RpcServices(),
        /,
        logger: logging.Logger = logging.getLogger("horus"),
    ) -> None:
        """
        Initializes the `Sdk`, which will connect to the services as specified by `services`.

        Diagnostics will be logged through the given `logger`.

        Note that connections to services will only be established on demand, _not_ when the SDK
        is created.
        """

        self.services = services
        self._logger = logger

        self._detection_service = _ClientListenerPair(
            services.detection.url,
            logger,
            _DetectionServiceClient,
            _DetectionServiceListener,
            _DetectionServiceClient.subscribe,
            _DetectionServiceClient.unsubscribe,
        )

        self._notification_service = _ClientListenerPair(
            services.notification.url,
            logger,
            _NotificationServiceClient,
            _NotificationServiceListener,
            _NotificationServiceClient.subscribe,
            _NotificationServiceClient.unsubscribe,
        )

        self._point_aggregator_service = _ClientListenerPair(
            services.point_aggregator.url,
            logger,
            _PointAggregatorServiceClient,
            _PointAggregatorServiceListener,
            _PointAggregatorServiceClient.subscribe,
            _PointAggregatorServiceClient.unsubscribe,
        )

        self._project_manager_service_client = _Client(
            services.project_manager.url,
            logger,
            _ProjectManagerServiceClient,
        )

        self._status_service_client = _Client(
            services.project_manager.url, logger, _StatusServiceClient
        )

    def subscribe_to_detections(
        self, on_detection_event: typing.Callable[[DetectionEvent], None]
    ) -> "Subscription":
        """Returns a `Subscription` which will call `on_detection_event()` with new detections until destroyed."""
        if not callable(on_detection_event):
            raise TypeError("on_detection_event must be callable")

        return Subscription(
            lambda: self._subscribe_to_detections_async(on_detection_event)
        )

    def subscribe_to_logs(
        self, on_log_message: typing.Callable[[Log], None]
    ) -> "Subscription":
        """Returns a `Subscription` which will call `on_log_message()` with new logs until destroyed."""
        if not callable(on_log_message):
            raise TypeError("on_log_message must be callable")

        return Subscription(lambda: self._subscribe_to_logs_async(on_log_message))

    def subscribe_to_occupancy_grid(
        self, on_occupancy_grid_event: typing.Callable[[OccupancyGridEvent], None]
    ) -> "Subscription":
        """Returns a `Subscription` which will call `on_occupancy_grid()` with new occupancy grids until destroyed."""
        if not callable(on_occupancy_grid_event):
            raise TypeError("on_occupancy_grid must be callable")

        return Subscription(
            lambda: self._subscribe_to_occupancy_grid_async(on_occupancy_grid_event)
        )

    def subscribe_to_profiling(
        self, on_profiling_info: typing.Callable[[ProfilingInfo], None]
    ) -> "Subscription":
        """Returns a `Subscription` which will call `on_profiling_info()` with new profiling information until destroyed."""
        if not callable(on_profiling_info):
            raise TypeError("on_profiling_info must be callable")

        return Subscription(
            lambda: self._subscribe_to_profiling_async(on_profiling_info)
        )

    def subscribe_to_sensor_info(
        self, on_sensor_info_event: typing.Callable[[SensorInfoEvent], None]
    ) -> "Subscription":
        """Returns a `Subscription` which will call `on_sensor_info_event()` with new sensor information until destroyed."""
        if not callable(on_sensor_info_event):
            raise TypeError("on_sensor_info_event must be callable")

        return Subscription(
            lambda: self._subscribe_to_sensor_info_async(on_sensor_info_event)
        )

    async def get_health_status(self) -> HealthStatus:
        """
        Retrieves a general status summarizing the health of the system.

        This health status gathers:
        - License status
        - Sensor statuses
        - Service connectivity statuses
        """

        client = await self._ensure_project_manager_service()
        return HealthStatus._from_pb(
            await client.get_health_status(_GetHealthStatusRequest())
        )

    async def get_version(self) -> Version:
        """Retrieve horus version."""
        client = await self._ensure_status_service()
        return Version._from_pb(await client.get_version(_GetVersionRequest()))

    async def _ensure_detection_service(
        self,
    ) -> typing.Tuple[_DetectionServiceClient, _DetectionServiceListener]:
        return await self._detection_service.connect_and_subscribe(
            _DefaultSubscribeRequest(), _DefaultUnsubscribeRequest()
        )

    async def _ensure_notification_service(
        self,
    ) -> typing.Tuple[_NotificationServiceClient, _NotificationServiceListener]:
        return await self._notification_service.connect_and_subscribe(
            _DefaultSubscribeRequest(), _DefaultUnsubscribeRequest()
        )

    async def _ensure_point_aggregator_service(
        self,
    ) -> typing.Tuple[_PointAggregatorServiceClient, _PointAggregatorServiceListener]:
        return await self._point_aggregator_service.connect_and_subscribe(
            _DefaultSubscribeRequest(), _DefaultUnsubscribeRequest()
        )

    async def _ensure_project_manager_service(
        self,
    ) -> _ProjectManagerServiceClient:
        return await self._project_manager_service_client.connect()

    async def _ensure_status_service(
        self,
    ) -> _StatusServiceClient:
        return await self._status_service_client.connect()

    async def _subscribe_to_detections_async(
        self, on_detection_event: typing.Callable[[DetectionEvent], None]
    ) -> _UnsubscribeCallable:
        _, listener = await self._ensure_detection_service()

        listener._on_detection_event.add(on_detection_event)

        return lambda: _disconnect_if_set_emptied(
            listener._on_detection_event,
            on_detection_event,
            self._detection_service,
            listener.has_no_subscriber,
        )

    async def _subscribe_to_logs_async(
        self, on_log_message: typing.Callable[[Log], None]
    ) -> _UnsubscribeCallable:
        _, listener = await self._ensure_notification_service()

        listener._on_log_message.add(on_log_message)

        return lambda: _disconnect_if_set_emptied(
            listener._on_log_message,
            on_log_message,
            self._notification_service,
            listener.has_no_subscriber,
        )

    async def _subscribe_to_occupancy_grid_async(
        self, on_occupancy_grid_event: typing.Callable[[OccupancyGridEvent], None]
    ) -> _UnsubscribeCallable:
        _, listener = await self._ensure_point_aggregator_service()

        listener._on_occupancy_grid_event.add(on_occupancy_grid_event)

        return lambda: _disconnect_if_set_emptied(
            listener._on_occupancy_grid_event,
            on_occupancy_grid_event,
            self._point_aggregator_service,
            listener.has_no_subscriber,
        )

    async def _subscribe_to_profiling_async(
        self, on_profiling_info: typing.Callable[[ProfilingInfo], None]
    ) -> _UnsubscribeCallable:
        _, listener = await self._ensure_notification_service()

        listener._on_profiling_info.add(on_profiling_info)

        return lambda: _disconnect_if_set_emptied(
            listener._on_profiling_info,
            on_profiling_info,
            self._notification_service,
            listener.has_no_subscriber,
        )

    async def _subscribe_to_sensor_info_async(
        self, on_sensor_info_event: typing.Callable[[SensorInfoEvent], None]
    ) -> _UnsubscribeCallable:
        _, listener = await self._ensure_notification_service()

        listener._on_sensor_info_event.add(on_sensor_info_event)

        return lambda: _disconnect_if_set_emptied(
            listener._on_sensor_info_event,
            on_sensor_info_event,
            self._notification_service,
            listener.has_no_subscriber,
        )

    async def close(self) -> None:
        """Closes all connections"""

        await asyncio.gather(
            self._detection_service.disconnect(),
            self._notification_service.disconnect(),
            self._point_aggregator_service.disconnect(),
            self._project_manager_service_client.disconnect(),
        )

    async def __aenter__(self) -> Self:
        """No-op. Must be called to use the SDK as an async context manager."""
        return self

    async def __aexit__(
        self, exc_type: typing.Any, exc: typing.Any, tb: typing.Any
    ) -> None:
        """Closes all connections"""

        await self.close()


class Subscription:
    """
    A subscription to an event stream created by the SDK.

    The subscription may not be established yet, use `async with` to ensure it is set up and later
    cleaned up:

        async with sdk.subscribe_to_logs(...):
            ...

    The subscription can also be manually awaited and cleaned up:

        subscription = await sdk.subscribe_to_logs(...)
        ...
        await subscription.unsubscribe()
    """

    def __init__(
        self,
        subscribe: typing.Callable[[], typing.Awaitable[_UnsubscribeCallable]],
    ) -> None:
        """
        Initializes the subscription.

        Args:
            subscribe: async function which sets up the subscription and returns another async
                function which tears down the subscription.
        """
        self._unsubscribe: typing.Union[
            typing.Awaitable[_UnsubscribeCallable], _UnsubscribeCallable, None
        ] = subscribe()

    def __await__(self) -> typing.Generator["Subscription", typing.Any, typing.Any]:
        """Awaits for the subscription to be initiated."""

        async def impl() -> "Subscription":
            await self.__aenter__()
            return self

        return impl().__await__()

    async def unsubscribe(self) -> None:
        """Unsubscribes from future events."""
        if self._unsubscribe is None:
            return

        if not callable(self._unsubscribe):
            self._unsubscribe = await self._unsubscribe

        await self._unsubscribe()

        self._unsubscribe = None

    async def __aenter__(self) -> None:
        """Enters the "subscription context", equivalent to `await`ing it."""
        if self._unsubscribe is not None and not callable(self._unsubscribe):
            self._unsubscribe = await self._unsubscribe

    async def __aexit__(
        self, exc_type: typing.Any, exc: typing.Any, tb: typing.Any
    ) -> None:
        """Exits the "subscription context", equivalent to `unsubscribe()`-ing."""
        _ = exc_type, exc, tb

        await self.unsubscribe()


async def _disconnect_if_set_emptied(
    set: typing.Set[_T],
    value: _T,
    client_listener_pair: _ClientListenerPair[
        typing.Any, typing.Any, typing.Any, typing.Any
    ],
    is_empty: typing.Callable[[], bool],
) -> None:
    """Removes `value` from `set` and calls `unsubscribe()` if `set` is now empty and `is_empty()` returns true."""
    set.discard(value)

    if not set and is_empty():
        await client_listener_pair.disconnect()
