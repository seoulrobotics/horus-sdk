from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar

DESCRIPTOR: _descriptor.FileDescriptor

class Ros2TopicLidarConfig(_message.Message):
    __slots__ = ()
    class Model(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = ()
        MODEL_UNSPECIFIED: _ClassVar[Ros2TopicLidarConfig.Model]
        ROS2_TOPIC: _ClassVar[Ros2TopicLidarConfig.Model]
    MODEL_UNSPECIFIED: Ros2TopicLidarConfig.Model
    ROS2_TOPIC: Ros2TopicLidarConfig.Model
    def __init__(self) -> None: ...
