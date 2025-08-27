from horus.pb.config import metadata_pb2 as _metadata_pb2
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class DetectionRange(_message.Message):
    __slots__ = ("x_range", "y_range", "z_range")
    X_RANGE_FIELD_NUMBER: _ClassVar[int]
    Y_RANGE_FIELD_NUMBER: _ClassVar[int]
    Z_RANGE_FIELD_NUMBER: _ClassVar[int]
    x_range: _metadata_pb2.Range
    y_range: _metadata_pb2.Range
    z_range: _metadata_pb2.Range
    def __init__(self, x_range: _Optional[_Union[_metadata_pb2.Range, _Mapping]] = ..., y_range: _Optional[_Union[_metadata_pb2.Range, _Mapping]] = ..., z_range: _Optional[_Union[_metadata_pb2.Range, _Mapping]] = ...) -> None: ...
