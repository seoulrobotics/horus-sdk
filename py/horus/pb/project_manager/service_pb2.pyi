from horus.pb.logs import logs_pb2 as _logs_pb2
from horus.pb.preprocessing import messages_pb2 as _messages_pb2
from horus.pb.license_server import messages_pb2 as _messages_pb2_1
from horus.pb import rpc_pb2 as _rpc_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class LicensePrivilege(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    LICENSE_PRIVILEGE_UNSPECIFIED: _ClassVar[LicensePrivilege]
    LOWEST: _ClassVar[LicensePrivilege]
    SUBSCRIBE: _ClassVar[LicensePrivilege]
    LICENSE_ACTIONS: _ClassVar[LicensePrivilege]
    READ: _ClassVar[LicensePrivilege]
    WRITE: _ClassVar[LicensePrivilege]
LICENSE_PRIVILEGE_UNSPECIFIED: LicensePrivilege
LOWEST: LicensePrivilege
SUBSCRIBE: LicensePrivilege
LICENSE_ACTIONS: LicensePrivilege
READ: LicensePrivilege
WRITE: LicensePrivilege

class LicenseLevel(_message.Message):
    __slots__ = ("level_1", "level_2", "level_3", "level_4", "privilege")
    class Level1(_message.Message):
        __slots__ = ("license_server_unreachable",)
        LICENSE_SERVER_UNREACHABLE_FIELD_NUMBER: _ClassVar[int]
        license_server_unreachable: _logs_pb2.LicenseServerConnectionError
        def __init__(self, license_server_unreachable: _Optional[_Union[_logs_pb2.LicenseServerConnectionError, _Mapping]] = ...) -> None: ...
    class Level2(_message.Message):
        __slots__ = ("license_not_found",)
        LICENSE_NOT_FOUND_FIELD_NUMBER: _ClassVar[int]
        license_not_found: _logs_pb2.LicenseNotFoundError
        def __init__(self, license_not_found: _Optional[_Union[_logs_pb2.LicenseNotFoundError, _Mapping]] = ...) -> None: ...
    class Level3(_message.Message):
        __slots__ = ("license_expired", "license_server_disconnected")
        LICENSE_EXPIRED_FIELD_NUMBER: _ClassVar[int]
        LICENSE_SERVER_DISCONNECTED_FIELD_NUMBER: _ClassVar[int]
        license_expired: _logs_pb2.LicenseExpiredError
        license_server_disconnected: _logs_pb2.LicenseServerConnectionError
        def __init__(self, license_expired: _Optional[_Union[_logs_pb2.LicenseExpiredError, _Mapping]] = ..., license_server_disconnected: _Optional[_Union[_logs_pb2.LicenseServerConnectionError, _Mapping]] = ...) -> None: ...
    class Level4(_message.Message):
        __slots__ = ("license_active",)
        LICENSE_ACTIVE_FIELD_NUMBER: _ClassVar[int]
        license_active: _logs_pb2.LicenseActiveInfo
        def __init__(self, license_active: _Optional[_Union[_logs_pb2.LicenseActiveInfo, _Mapping]] = ...) -> None: ...
    LEVEL_1_FIELD_NUMBER: _ClassVar[int]
    LEVEL_2_FIELD_NUMBER: _ClassVar[int]
    LEVEL_3_FIELD_NUMBER: _ClassVar[int]
    LEVEL_4_FIELD_NUMBER: _ClassVar[int]
    PRIVILEGE_FIELD_NUMBER: _ClassVar[int]
    level_1: LicenseLevel.Level1
    level_2: LicenseLevel.Level2
    level_3: LicenseLevel.Level3
    level_4: LicenseLevel.Level4
    privilege: int
    def __init__(self, level_1: _Optional[_Union[LicenseLevel.Level1, _Mapping]] = ..., level_2: _Optional[_Union[LicenseLevel.Level2, _Mapping]] = ..., level_3: _Optional[_Union[LicenseLevel.Level3, _Mapping]] = ..., level_4: _Optional[_Union[LicenseLevel.Level4, _Mapping]] = ..., privilege: _Optional[int] = ...) -> None: ...

class LicenseStatus(_message.Message):
    __slots__ = ("license_level", "license_info")
    LICENSE_LEVEL_FIELD_NUMBER: _ClassVar[int]
    LICENSE_INFO_FIELD_NUMBER: _ClassVar[int]
    license_level: LicenseLevel
    license_info: _messages_pb2_1.LicenseInfo
    def __init__(self, license_level: _Optional[_Union[LicenseLevel, _Mapping]] = ..., license_info: _Optional[_Union[_messages_pb2_1.LicenseInfo, _Mapping]] = ...) -> None: ...

class GetHealthStatusRequest(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class GetHealthStatusResponse(_message.Message):
    __slots__ = ("license_status", "sensor_statuses", "service_statuses", "node_resources")
    class NodeHealth(_message.Message):
        __slots__ = ("service", "node_id", "node_status")
        class Service(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
            __slots__ = ()
            SERVICE_UNSPECIFIED: _ClassVar[GetHealthStatusResponse.NodeHealth.Service]
            SERVICE_DETECTION: _ClassVar[GetHealthStatusResponse.NodeHealth.Service]
            SERVICE_LIDAR_RUNNER: _ClassVar[GetHealthStatusResponse.NodeHealth.Service]
            SERVICE_NOTIFICATION: _ClassVar[GetHealthStatusResponse.NodeHealth.Service]
            SERVICE_POINT_AGGREGATOR: _ClassVar[GetHealthStatusResponse.NodeHealth.Service]
            SERVICE_PREPROCESSING: _ClassVar[GetHealthStatusResponse.NodeHealth.Service]
            SERVICE_PROJECT_MANAGER: _ClassVar[GetHealthStatusResponse.NodeHealth.Service]
        SERVICE_UNSPECIFIED: GetHealthStatusResponse.NodeHealth.Service
        SERVICE_DETECTION: GetHealthStatusResponse.NodeHealth.Service
        SERVICE_LIDAR_RUNNER: GetHealthStatusResponse.NodeHealth.Service
        SERVICE_NOTIFICATION: GetHealthStatusResponse.NodeHealth.Service
        SERVICE_POINT_AGGREGATOR: GetHealthStatusResponse.NodeHealth.Service
        SERVICE_PREPROCESSING: GetHealthStatusResponse.NodeHealth.Service
        SERVICE_PROJECT_MANAGER: GetHealthStatusResponse.NodeHealth.Service
        class Status(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
            __slots__ = ()
            STATUS_UNSPECIFIED: _ClassVar[GetHealthStatusResponse.NodeHealth.Status]
            STATUS_UNREACHABLE: _ClassVar[GetHealthStatusResponse.NodeHealth.Status]
            STATUS_ALIVE: _ClassVar[GetHealthStatusResponse.NodeHealth.Status]
        STATUS_UNSPECIFIED: GetHealthStatusResponse.NodeHealth.Status
        STATUS_UNREACHABLE: GetHealthStatusResponse.NodeHealth.Status
        STATUS_ALIVE: GetHealthStatusResponse.NodeHealth.Status
        SERVICE_FIELD_NUMBER: _ClassVar[int]
        NODE_ID_FIELD_NUMBER: _ClassVar[int]
        NODE_STATUS_FIELD_NUMBER: _ClassVar[int]
        service: GetHealthStatusResponse.NodeHealth.Service
        node_id: str
        node_status: GetHealthStatusResponse.NodeHealth.Status
        def __init__(self, service: _Optional[_Union[GetHealthStatusResponse.NodeHealth.Service, str]] = ..., node_id: _Optional[str] = ..., node_status: _Optional[_Union[GetHealthStatusResponse.NodeHealth.Status, str]] = ...) -> None: ...
    class SensorHealth(_message.Message):
        __slots__ = ("preprocessing_node_id", "info", "timeout")
        PREPROCESSING_NODE_ID_FIELD_NUMBER: _ClassVar[int]
        INFO_FIELD_NUMBER: _ClassVar[int]
        TIMEOUT_FIELD_NUMBER: _ClassVar[int]
        preprocessing_node_id: str
        info: _messages_pb2.SensorInfo
        timeout: _logs_pb2.ServiceConnectionTimedOut
        def __init__(self, preprocessing_node_id: _Optional[str] = ..., info: _Optional[_Union[_messages_pb2.SensorInfo, _Mapping]] = ..., timeout: _Optional[_Union[_logs_pb2.ServiceConnectionTimedOut, _Mapping]] = ...) -> None: ...
    class NodeResources(_message.Message):
        __slots__ = ()
        def __init__(self) -> None: ...
    LICENSE_STATUS_FIELD_NUMBER: _ClassVar[int]
    SENSOR_STATUSES_FIELD_NUMBER: _ClassVar[int]
    SERVICE_STATUSES_FIELD_NUMBER: _ClassVar[int]
    NODE_RESOURCES_FIELD_NUMBER: _ClassVar[int]
    license_status: LicenseStatus
    sensor_statuses: _containers.RepeatedCompositeFieldContainer[GetHealthStatusResponse.SensorHealth]
    service_statuses: _containers.RepeatedCompositeFieldContainer[GetHealthStatusResponse.NodeHealth]
    node_resources: _containers.RepeatedCompositeFieldContainer[GetHealthStatusResponse.NodeResources]
    def __init__(self, license_status: _Optional[_Union[LicenseStatus, _Mapping]] = ..., sensor_statuses: _Optional[_Iterable[_Union[GetHealthStatusResponse.SensorHealth, _Mapping]]] = ..., service_statuses: _Optional[_Iterable[_Union[GetHealthStatusResponse.NodeHealth, _Mapping]]] = ..., node_resources: _Optional[_Iterable[_Union[GetHealthStatusResponse.NodeResources, _Mapping]]] = ...) -> None: ...
