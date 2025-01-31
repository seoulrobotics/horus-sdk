from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional

DESCRIPTOR: _descriptor.FileDescriptor

class ValidationError(_message.Message):
    __slots__ = ("path", "message")
    PATH_FIELD_NUMBER: _ClassVar[int]
    MESSAGE_FIELD_NUMBER: _ClassVar[int]
    path: str
    message: str
    def __init__(self, path: _Optional[str] = ..., message: _Optional[str] = ...) -> None: ...
