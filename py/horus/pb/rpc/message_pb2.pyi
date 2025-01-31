from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class RpcMessage(_message.Message):
    __slots__ = ("version", "service_id", "message_bytes", "request_id", "method_id", "error", "cancel")
    class Version(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = ()
        VERSION_UNSPECIFIED: _ClassVar[RpcMessage.Version]
        VERSION_ONE: _ClassVar[RpcMessage.Version]
    VERSION_UNSPECIFIED: RpcMessage.Version
    VERSION_ONE: RpcMessage.Version
    VERSION_FIELD_NUMBER: _ClassVar[int]
    SERVICE_ID_FIELD_NUMBER: _ClassVar[int]
    MESSAGE_BYTES_FIELD_NUMBER: _ClassVar[int]
    REQUEST_ID_FIELD_NUMBER: _ClassVar[int]
    METHOD_ID_FIELD_NUMBER: _ClassVar[int]
    ERROR_FIELD_NUMBER: _ClassVar[int]
    CANCEL_FIELD_NUMBER: _ClassVar[int]
    version: RpcMessage.Version
    service_id: int
    message_bytes: bytes
    request_id: int
    method_id: int
    error: str
    cancel: bool
    def __init__(self, version: _Optional[_Union[RpcMessage.Version, str]] = ..., service_id: _Optional[int] = ..., message_bytes: _Optional[bytes] = ..., request_id: _Optional[int] = ..., method_id: _Optional[int] = ..., error: _Optional[str] = ..., cancel: bool = ...) -> None: ...
