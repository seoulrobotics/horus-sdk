from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class TestEnum(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
    __slots__ = ()
    TEST_ENUM_UNSPECIFIED: _ClassVar[TestEnum]
    TEST_ENUM_ONE: _ClassVar[TestEnum]
    TEST_ENUM_TWO: _ClassVar[TestEnum]
TEST_ENUM_UNSPECIFIED: TestEnum
TEST_ENUM_ONE: TestEnum
TEST_ENUM_TWO: TestEnum

class TestMessage(_message.Message):
    __slots__ = ("float", "double", "i32", "i64", "u32", "u64", "bytes", "string", "submessage", "bool", "oneof_float", "oneof_double", "oneof_i32", "oneof_i64", "oneof_u32", "oneof_u64", "oneof_bytes", "oneof_string", "oneof_submessage", "oneof_bool", "rep_float", "rep_double", "rep_i32", "rep_i64", "rep_u32", "rep_u64", "rep_bytes", "rep_string", "rep_submessage", "rep_bool")
    class SubMessage(_message.Message):
        __slots__ = ("u32", "i32", "string")
        U32_FIELD_NUMBER: _ClassVar[int]
        I32_FIELD_NUMBER: _ClassVar[int]
        STRING_FIELD_NUMBER: _ClassVar[int]
        u32: int
        i32: int
        string: str
        def __init__(self, u32: _Optional[int] = ..., i32: _Optional[int] = ..., string: _Optional[str] = ...) -> None: ...
    FLOAT_FIELD_NUMBER: _ClassVar[int]
    DOUBLE_FIELD_NUMBER: _ClassVar[int]
    I32_FIELD_NUMBER: _ClassVar[int]
    I64_FIELD_NUMBER: _ClassVar[int]
    U32_FIELD_NUMBER: _ClassVar[int]
    U64_FIELD_NUMBER: _ClassVar[int]
    BYTES_FIELD_NUMBER: _ClassVar[int]
    STRING_FIELD_NUMBER: _ClassVar[int]
    SUBMESSAGE_FIELD_NUMBER: _ClassVar[int]
    BOOL_FIELD_NUMBER: _ClassVar[int]
    ONEOF_FLOAT_FIELD_NUMBER: _ClassVar[int]
    ONEOF_DOUBLE_FIELD_NUMBER: _ClassVar[int]
    ONEOF_I32_FIELD_NUMBER: _ClassVar[int]
    ONEOF_I64_FIELD_NUMBER: _ClassVar[int]
    ONEOF_U32_FIELD_NUMBER: _ClassVar[int]
    ONEOF_U64_FIELD_NUMBER: _ClassVar[int]
    ONEOF_BYTES_FIELD_NUMBER: _ClassVar[int]
    ONEOF_STRING_FIELD_NUMBER: _ClassVar[int]
    ONEOF_SUBMESSAGE_FIELD_NUMBER: _ClassVar[int]
    ONEOF_BOOL_FIELD_NUMBER: _ClassVar[int]
    REP_FLOAT_FIELD_NUMBER: _ClassVar[int]
    REP_DOUBLE_FIELD_NUMBER: _ClassVar[int]
    REP_I32_FIELD_NUMBER: _ClassVar[int]
    REP_I64_FIELD_NUMBER: _ClassVar[int]
    REP_U32_FIELD_NUMBER: _ClassVar[int]
    REP_U64_FIELD_NUMBER: _ClassVar[int]
    REP_BYTES_FIELD_NUMBER: _ClassVar[int]
    REP_STRING_FIELD_NUMBER: _ClassVar[int]
    REP_SUBMESSAGE_FIELD_NUMBER: _ClassVar[int]
    REP_BOOL_FIELD_NUMBER: _ClassVar[int]
    float: float
    double: float
    i32: int
    i64: int
    u32: int
    u64: int
    bytes: bytes
    string: str
    submessage: TestMessage.SubMessage
    bool: bool
    oneof_float: float
    oneof_double: float
    oneof_i32: int
    oneof_i64: int
    oneof_u32: int
    oneof_u64: int
    oneof_bytes: bytes
    oneof_string: str
    oneof_submessage: TestMessage.SubMessage
    oneof_bool: bool
    rep_float: _containers.RepeatedScalarFieldContainer[float]
    rep_double: _containers.RepeatedScalarFieldContainer[float]
    rep_i32: _containers.RepeatedScalarFieldContainer[int]
    rep_i64: _containers.RepeatedScalarFieldContainer[int]
    rep_u32: _containers.RepeatedScalarFieldContainer[int]
    rep_u64: _containers.RepeatedScalarFieldContainer[int]
    rep_bytes: _containers.RepeatedScalarFieldContainer[bytes]
    rep_string: _containers.RepeatedScalarFieldContainer[str]
    rep_submessage: _containers.RepeatedCompositeFieldContainer[TestMessage.SubMessage]
    rep_bool: _containers.RepeatedScalarFieldContainer[bool]
    def __init__(self, float: _Optional[float] = ..., double: _Optional[float] = ..., i32: _Optional[int] = ..., i64: _Optional[int] = ..., u32: _Optional[int] = ..., u64: _Optional[int] = ..., bytes: _Optional[bytes] = ..., string: _Optional[str] = ..., submessage: _Optional[_Union[TestMessage.SubMessage, _Mapping]] = ..., bool: bool = ..., oneof_float: _Optional[float] = ..., oneof_double: _Optional[float] = ..., oneof_i32: _Optional[int] = ..., oneof_i64: _Optional[int] = ..., oneof_u32: _Optional[int] = ..., oneof_u64: _Optional[int] = ..., oneof_bytes: _Optional[bytes] = ..., oneof_string: _Optional[str] = ..., oneof_submessage: _Optional[_Union[TestMessage.SubMessage, _Mapping]] = ..., oneof_bool: bool = ..., rep_float: _Optional[_Iterable[float]] = ..., rep_double: _Optional[_Iterable[float]] = ..., rep_i32: _Optional[_Iterable[int]] = ..., rep_i64: _Optional[_Iterable[int]] = ..., rep_u32: _Optional[_Iterable[int]] = ..., rep_u64: _Optional[_Iterable[int]] = ..., rep_bytes: _Optional[_Iterable[bytes]] = ..., rep_string: _Optional[_Iterable[str]] = ..., rep_submessage: _Optional[_Iterable[_Union[TestMessage.SubMessage, _Mapping]]] = ..., rep_bool: _Optional[_Iterable[bool]] = ...) -> None: ...
