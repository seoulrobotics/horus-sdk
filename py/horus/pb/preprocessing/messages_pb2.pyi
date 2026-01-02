from horus.pb.config import metadata_pb2 as _metadata_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class OccupancyClassification(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    OCCUPANCYCLASSIFICATION_UNSPECIFIED: _ClassVar[OccupancyClassification]
    FREE: _ClassVar[OccupancyClassification]
    OCCLUDED: _ClassVar[OccupancyClassification]
    STATIONARY_OCCUPIED: _ClassVar[OccupancyClassification]
    EXCLUDED: _ClassVar[OccupancyClassification]

class SensorStatus(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    SENSOR_STATUS_UNSPECIFIED: _ClassVar[SensorStatus]
    NO_DATA: _ClassVar[SensorStatus]
    RECEIVING_DATA: _ClassVar[SensorStatus]
    LOW_FREQUENCY: _ClassVar[SensorStatus]
    HIGH_FREQUENCY: _ClassVar[SensorStatus]
    TILTED: _ClassVar[SensorStatus]
    OBSTRUCTED: _ClassVar[SensorStatus]
    PACKET_DROP: _ClassVar[SensorStatus]
    AUTO_CORRECTION_MODERATE: _ClassVar[SensorStatus]
    AUTO_CORRECTION_SEVERE: _ClassVar[SensorStatus]
OCCUPANCYCLASSIFICATION_UNSPECIFIED: OccupancyClassification
FREE: OccupancyClassification
OCCLUDED: OccupancyClassification
STATIONARY_OCCUPIED: OccupancyClassification
EXCLUDED: OccupancyClassification
SENSOR_STATUS_UNSPECIFIED: SensorStatus
NO_DATA: SensorStatus
RECEIVING_DATA: SensorStatus
LOW_FREQUENCY: SensorStatus
HIGH_FREQUENCY: SensorStatus
TILTED: SensorStatus
OBSTRUCTED: SensorStatus
PACKET_DROP: SensorStatus
AUTO_CORRECTION_MODERATE: SensorStatus
AUTO_CORRECTION_SEVERE: SensorStatus

class OccupancyGrid(_message.Message):
    __slots__ = ("rows", "cols", "cells")
    ROWS_FIELD_NUMBER: _ClassVar[int]
    COLS_FIELD_NUMBER: _ClassVar[int]
    CELLS_FIELD_NUMBER: _ClassVar[int]
    rows: int
    cols: int
    cells: _containers.RepeatedScalarFieldContainer[int]
    def __init__(self, rows: _Optional[int] = ..., cols: _Optional[int] = ..., cells: _Optional[_Iterable[int]] = ...) -> None: ...

class OccupancyGridEvent(_message.Message):
    __slots__ = ("grid", "x_min", "x_max", "y_min", "y_max", "resolution", "timestamp")
    GRID_FIELD_NUMBER: _ClassVar[int]
    X_MIN_FIELD_NUMBER: _ClassVar[int]
    X_MAX_FIELD_NUMBER: _ClassVar[int]
    Y_MIN_FIELD_NUMBER: _ClassVar[int]
    Y_MAX_FIELD_NUMBER: _ClassVar[int]
    RESOLUTION_FIELD_NUMBER: _ClassVar[int]
    TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    grid: OccupancyGrid
    x_min: float
    x_max: float
    y_min: float
    y_max: float
    resolution: float
    timestamp: _metadata_pb2.Timestamp
    def __init__(self, grid: _Optional[_Union[OccupancyGrid, _Mapping]] = ..., x_min: _Optional[float] = ..., x_max: _Optional[float] = ..., y_min: _Optional[float] = ..., y_max: _Optional[float] = ..., resolution: _Optional[float] = ..., timestamp: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ...) -> None: ...

class SensorInfo(_message.Message):
    __slots__ = ("lidar_id", "status", "measured_frequency", "pose_correction", "num_total_dropped_packets", "num_total_expected_packets")
    class PoseCorrection(_message.Message):
        __slots__ = ("translation", "rotation")
        TRANSLATION_FIELD_NUMBER: _ClassVar[int]
        ROTATION_FIELD_NUMBER: _ClassVar[int]
        translation: _metadata_pb2.Vector3f
        rotation: _metadata_pb2.Quaterniond
        def __init__(self, translation: _Optional[_Union[_metadata_pb2.Vector3f, _Mapping]] = ..., rotation: _Optional[_Union[_metadata_pb2.Quaterniond, _Mapping]] = ...) -> None: ...
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    STATUS_FIELD_NUMBER: _ClassVar[int]
    MEASURED_FREQUENCY_FIELD_NUMBER: _ClassVar[int]
    POSE_CORRECTION_FIELD_NUMBER: _ClassVar[int]
    NUM_TOTAL_DROPPED_PACKETS_FIELD_NUMBER: _ClassVar[int]
    NUM_TOTAL_EXPECTED_PACKETS_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    status: int
    measured_frequency: float
    pose_correction: SensorInfo.PoseCorrection
    num_total_dropped_packets: int
    num_total_expected_packets: int
    def __init__(self, lidar_id: _Optional[str] = ..., status: _Optional[int] = ..., measured_frequency: _Optional[float] = ..., pose_correction: _Optional[_Union[SensorInfo.PoseCorrection, _Mapping]] = ..., num_total_dropped_packets: _Optional[int] = ..., num_total_expected_packets: _Optional[int] = ...) -> None: ...
