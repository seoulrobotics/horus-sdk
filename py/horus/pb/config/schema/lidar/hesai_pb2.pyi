from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar

DESCRIPTOR: _descriptor.FileDescriptor

class HesaiLidarConfig(_message.Message):
    __slots__ = ()
    class Model(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = ()
        MODEL_UNSPECIFIED: _ClassVar[HesaiLidarConfig.Model]
        XT16: _ClassVar[HesaiLidarConfig.Model]
        XT32: _ClassVar[HesaiLidarConfig.Model]
        OT128: _ClassVar[HesaiLidarConfig.Model]
        JT128: _ClassVar[HesaiLidarConfig.Model]
        QT128: _ClassVar[HesaiLidarConfig.Model]
    MODEL_UNSPECIFIED: HesaiLidarConfig.Model
    XT16: HesaiLidarConfig.Model
    XT32: HesaiLidarConfig.Model
    OT128: HesaiLidarConfig.Model
    JT128: HesaiLidarConfig.Model
    QT128: HesaiLidarConfig.Model
    def __init__(self) -> None: ...
