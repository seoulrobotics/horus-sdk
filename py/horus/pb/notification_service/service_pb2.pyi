from horus.pb.config import metadata_pb2 as _metadata_pb2
from horus.pb.logs import message_pb2 as _message_pb2
from horus.pb import profiling_pb2 as _profiling_pb2
from horus.pb import rpc_pb2 as _rpc_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class LogMessageRequest(_message.Message):
    __slots__ = ("log_message",)
    LOG_MESSAGE_FIELD_NUMBER: _ClassVar[int]
    log_message: _message_pb2.LogMessage
    def __init__(self, log_message: _Optional[_Union[_message_pb2.LogMessage, _Mapping]] = ...) -> None: ...

class LogMessageEvent(_message.Message):
    __slots__ = ("log_message",)
    LOG_MESSAGE_FIELD_NUMBER: _ClassVar[int]
    log_message: _message_pb2.LogMessage
    def __init__(self, log_message: _Optional[_Union[_message_pb2.LogMessage, _Mapping]] = ...) -> None: ...

class ProfilingInfoEvent(_message.Message):
    __slots__ = ("profiling_info",)
    PROFILING_INFO_FIELD_NUMBER: _ClassVar[int]
    profiling_info: _profiling_pb2.ProfilingInfo
    def __init__(self, profiling_info: _Optional[_Union[_profiling_pb2.ProfilingInfo, _Mapping]] = ...) -> None: ...

class SensorInfo(_message.Message):
    __slots__ = ("lidar_id", "status", "measured_frequency", "pose_correction")
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
    lidar_id: str
    status: int
    measured_frequency: float
    pose_correction: SensorInfo.PoseCorrection
    def __init__(self, lidar_id: _Optional[str] = ..., status: _Optional[int] = ..., measured_frequency: _Optional[float] = ..., pose_correction: _Optional[_Union[SensorInfo.PoseCorrection, _Mapping]] = ...) -> None: ...

class SensorInfoEvent(_message.Message):
    __slots__ = ("sensor_info",)
    SENSOR_INFO_FIELD_NUMBER: _ClassVar[int]
    sensor_info: _containers.RepeatedCompositeFieldContainer[SensorInfo]
    def __init__(self, sensor_info: _Optional[_Iterable[_Union[SensorInfo, _Mapping]]] = ...) -> None: ...
