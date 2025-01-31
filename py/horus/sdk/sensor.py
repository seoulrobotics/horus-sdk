import dataclasses
import enum
import typing

from horus.pb.notification_service import service_pb2
from horus.pb.preprocessing import messages_pb2


class SensorStatus(enum.Flag):
    """The status of a sensor."""

    SENSOR_STATUS_UNSPECIFIED = 0
    NO_DATA = 1
    RECEIVING_DATA = 2
    LOW_FREQUENCY = 4
    HIGH_FREQUENCY = 8
    TILTED = 16
    OBSTRUCTED = 32


@dataclasses.dataclass(frozen=True)
class SensorInfo:
    """A sensor status message."""

    lidar_id: str
    """Unique identifier of the sensor."""
    status: SensorStatus
    """Status of the sensor."""
    measured_frequency_hz: float
    """Frequency at which the sensor is measuring."""

    @staticmethod
    def _from_pb(pb: service_pb2.SensorInfo) -> "SensorInfo":
        return SensorInfo(
            lidar_id=pb.lidar_id,
            status=SensorStatus(pb.status),
            measured_frequency_hz=pb.measured_frequency,
        )


@dataclasses.dataclass(frozen=True)
class SensorInfoEvent:
    """A sensor status message."""

    sensor_info: typing.List[SensorInfo]
    """Status of the sensor."""

    @staticmethod
    def _from_pb(pb: service_pb2.SensorInfoEvent) -> "SensorInfoEvent":
        return SensorInfoEvent(
            sensor_info=[
                SensorInfo._from_pb(sensor_info) for sensor_info in pb.sensor_info
            ]
        )


class OccupancyClassification(enum.Flag):
    """The classification of an occupancy grid cell."""

    OCCUPANCYCLASSIFICATION_UNSPECIFIED = 0
    FREE = 1
    OCCLUDED = 2
    STATIONARY_OCCUPIED = 3


@dataclasses.dataclass(frozen=True)
class OccupancyGrid:
    """An occupancy grid."""

    """The number of rows in the grid."""
    rows: int
    """The number of columns in the grid."""
    cols: int
    """The grid data."""
    data: typing.List[int]

    @staticmethod
    def _from_pb(pb: messages_pb2.OccupancyGrid) -> "OccupancyGrid":
        NUM_COUNT_BITS = 29
        MAX_COUNT = (1 << NUM_COUNT_BITS) - 1
        data = []

        for cell in pb.cells:
            # Extract classification (3 most significant bits)
            classification = OccupancyClassification(cell >> NUM_COUNT_BITS)

            # Extract count (remaining 29 bits)
            count = cell & MAX_COUNT

            data.extend([classification.value] * count)

        return OccupancyGrid(rows=pb.rows, cols=pb.cols, data=data)


@dataclasses.dataclass(frozen=True)
class OccupancyGridEvent:
    """An occupancy grid."""

    """The grid data."""
    grid: OccupancyGrid

    """The x min of the detection range."""
    x_min: float
    """The x max of the detection range."""
    x_max: float
    """The y min of the detection range."""
    y_min: float
    """The y max of the detection range."""
    y_max: float
    """The resolution of the grid."""
    resolution: float

    @staticmethod
    def _from_pb(pb: messages_pb2.OccupancyGridEvent) -> "OccupancyGridEvent":
        return OccupancyGridEvent(
            grid=OccupancyGrid._from_pb(pb.grid),
            x_min=pb.x_min,
            x_max=pb.x_max,
            y_min=pb.y_min,
            y_max=pb.y_max,
            resolution=pb.resolution,
        )
