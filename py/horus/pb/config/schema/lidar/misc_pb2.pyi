from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar

DESCRIPTOR: _descriptor.FileDescriptor

class MiscLidarConfig(_message.Message):
    __slots__ = ()
    class Model(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = ()
        MODEL_UNSPECIFIED: _ClassVar[MiscLidarConfig.Model]
        HORUS_BAG_LIDAR: _ClassVar[MiscLidarConfig.Model]
    MODEL_UNSPECIFIED: MiscLidarConfig.Model
    HORUS_BAG_LIDAR: MiscLidarConfig.Model
    def __init__(self) -> None: ...
