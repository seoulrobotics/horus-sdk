# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: horus/pb/config/metadata.proto
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
    'horus/pb/config/metadata.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x1ehorus/pb/config/metadata.proto\x12\x08horus.pb\" \n\x08Vector2f\x12\t\n\x01x\x18\x01 \x01(\x02\x12\t\n\x01y\x18\x02 \x01(\x02\"+\n\x08Vector3f\x12\t\n\x01x\x18\x01 \x01(\x02\x12\t\n\x01y\x18\x02 \x01(\x02\x12\t\n\x01z\x18\x03 \x01(\x02\"#\n\x05Range\x12\r\n\x05start\x18\x01 \x01(\x01\x12\x0b\n\x03\x65nd\x18\x02 \x01(\x01\"\x1c\n\nUInt32List\x12\x0e\n\x06values\x18\x01 \x03(\r\"+\n\tTimestamp\x12\x0f\n\x07seconds\x18\x01 \x01(\x03\x12\r\n\x05nanos\x18\x02 \x01(\x05\"*\n\x08\x44uration\x12\x0f\n\x07seconds\x18\x01 \x01(\x03\x12\r\n\x05nanos\x18\x02 \x01(\x05\"!\n\x11\x41\x66\x66ineTransform3f\x12\x0c\n\x04\x64\x61ta\x18\x01 \x03(\x02\x62\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'horus.pb.config.metadata_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_VECTOR2F']._serialized_start=44
  _globals['_VECTOR2F']._serialized_end=76
  _globals['_VECTOR3F']._serialized_start=78
  _globals['_VECTOR3F']._serialized_end=121
  _globals['_RANGE']._serialized_start=123
  _globals['_RANGE']._serialized_end=158
  _globals['_UINT32LIST']._serialized_start=160
  _globals['_UINT32LIST']._serialized_end=188
  _globals['_TIMESTAMP']._serialized_start=190
  _globals['_TIMESTAMP']._serialized_end=233
  _globals['_DURATION']._serialized_start=235
  _globals['_DURATION']._serialized_end=277
  _globals['_AFFINETRANSFORM3F']._serialized_start=279
  _globals['_AFFINETRANSFORM3F']._serialized_end=312
# @@protoc_insertion_point(module_scope)
