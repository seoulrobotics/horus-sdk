from google.protobuf import descriptor_pb2 as _descriptor_pb2
from horus.pb.logs import logs_pb2 as _logs_pb2
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor
SERVICE_OPTIONS_FIELD_NUMBER: _ClassVar[int]
service_options: _descriptor.FieldDescriptor
METHOD_OPTIONS_FIELD_NUMBER: _ClassVar[int]
method_options: _descriptor.FieldDescriptor

class NoResponse(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class RpcServiceOptions(_message.Message):
    __slots__ = ("id", "description")
    ID_FIELD_NUMBER: _ClassVar[int]
    DESCRIPTION_FIELD_NUMBER: _ClassVar[int]
    id: int
    description: str
    def __init__(self, id: _Optional[int] = ..., description: _Optional[str] = ...) -> None: ...

class RpcMethodOptions(_message.Message):
    __slots__ = ("id", "description", "subscribe", "unsubscribe")
    class SubscribeMethod(_message.Message):
        __slots__ = ("subscriber_name",)
        SUBSCRIBER_NAME_FIELD_NUMBER: _ClassVar[int]
        subscriber_name: str
        def __init__(self, subscriber_name: _Optional[str] = ...) -> None: ...
    class UnsubscribeMethod(_message.Message):
        __slots__ = ("subscriber_name",)
        SUBSCRIBER_NAME_FIELD_NUMBER: _ClassVar[int]
        subscriber_name: str
        def __init__(self, subscriber_name: _Optional[str] = ...) -> None: ...
    ID_FIELD_NUMBER: _ClassVar[int]
    DESCRIPTION_FIELD_NUMBER: _ClassVar[int]
    SUBSCRIBE_FIELD_NUMBER: _ClassVar[int]
    UNSUBSCRIBE_FIELD_NUMBER: _ClassVar[int]
    id: int
    description: str
    subscribe: RpcMethodOptions.SubscribeMethod
    unsubscribe: RpcMethodOptions.UnsubscribeMethod
    def __init__(self, id: _Optional[int] = ..., description: _Optional[str] = ..., subscribe: _Optional[_Union[RpcMethodOptions.SubscribeMethod, _Mapping]] = ..., unsubscribe: _Optional[_Union[RpcMethodOptions.UnsubscribeMethod, _Mapping]] = ...) -> None: ...

class DefaultSubscribeRequest(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class DefaultSubscribeResponse(_message.Message):
    __slots__ = ("connection_error",)
    CONNECTION_ERROR_FIELD_NUMBER: _ClassVar[int]
    connection_error: _logs_pb2.RpcConnectionError
    def __init__(self, connection_error: _Optional[_Union[_logs_pb2.RpcConnectionError, _Mapping]] = ...) -> None: ...

class DefaultUnsubscribeRequest(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class DefaultUnsubscribeResponse(_message.Message):
    __slots__ = ("disconnection_error",)
    DISCONNECTION_ERROR_FIELD_NUMBER: _ClassVar[int]
    disconnection_error: _logs_pb2.RpcDisconnectionError
    def __init__(self, disconnection_error: _Optional[_Union[_logs_pb2.RpcDisconnectionError, _Mapping]] = ...) -> None: ...
