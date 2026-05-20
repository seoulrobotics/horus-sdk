from horus.pb.config import metadata_pb2 as _metadata_pb2
from horus.pb.point import point_message_pb2 as _point_message_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class ObjectLabel(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    LABEL_UNSPECIFIED: _ClassVar[ObjectLabel]
    MISC: _ClassVar[ObjectLabel]
    CAR: _ClassVar[ObjectLabel]
    CYCLIST: _ClassVar[ObjectLabel]
    PEDESTRIAN: _ClassVar[ObjectLabel]

class TrackingStatus(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    TRACKING_STATUS_UNSPECIFIED: _ClassVar[TrackingStatus]
    INVALIDATING: _ClassVar[TrackingStatus]
    VALIDATING: _ClassVar[TrackingStatus]
    DRIFTING: _ClassVar[TrackingStatus]
    TRACKING: _ClassVar[TrackingStatus]
LABEL_UNSPECIFIED: ObjectLabel
MISC: ObjectLabel
CAR: ObjectLabel
CYCLIST: ObjectLabel
PEDESTRIAN: ObjectLabel
TRACKING_STATUS_UNSPECIFIED: TrackingStatus
INVALIDATING: TrackingStatus
VALIDATING: TrackingStatus
DRIFTING: TrackingStatus
TRACKING: TrackingStatus

class BoundingBox(_message.Message):
    __slots__ = ("base", "size", "yaw")
    BASE_FIELD_NUMBER: _ClassVar[int]
    SIZE_FIELD_NUMBER: _ClassVar[int]
    YAW_FIELD_NUMBER: _ClassVar[int]
    base: _metadata_pb2.Vector3f
    size: _metadata_pb2.Vector3f
    yaw: float
    def __init__(self, base: _Optional[_Union[_metadata_pb2.Vector3f, _Mapping]] = ..., size: _Optional[_Union[_metadata_pb2.Vector3f, _Mapping]] = ..., yaw: _Optional[float] = ...) -> None: ...

class LabeledPointCloud(_message.Message):
    __slots__ = ("point_cloud", "point_index_to_object_id")
    POINT_CLOUD_FIELD_NUMBER: _ClassVar[int]
    POINT_INDEX_TO_OBJECT_ID_FIELD_NUMBER: _ClassVar[int]
    point_cloud: _point_message_pb2.PointFrame
    point_index_to_object_id: _metadata_pb2.UInt32List
    def __init__(self, point_cloud: _Optional[_Union[_point_message_pb2.PointFrame, _Mapping]] = ..., point_index_to_object_id: _Optional[_Union[_metadata_pb2.UInt32List, _Mapping]] = ...) -> None: ...

class TimeRange(_message.Message):
    __slots__ = ("start", "end")
    START_FIELD_NUMBER: _ClassVar[int]
    END_FIELD_NUMBER: _ClassVar[int]
    start: _metadata_pb2.Timestamp
    end: _metadata_pb2.Timestamp
    def __init__(self, start: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ..., end: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ...) -> None: ...

class DetectedObject(_message.Message):
    __slots__ = ("classification", "kinematics", "shape", "status", "event_zone_ids")
    class Classification(_message.Message):
        __slots__ = ("class_label", "class_confidence")
        CLASS_LABEL_FIELD_NUMBER: _ClassVar[int]
        CLASS_CONFIDENCE_FIELD_NUMBER: _ClassVar[int]
        class_label: ObjectLabel
        class_confidence: float
        def __init__(self, class_label: _Optional[_Union[ObjectLabel, str]] = ..., class_confidence: _Optional[float] = ...) -> None: ...
    class Kinematics(_message.Message):
        __slots__ = ("linear_velocity", "yaw_rate")
        LINEAR_VELOCITY_FIELD_NUMBER: _ClassVar[int]
        YAW_RATE_FIELD_NUMBER: _ClassVar[int]
        linear_velocity: _metadata_pb2.Vector2f
        yaw_rate: float
        def __init__(self, linear_velocity: _Optional[_Union[_metadata_pb2.Vector2f, _Mapping]] = ..., yaw_rate: _Optional[float] = ...) -> None: ...
    class Shape(_message.Message):
        __slots__ = ("bounding_box", "tight_bounding_box")
        BOUNDING_BOX_FIELD_NUMBER: _ClassVar[int]
        TIGHT_BOUNDING_BOX_FIELD_NUMBER: _ClassVar[int]
        bounding_box: BoundingBox
        tight_bounding_box: BoundingBox
        def __init__(self, bounding_box: _Optional[_Union[BoundingBox, _Mapping]] = ..., tight_bounding_box: _Optional[_Union[BoundingBox, _Mapping]] = ...) -> None: ...
    class Status(_message.Message):
        __slots__ = ("id", "tracking_status", "last_seen", "observation_time_range")
        ID_FIELD_NUMBER: _ClassVar[int]
        TRACKING_STATUS_FIELD_NUMBER: _ClassVar[int]
        LAST_SEEN_FIELD_NUMBER: _ClassVar[int]
        OBSERVATION_TIME_RANGE_FIELD_NUMBER: _ClassVar[int]
        id: int
        tracking_status: TrackingStatus
        last_seen: _metadata_pb2.Timestamp
        observation_time_range: TimeRange
        def __init__(self, id: _Optional[int] = ..., tracking_status: _Optional[_Union[TrackingStatus, str]] = ..., last_seen: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ..., observation_time_range: _Optional[_Union[TimeRange, _Mapping]] = ...) -> None: ...
    CLASSIFICATION_FIELD_NUMBER: _ClassVar[int]
    KINEMATICS_FIELD_NUMBER: _ClassVar[int]
    SHAPE_FIELD_NUMBER: _ClassVar[int]
    STATUS_FIELD_NUMBER: _ClassVar[int]
    EVENT_ZONE_IDS_FIELD_NUMBER: _ClassVar[int]
    classification: DetectedObject.Classification
    kinematics: DetectedObject.Kinematics
    shape: DetectedObject.Shape
    status: DetectedObject.Status
    event_zone_ids: _containers.RepeatedScalarFieldContainer[str]
    def __init__(self, classification: _Optional[_Union[DetectedObject.Classification, _Mapping]] = ..., kinematics: _Optional[_Union[DetectedObject.Kinematics, _Mapping]] = ..., shape: _Optional[_Union[DetectedObject.Shape, _Mapping]] = ..., status: _Optional[_Union[DetectedObject.Status, _Mapping]] = ..., event_zone_ids: _Optional[_Iterable[str]] = ...) -> None: ...

class ZoneEvent(_message.Message):
    __slots__ = ("timestamp", "zone_id", "type", "object", "object_id")
    class Type(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = ()
        ZONE_EVENT_TYPE_UNSPECIFIED: _ClassVar[ZoneEvent.Type]
        ENTRY: _ClassVar[ZoneEvent.Type]
        EXIT: _ClassVar[ZoneEvent.Type]
    ZONE_EVENT_TYPE_UNSPECIFIED: ZoneEvent.Type
    ENTRY: ZoneEvent.Type
    EXIT: ZoneEvent.Type
    TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    ZONE_ID_FIELD_NUMBER: _ClassVar[int]
    TYPE_FIELD_NUMBER: _ClassVar[int]
    OBJECT_FIELD_NUMBER: _ClassVar[int]
    OBJECT_ID_FIELD_NUMBER: _ClassVar[int]
    timestamp: _metadata_pb2.Timestamp
    zone_id: str
    type: ZoneEvent.Type
    object: DetectedObject
    object_id: int
    def __init__(self, timestamp: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ..., zone_id: _Optional[str] = ..., type: _Optional[_Union[ZoneEvent.Type, str]] = ..., object: _Optional[_Union[DetectedObject, _Mapping]] = ..., object_id: _Optional[int] = ...) -> None: ...

class ZoneEventList(_message.Message):
    __slots__ = ("zone_events",)
    ZONE_EVENTS_FIELD_NUMBER: _ClassVar[int]
    zone_events: _containers.RepeatedCompositeFieldContainer[ZoneEvent]
    def __init__(self, zone_events: _Optional[_Iterable[_Union[ZoneEvent, _Mapping]]] = ...) -> None: ...

class DeepLearningObject(_message.Message):
    __slots__ = ("classification", "bounding_box", "associated_object_id")
    class Classification(_message.Message):
        __slots__ = ("class_label", "class_confidence")
        CLASS_LABEL_FIELD_NUMBER: _ClassVar[int]
        CLASS_CONFIDENCE_FIELD_NUMBER: _ClassVar[int]
        class_label: ObjectLabel
        class_confidence: float
        def __init__(self, class_label: _Optional[_Union[ObjectLabel, str]] = ..., class_confidence: _Optional[float] = ...) -> None: ...
    CLASSIFICATION_FIELD_NUMBER: _ClassVar[int]
    BOUNDING_BOX_FIELD_NUMBER: _ClassVar[int]
    ASSOCIATED_OBJECT_ID_FIELD_NUMBER: _ClassVar[int]
    classification: DeepLearningObject.Classification
    bounding_box: BoundingBox
    associated_object_id: int
    def __init__(self, classification: _Optional[_Union[DeepLearningObject.Classification, _Mapping]] = ..., bounding_box: _Optional[_Union[BoundingBox, _Mapping]] = ..., associated_object_id: _Optional[int] = ...) -> None: ...

class DebugMergerInfo(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class DetectionEvent(_message.Message):
    __slots__ = ("objects", "labeled_point_clouds", "frame_info", "raw_deep_learning_objects", "unrecovered_object_ids", "is_replaying", "overall_frame_latency", "publishing_time", "debug_merger_info")
    class FrameInfo(_message.Message):
        __slots__ = ("frame_timestamp",)
        FRAME_TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
        frame_timestamp: _metadata_pb2.Timestamp
        def __init__(self, frame_timestamp: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ...) -> None: ...
    OBJECTS_FIELD_NUMBER: _ClassVar[int]
    LABELED_POINT_CLOUDS_FIELD_NUMBER: _ClassVar[int]
    FRAME_INFO_FIELD_NUMBER: _ClassVar[int]
    RAW_DEEP_LEARNING_OBJECTS_FIELD_NUMBER: _ClassVar[int]
    UNRECOVERED_OBJECT_IDS_FIELD_NUMBER: _ClassVar[int]
    IS_REPLAYING_FIELD_NUMBER: _ClassVar[int]
    OVERALL_FRAME_LATENCY_FIELD_NUMBER: _ClassVar[int]
    PUBLISHING_TIME_FIELD_NUMBER: _ClassVar[int]
    DEBUG_MERGER_INFO_FIELD_NUMBER: _ClassVar[int]
    objects: _containers.RepeatedCompositeFieldContainer[DetectedObject]
    labeled_point_clouds: _containers.RepeatedCompositeFieldContainer[LabeledPointCloud]
    frame_info: DetectionEvent.FrameInfo
    raw_deep_learning_objects: _containers.RepeatedCompositeFieldContainer[DeepLearningObject]
    unrecovered_object_ids: _containers.RepeatedScalarFieldContainer[int]
    is_replaying: bool
    overall_frame_latency: _metadata_pb2.Duration
    publishing_time: _metadata_pb2.Timestamp
    debug_merger_info: DebugMergerInfo
    def __init__(self, objects: _Optional[_Iterable[_Union[DetectedObject, _Mapping]]] = ..., labeled_point_clouds: _Optional[_Iterable[_Union[LabeledPointCloud, _Mapping]]] = ..., frame_info: _Optional[_Union[DetectionEvent.FrameInfo, _Mapping]] = ..., raw_deep_learning_objects: _Optional[_Iterable[_Union[DeepLearningObject, _Mapping]]] = ..., unrecovered_object_ids: _Optional[_Iterable[int]] = ..., is_replaying: bool = ..., overall_frame_latency: _Optional[_Union[_metadata_pb2.Duration, _Mapping]] = ..., publishing_time: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ..., debug_merger_info: _Optional[_Union[DebugMergerInfo, _Mapping]] = ...) -> None: ...
