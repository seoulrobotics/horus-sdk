import dataclasses
from datetime import datetime
import enum
import typing
import horus.pb.project_manager.service_pb2 as pm_pb
import horus.pb.license_server.messages_pb2 as lc_pb
from horus.logs.format import (
    format_license_active_info,
    format_license_expired_error,
    format_license_not_found_error,
    format_license_server_connection_error,
    format_service_connection_timed_out,
)
from horus.sdk.sensor import SensorInfo, SensorStatus


@dataclasses.dataclass(frozen=True)
class LicenseInfo:
    number_of_lidars: int
    """Number of simultaneous lidars allowed by the license."""
    expiration_epoch: datetime
    """Timestamp of license expiration."""

    @staticmethod
    def _from_pb(pb: lc_pb.LicenseInfo) -> "LicenseInfo":
        return LicenseInfo(
            number_of_lidars=pb.lidar_count,
            expiration_epoch=datetime.fromtimestamp(pb.expiration_date.seconds),
        )


class LicensePrivilege(enum.Flag):
    """Privilege granted by a license."""

    LOWEST = 1
    SUBSCRIBE = 2
    LICENSE_ACTIONS = 4
    READ = 8
    WRITE = 16


@dataclasses.dataclass(frozen=True)
class LicenseStatus:
    """License status of horus instance."""

    level: int
    """Numeric representation of the current level. Ranges from 1 to 4."""
    reason: str
    """The reason why the license is at that level."""
    privileges: typing.FrozenSet[LicensePrivilege]
    """The set of privileges the current license grants you."""
    license_info: typing.Optional[LicenseInfo]
    """If a license was found, i.e if level is >= 3, contains the info of the license."""

    @staticmethod
    def _from_pb(pb: pm_pb.LicenseStatus) -> "LicenseStatus":
        level: int
        reason: str
        privileges: typing.FrozenSet[LicensePrivilege]
        license_info: typing.Optional[LicenseInfo] = None

        level_oneof = pb.license_level.WhichOneof("level")
        if level_oneof == "level_1":
            level = 1
            reason = format_license_server_connection_error(
                pb.license_level.level_1.license_server_unreachable
            )
        elif level_oneof == "level_2":
            level = 2
            reason = format_license_not_found_error(
                pb.license_level.level_2.license_not_found
            )
        elif level_oneof == "level_3":
            level = 3
            reason_oneof = pb.license_level.level_3.WhichOneof("reason")
            if reason_oneof == "license_expired":
                reason = format_license_expired_error(
                    pb.license_level.level_3.license_expired
                )
            elif reason_oneof == "license_server_disconnected":
                reason = format_license_server_connection_error(
                    pb.license_level.level_3.license_server_disconnected
                )
            else:
                raise Exception("Invalid LicenseLevel enum value")
            license_info = LicenseInfo._from_pb(pb.license_info)
        elif level_oneof == "level_4":
            level = 4
            reason = format_license_active_info(pb.license_level.level_4.license_active)
            license_info = LicenseInfo._from_pb(pb.license_info)
        else:
            raise Exception("Invalid LicenseLevel enum value")

        privileges = frozenset(
            LicensePrivilege(privilege)
            for privilege in pm_pb.LicensePrivilege.values()
            if privilege != 0 and (pb.license_level.privilege & privilege) == privilege
        )
        return LicenseStatus(
            level=level,
            reason=reason,
            privileges=privileges,
            license_info=license_info,
        )


@dataclasses.dataclass(frozen=True)
class SensorHealth:
    """A sensor health status. Maybe the node serving the sensor info was unreachable."""

    lidar_id: str
    """Unique identifier of the sensor."""
    status: typing.FrozenSet[SensorStatus]
    """Status of the sensor."""
    measured_frequency_hz: float
    """Frequency at which the sensor is measuring."""

    node_unreachable_error: typing.Optional[str]

    def is_unreachable(self) -> bool:
        """Returns whether the sensor could not be fetched from the preprocessing node."""
        return self.node_unreachable_error is not None

    @staticmethod
    def _from_pb(pb: pm_pb.GetHealthStatusResponse.SensorHealth) -> "SensorHealth":
        sensor_info = SensorInfo._from_pb(pb.info)

        node_unreachable_error: typing.Optional[str] = None
        if pb.WhichOneof("node_unreachable_error") == "timeout":
            node_unreachable_error = format_service_connection_timed_out(pb.timeout)

        return SensorHealth(
            lidar_id=sensor_info.lidar_id,
            status=sensor_info.status,
            measured_frequency_hz=sensor_info.measured_frequency_hz,
            node_unreachable_error=node_unreachable_error,
        )


@dataclasses.dataclass(frozen=True)
class NodeHealth:
    """Connectivity status of a specific node."""

    class Service(enum.Enum):
        """Service represented by a node."""

        DETECTION = 1
        LIDAR_RUNNER = 2
        NOTIFICATION = 3
        POINT_AGGREGATOR = 4
        PREPROCESSING = 5
        PROJECT_MANAGER = 6

    class Status(enum.Enum):
        """Connectivity status of a node."""

        UNREACHABLE = 1
        ALIVE = 2

    service: Service
    """Service represented by the node."""
    node_id: str
    """Unique node identifier."""
    node_status: Status
    """Connectivity status."""

    @staticmethod
    def _from_pb(
        pb: pm_pb.GetHealthStatusResponse.NodeHealth,
    ) -> "NodeHealth":
        return NodeHealth(
            node_id=pb.node_id,
            node_status=NodeHealth.Status(pb.node_status),
            service=NodeHealth.Service(pb.service),
        )


@dataclasses.dataclass(frozen=True)
class HealthStatus:
    """Response of `ProjectManager.GetHealthStatus`."""

    license_status: LicenseStatus
    """Status of the license."""
    sensor_statuses: typing.List[SensorHealth]
    """Statuses of the running lidars."""
    service_statuses: typing.List[NodeHealth]
    """Statuses of the services."""

    @staticmethod
    def _from_pb(pb: pm_pb.GetHealthStatusResponse) -> "HealthStatus":
        license_status: LicenseStatus = LicenseStatus._from_pb(pb.license_status)
        sensor_statuses: typing.List[SensorHealth] = []
        service_statuses: typing.List[NodeHealth] = []

        if LicensePrivilege.READ in license_status.privileges:
            sensor_statuses = [
                SensorHealth._from_pb(sensor_status)
                for sensor_status in pb.sensor_statuses
            ]
            service_statuses = [
                NodeHealth._from_pb(node_status) for node_status in pb.service_statuses
            ]

        return HealthStatus(
            license_status=license_status,
            sensor_statuses=sensor_statuses,
            service_statuses=service_statuses,
        )
