from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional

DESCRIPTOR: _descriptor.FileDescriptor

class Version(_message.Message):
    __slots__ = ("major", "patch", "pre")
    MAJOR_FIELD_NUMBER: _ClassVar[int]
    PATCH_FIELD_NUMBER: _ClassVar[int]
    PRE_FIELD_NUMBER: _ClassVar[int]
    major: int
    patch: int
    pre: str
    def __init__(self, major: _Optional[int] = ..., patch: _Optional[int] = ..., pre: _Optional[str] = ...) -> None: ...
