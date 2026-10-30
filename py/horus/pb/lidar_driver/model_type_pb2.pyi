from horus.pb.config.schema.lidar import hesai_pb2 as _hesai_pb2
from horus.pb.config.schema.lidar import misc_pb2 as _misc_pb2
from horus.pb.config.schema.lidar import ros2_topic_pb2 as _ros2_topic_pb2
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class LidarModelType(_message.Message):
    __slots__ = ("misc", "hesai", "ros2_topic")
    MISC_FIELD_NUMBER: _ClassVar[int]
    HESAI_FIELD_NUMBER: _ClassVar[int]
    ROS2_TOPIC_FIELD_NUMBER: _ClassVar[int]
    misc: _misc_pb2.MiscLidarConfig.Model
    hesai: _hesai_pb2.HesaiLidarConfig.Model
    ros2_topic: _ros2_topic_pb2.Ros2TopicLidarConfig.Model
    def __init__(self, misc: _Optional[_Union[_misc_pb2.MiscLidarConfig.Model, str]] = ..., hesai: _Optional[_Union[_hesai_pb2.HesaiLidarConfig.Model, str]] = ..., ros2_topic: _Optional[_Union[_ros2_topic_pb2.Ros2TopicLidarConfig.Model, str]] = ...) -> None: ...
