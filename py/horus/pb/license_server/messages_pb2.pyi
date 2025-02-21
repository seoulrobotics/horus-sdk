from horus.pb.config import metadata_pb2 as _metadata_pb2
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class LicenseInfo(_message.Message):
    __slots__ = ("expiration_date", "lidar_count")
    EXPIRATION_DATE_FIELD_NUMBER: _ClassVar[int]
    LIDAR_COUNT_FIELD_NUMBER: _ClassVar[int]
    expiration_date: _metadata_pb2.Timestamp
    lidar_count: int
    def __init__(self, expiration_date: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ..., lidar_count: _Optional[int] = ...) -> None: ...
