from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class LogMetadata(_message.Message):
    __slots__ = ("ms_since_epoch", "severity", "node_id")
    class Severity(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = ()
        SEVERITY_UNSPECIFIED: _ClassVar[LogMetadata.Severity]
        SEVERITY_DEBUG: _ClassVar[LogMetadata.Severity]
        SEVERITY_INFO: _ClassVar[LogMetadata.Severity]
        SEVERITY_WARNING: _ClassVar[LogMetadata.Severity]
        SEVERITY_ERROR: _ClassVar[LogMetadata.Severity]
        SEVERITY_FATAL: _ClassVar[LogMetadata.Severity]
    SEVERITY_UNSPECIFIED: LogMetadata.Severity
    SEVERITY_DEBUG: LogMetadata.Severity
    SEVERITY_INFO: LogMetadata.Severity
    SEVERITY_WARNING: LogMetadata.Severity
    SEVERITY_ERROR: LogMetadata.Severity
    SEVERITY_FATAL: LogMetadata.Severity
    class SourceLocation(_message.Message):
        __slots__ = ("file", "line")
        FILE_FIELD_NUMBER: _ClassVar[int]
        LINE_FIELD_NUMBER: _ClassVar[int]
        file: str
        line: int
        def __init__(self, file: _Optional[str] = ..., line: _Optional[int] = ...) -> None: ...
    class Duration(_message.Message):
        __slots__ = ("ms",)
        MS_FIELD_NUMBER: _ClassVar[int]
        ms: int
        def __init__(self, ms: _Optional[int] = ...) -> None: ...
    class Timestamp(_message.Message):
        __slots__ = ("ms_since_epoch",)
        MS_SINCE_EPOCH_FIELD_NUMBER: _ClassVar[int]
        ms_since_epoch: int
        def __init__(self, ms_since_epoch: _Optional[int] = ...) -> None: ...
    MS_SINCE_EPOCH_FIELD_NUMBER: _ClassVar[int]
    SEVERITY_FIELD_NUMBER: _ClassVar[int]
    NODE_ID_FIELD_NUMBER: _ClassVar[int]
    ms_since_epoch: int
    severity: LogMetadata.Severity
    node_id: str
    def __init__(self, ms_since_epoch: _Optional[int] = ..., severity: _Optional[_Union[LogMetadata.Severity, str]] = ..., node_id: _Optional[str] = ...) -> None: ...
