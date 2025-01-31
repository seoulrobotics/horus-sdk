from horus.pb.config import metadata_pb2 as _metadata_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class PointAttribute(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    ATTRIBUTE_NONE: _ClassVar[PointAttribute]
    OUT_OF_RANGE: _ClassVar[PointAttribute]
    GROUND: _ClassVar[PointAttribute]
    LOW_INTENSITY: _ClassVar[PointAttribute]
    EXCLUSION_ZONE: _ClassVar[PointAttribute]
    STATIC: _ClassVar[PointAttribute]
    STATIC_DETECTION_ZONE: _ClassVar[PointAttribute]
    REFLECTION_ZONE: _ClassVar[PointAttribute]
ATTRIBUTE_NONE: PointAttribute
OUT_OF_RANGE: PointAttribute
GROUND: PointAttribute
LOW_INTENSITY: PointAttribute
EXCLUSION_ZONE: PointAttribute
STATIC: PointAttribute
STATIC_DETECTION_ZONE: PointAttribute
REFLECTION_ZONE: PointAttribute

class PointFrame(_message.Message):
    __slots__ = ("header", "points")
    class Header(_message.Message):
        __slots__ = ("calibration_transform", "lidar_id", "point_cloud_creation_timestamp")
        CALIBRATION_TRANSFORM_FIELD_NUMBER: _ClassVar[int]
        LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
        POINT_CLOUD_CREATION_TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
        calibration_transform: _metadata_pb2.AffineTransform3f
        lidar_id: str
        point_cloud_creation_timestamp: _metadata_pb2.Timestamp
        def __init__(self, calibration_transform: _Optional[_Union[_metadata_pb2.AffineTransform3f, _Mapping]] = ..., lidar_id: _Optional[str] = ..., point_cloud_creation_timestamp: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ...) -> None: ...
    HEADER_FIELD_NUMBER: _ClassVar[int]
    POINTS_FIELD_NUMBER: _ClassVar[int]
    header: PointFrame.Header
    points: AttributedPoints
    def __init__(self, header: _Optional[_Union[PointFrame.Header, _Mapping]] = ..., points: _Optional[_Union[AttributedPoints, _Mapping]] = ...) -> None: ...

class AttributedPoints(_message.Message):
    __slots__ = ("flattened_points", "attributes", "intensities")
    FLATTENED_POINTS_FIELD_NUMBER: _ClassVar[int]
    ATTRIBUTES_FIELD_NUMBER: _ClassVar[int]
    INTENSITIES_FIELD_NUMBER: _ClassVar[int]
    flattened_points: _containers.RepeatedScalarFieldContainer[float]
    attributes: _containers.RepeatedScalarFieldContainer[int]
    intensities: _containers.RepeatedScalarFieldContainer[int]
    def __init__(self, flattened_points: _Optional[_Iterable[float]] = ..., attributes: _Optional[_Iterable[int]] = ..., intensities: _Optional[_Iterable[int]] = ...) -> None: ...

class ProcessedPointsEvent(_message.Message):
    __slots__ = ("point_frame",)
    POINT_FRAME_FIELD_NUMBER: _ClassVar[int]
    point_frame: PointFrame
    def __init__(self, point_frame: _Optional[_Union[PointFrame, _Mapping]] = ...) -> None: ...

class AggregatedPointEvents(_message.Message):
    __slots__ = ("events",)
    EVENTS_FIELD_NUMBER: _ClassVar[int]
    events: _containers.RepeatedCompositeFieldContainer[ProcessedPointsEvent]
    def __init__(self, events: _Optional[_Iterable[_Union[ProcessedPointsEvent, _Mapping]]] = ...) -> None: ...
