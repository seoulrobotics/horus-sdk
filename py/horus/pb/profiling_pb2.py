# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# NO CHECKED-IN PROTOBUF GENCODE
# source: horus/pb/profiling.proto
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
    'horus/pb/profiling.proto'
)
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()


from horus.pb.config import metadata_pb2 as horus_dot_pb_dot_config_dot_metadata__pb2


DESCRIPTOR = _descriptor_pool.Default().AddSerializedFile(b'\n\x18horus/pb/profiling.proto\x12\x08horus.pb\x1a\x1ehorus/pb/config/metadata.proto\"\x85\x07\n\x0cProfilingSet\x12@\n\x10profiled_service\x18\x01 \x01(\x0e\x32&.horus.pb.ProfilingSet.ProfiledService\x12I\n\x10processing_times\x18\x02 \x03(\x0b\x32/.horus.pb.ProfilingSet.ProfiledDurationMapEntry\x12\x41\n\x0eresource_usage\x18\x03 \x01(\x0b\x32$.horus.pb.ProfilingSet.ResourceUsageH\x00\x88\x01\x01\x1a\x8d\x03\n\x10ProfiledDuration\x12$\n\x08\x64uration\x18\x01 \x01(\x0b\x32\x12.horus.pb.Duration\x12R\n\x11performance_hints\x18\x02 \x03(\x0b\x32\x37.horus.pb.ProfilingSet.ProfiledDuration.PerformanceHint\x1a\xfe\x01\n\x0fPerformanceHint\x12n\n\x17\x63onfig_parameter_action\x18\x01 \x01(\x0e\x32M.horus.pb.ProfilingSet.ProfiledDuration.PerformanceHint.ConfigParameterAction\x12\x1d\n\x15\x63onfig_parameter_name\x18\x02 \x01(\t\"\\\n\x15\x43onfigParameterAction\x12\'\n#CONFIG_PARAMETER_ACTION_UNSPECIFIED\x10\x00\x12\x0c\n\x08INCREASE\x10\x01\x12\x0c\n\x08\x44\x45\x43REASE\x10\x02\x1a_\n\x18ProfiledDurationMapEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12\x36\n\x05value\x18\x02 \x01(\x0b\x32\'.horus.pb.ProfilingSet.ProfiledDuration\x1a\x43\n\rResourceUsage\x12\x1c\n\x14\x63pu_usage_percentage\x18\x01 \x01(\r\x12\x14\n\x0cmemory_usage\x18\x02 \x01(\x04\"\\\n\x0fProfiledService\x12\x17\n\x13SERVICE_UNSPECIFIED\x10\x00\x12\x19\n\x15PREPROCESSING_SERVICE\x10\x01\x12\x15\n\x11\x44\x45TECTION_SERVICE\x10\x02\x42\x11\n\x0f_resource_usage\"\xf2\x01\n\x10ServiceProfiling\x12\x35\n\x15\x64\x65tails_profiling_set\x18\x01 \x01(\x0b\x32\x16.horus.pb.ProfilingSet\x12\x31\n\x15total_service_latency\x18\x02 \x01(\x0b\x32\x12.horus.pb.Duration\x12\x37\n\x1bidle_time_before_processing\x18\x04 \x01(\x0b\x32\x12.horus.pb.Duration\x12\x35\n\x19intra_component_idle_time\x18\x05 \x01(\x0b\x32\x12.horus.pb.DurationJ\x04\x08\x03\x10\x04\"\x99\x01\n\'PreprocessingServicePointCloudProfiling\x12\x35\n\x11service_profiling\x18\x01 \x01(\x0b\x32\x1a.horus.pb.ServiceProfiling\x12\x37\n\x1bpoint_cloud_sending_latency\x18\x02 \x01(\x0b\x32\x12.horus.pb.Duration\"w\n\x0e\x46rameProfiling\x12\x31\n\x15overall_frame_latency\x18\x01 \x01(\x0b\x32\x12.horus.pb.Duration\x12\x32\n\x16\x66rame_bundling_latency\x18\x02 \x01(\x0b\x32\x12.horus.pb.Duration\"\xdb\x03\n\x18\x42undledFrameProfilingSet\x12,\n\x0f\x66rame_timestamp\x18\x03 \x01(\x0b\x32\x13.horus.pb.Timestamp\x12\x31\n\x0f\x66rame_profiling\x18\x04 \x01(\x0b\x32\x18.horus.pb.FrameProfiling\x12?\n\x1b\x64\x65tection_service_profiling\x18\x05 \x01(\x0b\x32\x1a.horus.pb.ServiceProfiling\x12\x87\x01\n+preprocessing_service_point_cloud_profiling\x18\x07 \x03(\x0b\x32R.horus.pb.BundledFrameProfilingSet.PreprocessingServicePointCloudProfilingMapEntry\x1a\x80\x01\n/PreprocessingServicePointCloudProfilingMapEntry\x12\x0b\n\x03key\x18\x01 \x01(\t\x12@\n\x05value\x18\x02 \x01(\x0b\x32\x31.horus.pb.PreprocessingServicePointCloudProfilingJ\x04\x08\x01\x10\x02J\x04\x08\x02\x10\x03J\x04\x08\x06\x10\x07\"\xa4\x01\n\rProfilingInfo\x12\x37\n\x15general_profiling_set\x18\x01 \x01(\x0b\x32\x16.horus.pb.ProfilingSetH\x00\x12I\n\x1b\x62undled_frame_profiling_set\x18\x02 \x01(\x0b\x32\".horus.pb.BundledFrameProfilingSetH\x00\x42\x0f\n\rprofiling_setb\x06proto3')

_globals = globals()
_builder.BuildMessageAndEnumDescriptors(DESCRIPTOR, _globals)
_builder.BuildTopDescriptorsAndMessages(DESCRIPTOR, 'horus.pb.profiling_pb2', _globals)
if not _descriptor._USE_C_DESCRIPTORS:
  DESCRIPTOR._loaded_options = None
  _globals['_PROFILINGSET']._serialized_start=71
  _globals['_PROFILINGSET']._serialized_end=972
  _globals['_PROFILINGSET_PROFILEDDURATION']._serialized_start=296
  _globals['_PROFILINGSET_PROFILEDDURATION']._serialized_end=693
  _globals['_PROFILINGSET_PROFILEDDURATION_PERFORMANCEHINT']._serialized_start=439
  _globals['_PROFILINGSET_PROFILEDDURATION_PERFORMANCEHINT']._serialized_end=693
  _globals['_PROFILINGSET_PROFILEDDURATION_PERFORMANCEHINT_CONFIGPARAMETERACTION']._serialized_start=601
  _globals['_PROFILINGSET_PROFILEDDURATION_PERFORMANCEHINT_CONFIGPARAMETERACTION']._serialized_end=693
  _globals['_PROFILINGSET_PROFILEDDURATIONMAPENTRY']._serialized_start=695
  _globals['_PROFILINGSET_PROFILEDDURATIONMAPENTRY']._serialized_end=790
  _globals['_PROFILINGSET_RESOURCEUSAGE']._serialized_start=792
  _globals['_PROFILINGSET_RESOURCEUSAGE']._serialized_end=859
  _globals['_PROFILINGSET_PROFILEDSERVICE']._serialized_start=861
  _globals['_PROFILINGSET_PROFILEDSERVICE']._serialized_end=953
  _globals['_SERVICEPROFILING']._serialized_start=975
  _globals['_SERVICEPROFILING']._serialized_end=1217
  _globals['_PREPROCESSINGSERVICEPOINTCLOUDPROFILING']._serialized_start=1220
  _globals['_PREPROCESSINGSERVICEPOINTCLOUDPROFILING']._serialized_end=1373
  _globals['_FRAMEPROFILING']._serialized_start=1375
  _globals['_FRAMEPROFILING']._serialized_end=1494
  _globals['_BUNDLEDFRAMEPROFILINGSET']._serialized_start=1497
  _globals['_BUNDLEDFRAMEPROFILINGSET']._serialized_end=1972
  _globals['_BUNDLEDFRAMEPROFILINGSET_PREPROCESSINGSERVICEPOINTCLOUDPROFILINGMAPENTRY']._serialized_start=1826
  _globals['_BUNDLEDFRAMEPROFILINGSET_PREPROCESSINGSERVICEPOINTCLOUDPROFILINGMAPENTRY']._serialized_end=1954
  _globals['_PROFILINGINFO']._serialized_start=1975
  _globals['_PROFILINGINFO']._serialized_end=2139
# @@protoc_insertion_point(module_scope)
