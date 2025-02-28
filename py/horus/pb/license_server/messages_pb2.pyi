from horus.pb.config import metadata_pb2 as _metadata_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class LicenseFeature(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    LICENSE_FEATURE_CAN: _ClassVar[LicenseFeature]
    LICENSE_FEATURE_VIN_ASSOCIATOR: _ClassVar[LicenseFeature]
    LICENSE_FEATURE_MULTI_ROSBAG: _ClassVar[LicenseFeature]
    LICENSE_FEATURE_DEBUGGING_SUPPORT: _ClassVar[LicenseFeature]
    LICENSE_FEATURE_ATLAS: _ClassVar[LicenseFeature]
    LICENSE_FEATURE_MACGYVER: _ClassVar[LicenseFeature]
LICENSE_FEATURE_CAN: LicenseFeature
LICENSE_FEATURE_VIN_ASSOCIATOR: LicenseFeature
LICENSE_FEATURE_MULTI_ROSBAG: LicenseFeature
LICENSE_FEATURE_DEBUGGING_SUPPORT: LicenseFeature
LICENSE_FEATURE_ATLAS: LicenseFeature
LICENSE_FEATURE_MACGYVER: LicenseFeature

class LicenseInfo(_message.Message):
    __slots__ = ("expiration_date", "lidar_count", "allowed_features")
    class AllowedFeature(_message.Message):
        __slots__ = ("feature",)
        FEATURE_FIELD_NUMBER: _ClassVar[int]
        feature: LicenseFeature
        def __init__(self, feature: _Optional[_Union[LicenseFeature, str]] = ...) -> None: ...
    EXPIRATION_DATE_FIELD_NUMBER: _ClassVar[int]
    LIDAR_COUNT_FIELD_NUMBER: _ClassVar[int]
    ALLOWED_FEATURES_FIELD_NUMBER: _ClassVar[int]
    expiration_date: _metadata_pb2.Timestamp
    lidar_count: int
    allowed_features: _containers.RepeatedCompositeFieldContainer[LicenseInfo.AllowedFeature]
    def __init__(self, expiration_date: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ..., lidar_count: _Optional[int] = ..., allowed_features: _Optional[_Iterable[_Union[LicenseInfo.AllowedFeature, _Mapping]]] = ...) -> None: ...
