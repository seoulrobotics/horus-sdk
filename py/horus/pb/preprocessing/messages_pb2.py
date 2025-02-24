# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: horus/pb/preprocessing/messages.proto
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
    'horus/pb/preprocessing/messages.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from horus.pb.config import metadata_pb2 as horus_dot_pb_dot_config_dot_metadata__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n%horus/pb/preprocessing/messages.proto\x12\x08horus.pb\x1a\x1ehorus/pb/config/metadata.proto\":\n\rOccupancyGrid\x12\x0c\n\x04rows\x18\x01 \x01(\r\x12\x0c\n\x04\x63ols\x18\x02 \x01(\r\x12\r\n\x05\x63\x65lls\x18\x03 \x03(\r\"\x8b\x01\n\x12OccupancyGridEvent\x12%\n\x04grid\x18\x01 \x01(\x0b\x32\x17.horus.pb.OccupancyGrid\x12\r\n\x05x_min\x18\x02 \x01(\x02\x12\r\n\x05x_max\x18\x03 \x01(\x02\x12\r\n\x05y_min\x18\x04 \x01(\x02\x12\r\n\x05y_max\x18\x05 \x01(\x02\x12\x12\n\nresolution\x18\x06 \x01(\x02\"\x85\x02\n\nSensorInfo\x12\x10\n\x08lidar_id\x18\x01 \x01(\t\x12\x0e\n\x06status\x18\x02 \x01(\r\x12\x1a\n\x12measured_frequency\x18\x03 \x01(\x01\x12\x41\n\x0fpose_correction\x18\x04 \x01(\x0b\x32#.horus.pb.SensorInfo.PoseCorrectionH\x00\x88\x01\x01\x1a\x62\n\x0ePoseCorrection\x12\'\n\x0btranslation\x18\x01 \x01(\x0b\x32\x12.horus.pb.Vector3f\x12\'\n\x08rotation\x18\x02 \x01(\x0b\x32\x15.horus.pb.QuaterniondB\x12\n\x10_pose_correction*\x81\x01\n\x17OccupancyClassification\x12\'\n#OCCUPANCYCLASSIFICATION_UNSPECIFIED\x10\x00\x12\x08\n\x04\x46REE\x10\x01\x12\x0c\n\x08OCCLUDED\x10\x02\x12\x17\n\x13STATIONARY_OCCUPIED\x10\x03\x12\x0c\n\x08\x45XCLUDED\x10\x04*\x91\x01\n\x0cSensorStatus\x12\x1d\n\x19SENSOR_STATUS_UNSPECIFIED\x10\x00\x12\x0b\n\x07NO_DATA\x10\x01\x12\x12\n\x0eRECEIVING_DATA\x10\x02\x12\x11\n\rLOW_FREQUENCY\x10\x04\x12\x12\n\x0eHIGH_FREQUENCY\x10\x08\x12\n\n\x06TILTED\x10\x10\x12\x0e\n\nOBSTRUCTED\x10 b\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'horus.pb.preprocessing.messages_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_OCCUPANCYCLASSIFICATION']._serialized_start=550
  _globals['_OCCUPANCYCLASSIFICATION']._serialized_end=679
  _globals['_SENSORSTATUS']._serialized_start=682
  _globals['_SENSORSTATUS']._serialized_end=827
  _globals['_OCCUPANCYGRID']._serialized_start=83
  _globals['_OCCUPANCYGRID']._serialized_end=141
  _globals['_OCCUPANCYGRIDEVENT']._serialized_start=144
  _globals['_OCCUPANCYGRIDEVENT']._serialized_end=283
  _globals['_SENSORINFO']._serialized_start=286
  _globals['_SENSORINFO']._serialized_end=547
  _globals['_SENSORINFO_POSECORRECTION']._serialized_start=429
  _globals['_SENSORINFO_POSECORRECTION']._serialized_end=527
# @@protoc_insertion_point(module_scope)
