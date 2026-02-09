from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Optional as _Optional

DESCRIPTOR: _descriptor.FileDescriptor

class CalibrationMap(_message.Message):
    __slots__ = ("map_points", "intensities_bytes")
    MAP_POINTS_FIELD_NUMBER: _ClassVar[int]
    INTENSITIES_BYTES_FIELD_NUMBER: _ClassVar[int]
    map_points: _containers.RepeatedScalarFieldContainer[float]
    intensities_bytes: bytes
    def __init__(self, map_points: _Optional[_Iterable[float]] = ..., intensities_bytes: _Optional[bytes] = ...) -> None: ...
