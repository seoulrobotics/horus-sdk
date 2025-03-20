from horus.pb import rpc_pb2 as _rpc_pb2
from horus.pb.status_service import messages_pb2 as _messages_pb2
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class GetVersionRequest(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class GetVersionResponse(_message.Message):
    __slots__ = ("version",)
    VERSION_FIELD_NUMBER: _ClassVar[int]
    version: _messages_pb2.Version
    def __init__(self, version: _Optional[_Union[_messages_pb2.Version, _Mapping]] = ...) -> None: ...
