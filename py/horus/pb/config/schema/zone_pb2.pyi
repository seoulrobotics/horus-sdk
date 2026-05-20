from horus.pb.config import metadata_pb2 as _metadata_pb2
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class ZoneType(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    ZONE_TYPE_UNSPECIFIED: _ClassVar[ZoneType]
    EXCLUSION: _ClassVar[ZoneType]
    STATIC_DETECTION: _ClassVar[ZoneType]
    REFLECTION: _ClassVar[ZoneType]
    STATIC_EXCLUSION: _ClassVar[ZoneType]
    EVENT: _ClassVar[ZoneType]
ZONE_TYPE_UNSPECIFIED: ZoneType
EXCLUSION: ZoneType
STATIC_DETECTION: ZoneType
REFLECTION: ZoneType
STATIC_EXCLUSION: ZoneType
EVENT: ZoneType

class Zone(_message.Message):
    __slots__ = ("zone_id", "status", "type", "name", "z_range", "vertices")
    ZONE_ID_FIELD_NUMBER: _ClassVar[int]
    STATUS_FIELD_NUMBER: _ClassVar[int]
    TYPE_FIELD_NUMBER: _ClassVar[int]
    NAME_FIELD_NUMBER: _ClassVar[int]
    Z_RANGE_FIELD_NUMBER: _ClassVar[int]
    VERTICES_FIELD_NUMBER: _ClassVar[int]
    zone_id: str
    status: _metadata_pb2.RepeatedMessageStatus
    type: ZoneType
    name: str
    z_range: _metadata_pb2.Range
    vertices: _metadata_pb2.Vector2dList
    def __init__(self, zone_id: _Optional[str] = ..., status: _Optional[_Union[_metadata_pb2.RepeatedMessageStatus, str]] = ..., type: _Optional[_Union[ZoneType, str]] = ..., name: _Optional[str] = ..., z_range: _Optional[_Union[_metadata_pb2.Range, _Mapping]] = ..., vertices: _Optional[_Union[_metadata_pb2.Vector2dList, _Mapping]] = ...) -> None: ...
