from dataclasses import dataclass as _dataclass
from typing import Optional as _Optional

@_dataclass(frozen=True)
class RpcServiceInfo:
    """Information about a Horus RPC service."""

    full_name: str
    "Unique full name of the service, e.g. `horus.pb.NotificationService`."
    id: int
    "Unique identifier of the service, e.g. 1."
    host: str
    "Host to connect to the service, e.g. `localhost`."
    port: int
    "Port to connect to the service, e.g. `40001`."

    @property
    def url(self) -> str:
        """URL used to connect to the service."""
        return f"ws://{self.host}:{self.port}"

    def with_(self, /, host: _Optional[str]=None, port: _Optional[int]=None) -> "RpcServiceInfo":
        """Returns a copy of `self` with `host` and `port` replaced by the given values."""
        return RpcServiceInfo(
            full_name=self.full_name,
            id=self.id,
            host=host if host is not None else self.host,
            port=port if port is not None else self.port,
        )

@_dataclass(frozen=True)
class RpcServices:
    """Enumeration of the Horus RPC services."""

    detection: RpcServiceInfo = RpcServiceInfo(
        full_name="horus.pb.DetectionService",
        id=7,
        host="127.0.0.1",
        port=40007,
    )

    notification: RpcServiceInfo = RpcServiceInfo(
        full_name="horus.pb.NotificationService",
        id=2,
        host="127.0.0.1",
        port=40002,
    )

    point_aggregator: RpcServiceInfo = RpcServiceInfo(
        full_name="horus.pb.PointAggregatorService",
        id=6,
        host="127.0.0.1",
        port=40006,
    )

    def with_(self,
              /,
              detection: _Optional[RpcServiceInfo]=None,
              notification: _Optional[RpcServiceInfo]=None,
              point_aggregator: _Optional[RpcServiceInfo]=None,
             ) -> "RpcServices":
        """Returns a copy of `self` with services replaced as specified."""
        return RpcServices(
            detection=detection or self.detection,
            notification=notification or self.notification,
            point_aggregator=point_aggregator or self.point_aggregator,
        )
