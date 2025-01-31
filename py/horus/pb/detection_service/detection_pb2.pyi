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

class DetectedObject(_message.Message):
    __slots__ = ("classification", "kinematics", "shape", "status")
    class Classification(_message.Message):
        __slots__ = ("class_label",)
        CLASS_LABEL_FIELD_NUMBER: _ClassVar[int]
        class_label: ObjectLabel
        def __init__(self, class_label: _Optional[_Union[ObjectLabel, str]] = ...) -> None: ...
    class Kinematics(_message.Message):
        __slots__ = ("linear_velocity", "yaw_rate")
        LINEAR_VELOCITY_FIELD_NUMBER: _ClassVar[int]
        YAW_RATE_FIELD_NUMBER: _ClassVar[int]
        linear_velocity: _metadata_pb2.Vector2f
        yaw_rate: float
        def __init__(self, linear_velocity: _Optional[_Union[_metadata_pb2.Vector2f, _Mapping]] = ..., yaw_rate: _Optional[float] = ...) -> None: ...
    class Shape(_message.Message):
        __slots__ = ("bounding_box",)
        BOUNDING_BOX_FIELD_NUMBER: _ClassVar[int]
        bounding_box: BoundingBox
        def __init__(self, bounding_box: _Optional[_Union[BoundingBox, _Mapping]] = ...) -> None: ...
    class Status(_message.Message):
        __slots__ = ("id", "tracking_status")
        ID_FIELD_NUMBER: _ClassVar[int]
        TRACKING_STATUS_FIELD_NUMBER: _ClassVar[int]
        id: int
        tracking_status: TrackingStatus
        def __init__(self, id: _Optional[int] = ..., tracking_status: _Optional[_Union[TrackingStatus, str]] = ...) -> None: ...
    CLASSIFICATION_FIELD_NUMBER: _ClassVar[int]
    KINEMATICS_FIELD_NUMBER: _ClassVar[int]
    SHAPE_FIELD_NUMBER: _ClassVar[int]
    STATUS_FIELD_NUMBER: _ClassVar[int]
    classification: DetectedObject.Classification
    kinematics: DetectedObject.Kinematics
    shape: DetectedObject.Shape
    status: DetectedObject.Status
    def __init__(self, classification: _Optional[_Union[DetectedObject.Classification, _Mapping]] = ..., kinematics: _Optional[_Union[DetectedObject.Kinematics, _Mapping]] = ..., shape: _Optional[_Union[DetectedObject.Shape, _Mapping]] = ..., status: _Optional[_Union[DetectedObject.Status, _Mapping]] = ...) -> None: ...

class DetectionEvent(_message.Message):
    __slots__ = ("objects", "labeled_point_clouds", "frame_info")
    class FrameInfo(_message.Message):
        __slots__ = ("frame_timestamp",)
        FRAME_TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
        frame_timestamp: _metadata_pb2.Timestamp
        def __init__(self, frame_timestamp: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ...) -> None: ...
    OBJECTS_FIELD_NUMBER: _ClassVar[int]
    LABELED_POINT_CLOUDS_FIELD_NUMBER: _ClassVar[int]
    FRAME_INFO_FIELD_NUMBER: _ClassVar[int]
    objects: _containers.RepeatedCompositeFieldContainer[DetectedObject]
    labeled_point_clouds: _containers.RepeatedCompositeFieldContainer[LabeledPointCloud]
    frame_info: DetectionEvent.FrameInfo
    def __init__(self, objects: _Optional[_Iterable[_Union[DetectedObject, _Mapping]]] = ..., labeled_point_clouds: _Optional[_Iterable[_Union[LabeledPointCloud, _Mapping]]] = ..., frame_info: _Optional[_Union[DetectionEvent.FrameInfo, _Mapping]] = ...) -> None: ...
