from horus.pb.logs import message_pb2 as _message_pb2
from horus.pb.preprocessing import messages_pb2 as _messages_pb2
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

class SensorInfoEvent(_message.Message):
    __slots__ = ("sensor_info",)
    SENSOR_INFO_FIELD_NUMBER: _ClassVar[int]
    sensor_info: _containers.RepeatedCompositeFieldContainer[_messages_pb2.SensorInfo]
    def __init__(self, sensor_info: _Optional[_Iterable[_Union[_messages_pb2.SensorInfo, _Mapping]]] = ...) -> None: ...
