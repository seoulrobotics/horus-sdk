# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: horus/pb/config/schema/general.proto
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
    'horus/pb/config/schema/general.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from horus.pb.config import metadata_pb2 as horus_dot_pb_dot_config_dot_metadata__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n$horus/pb/config/schema/general.proto\x12\x08horus.pb\x1a\x1ehorus/pb/config/metadata.proto\"\xa9\x01\n\x0e\x44\x65tectionRange\x12%\n\x07x_range\x18\x01 \x01(\x0b\x32\x0f.horus.pb.RangeH\x00\x88\x01\x01\x12%\n\x07y_range\x18\x02 \x01(\x0b\x32\x0f.horus.pb.RangeH\x01\x88\x01\x01\x12%\n\x07z_range\x18\x03 \x01(\x0b\x32\x0f.horus.pb.RangeH\x02\x88\x01\x01\x42\n\n\x08_x_rangeB\n\n\x08_y_rangeB\n\n\x08_z_rangeb\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'horus.pb.config.schema.general_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_DETECTIONRANGE']._serialized_start=83
  _globals['_DETECTIONRANGE']._serialized_end=252
# @@protoc_insertion_point(module_scope)
