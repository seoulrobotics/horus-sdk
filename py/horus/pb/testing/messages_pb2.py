# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: horus/pb/testing/messages.proto
# Protobuf Python Version: 5.28.3
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import descriptor_pool as _descriptor_pool
from google.protobuf import runtime_version as _runtime_version
from google.protobuf import symbol_database as _symbol_database
from google.protobuf.internal import builder as _builder
_runtime_version.ValidateProtobufRuntimeVersion(
    _runtime_version.Domain.PUBLIC,
    5,
    28,
    3,
    '',
    'horus/pb/testing/messages.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x1fhorus/pb/testing/messages.proto\x12\x08horus.pb\"\x93\x06\n\x0bTestMessage\x12\r\n\x05\x66loat\x18\x01 \x01(\x02\x12\x0e\n\x06\x64ouble\x18\x02 \x01(\x01\x12\x0b\n\x03i32\x18\x03 \x01(\x05\x12\x0b\n\x03i64\x18\x04 \x01(\x03\x12\x0b\n\x03u32\x18\x05 \x01(\r\x12\x0b\n\x03u64\x18\x06 \x01(\x04\x12\r\n\x05\x62ytes\x18\x07 \x01(\x0c\x12\x0e\n\x06string\x18\x08 \x01(\t\x12\x34\n\nsubmessage\x18\t \x01(\x0b\x32 .horus.pb.TestMessage.SubMessage\x12\x0c\n\x04\x62ool\x18\n \x01(\x08\x12\x15\n\x0boneof_float\x18\x0b \x01(\x02H\x00\x12\x16\n\x0coneof_double\x18\x0c \x01(\x01H\x00\x12\x13\n\toneof_i32\x18\r \x01(\x05H\x00\x12\x13\n\toneof_i64\x18\x0e \x01(\x03H\x00\x12\x13\n\toneof_u32\x18\x0f \x01(\rH\x00\x12\x13\n\toneof_u64\x18\x10 \x01(\x04H\x00\x12\x15\n\x0boneof_bytes\x18\x11 \x01(\x0cH\x00\x12\x16\n\x0coneof_string\x18\x12 \x01(\tH\x00\x12<\n\x10oneof_submessage\x18\x13 \x01(\x0b\x32 .horus.pb.TestMessage.SubMessageH\x00\x12\x14\n\noneof_bool\x18\x14 \x01(\x08H\x00\x12\x11\n\trep_float\x18\x15 \x03(\x02\x12\x12\n\nrep_double\x18\x16 \x03(\x01\x12\x0f\n\x07rep_i32\x18\x17 \x03(\x05\x12\x0f\n\x07rep_i64\x18\x18 \x03(\x03\x12\x0f\n\x07rep_u32\x18\x19 \x03(\r\x12\x0f\n\x07rep_u64\x18\x1a \x03(\x04\x12\x11\n\trep_bytes\x18\x1b \x03(\x0c\x12\x12\n\nrep_string\x18\x1c \x03(\t\x12\x38\n\x0erep_submessage\x18\x1d \x03(\x0b\x32 .horus.pb.TestMessage.SubMessage\x12\x10\n\x08rep_bool\x18\x1e \x03(\x08\x12$\n\x08rep_enum\x18\x1f \x03(\x0e\x32\x12.horus.pb.TestEnum\x1a\x36\n\nSubMessage\x12\x0b\n\x03u32\x18\x01 \x01(\r\x12\x0b\n\x03i32\x18\x02 \x01(\x05\x12\x0e\n\x06string\x18\x03 \x01(\tB\x0c\n\ntest_oneof*K\n\x08TestEnum\x12\x19\n\x15TEST_ENUM_UNSPECIFIED\x10\x00\x12\x11\n\rTEST_ENUM_ONE\x10\x01\x12\x11\n\rTEST_ENUM_TWO\x10\x02\x62\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'horus.pb.testing.messages_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_TESTENUM']._serialized_start=835
  _globals['_TESTENUM']._serialized_end=910
  _globals['_TESTMESSAGE']._serialized_start=46
  _globals['_TESTMESSAGE']._serialized_end=833
  _globals['_TESTMESSAGE_SUBMESSAGE']._serialized_start=765
  _globals['_TESTMESSAGE_SUBMESSAGE']._serialized_end=819
# @@protoc_insertion_point(module_scope)
