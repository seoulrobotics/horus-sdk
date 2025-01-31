from horus.pb.config import metadata_pb2 as _metadata_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf.internal import enum_type_wrapper as _enum_type_wrapper
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class ProfilingSet(_message.Message):
    __slots__ = ("profiled_service", "processing_times", "resource_usage")
    class ProfiledService(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
        __slots__ = ()
        SERVICE_UNSPECIFIED: _ClassVar[ProfilingSet.ProfiledService]
        PREPROCESSING_SERVICE: _ClassVar[ProfilingSet.ProfiledService]
        DETECTION_SERVICE: _ClassVar[ProfilingSet.ProfiledService]
    SERVICE_UNSPECIFIED: ProfilingSet.ProfiledService
    PREPROCESSING_SERVICE: ProfilingSet.ProfiledService
    DETECTION_SERVICE: ProfilingSet.ProfiledService
    class ProfiledDuration(_message.Message):
        __slots__ = ("duration", "performance_hints")
        class PerformanceHint(_message.Message):
            __slots__ = ("config_parameter_action", "config_parameter_name")
            class ConfigParameterAction(int, metaclass=_enum_type_wrapper.EnumTypeWrapper):
                __slots__ = ()
                CONFIG_PARAMETER_ACTION_UNSPECIFIED: _ClassVar[ProfilingSet.ProfiledDuration.PerformanceHint.ConfigParameterAction]
                INCREASE: _ClassVar[ProfilingSet.ProfiledDuration.PerformanceHint.ConfigParameterAction]
                DECREASE: _ClassVar[ProfilingSet.ProfiledDuration.PerformanceHint.ConfigParameterAction]
            CONFIG_PARAMETER_ACTION_UNSPECIFIED: ProfilingSet.ProfiledDuration.PerformanceHint.ConfigParameterAction
            INCREASE: ProfilingSet.ProfiledDuration.PerformanceHint.ConfigParameterAction
            DECREASE: ProfilingSet.ProfiledDuration.PerformanceHint.ConfigParameterAction
            CONFIG_PARAMETER_ACTION_FIELD_NUMBER: _ClassVar[int]
            CONFIG_PARAMETER_NAME_FIELD_NUMBER: _ClassVar[int]
            config_parameter_action: ProfilingSet.ProfiledDuration.PerformanceHint.ConfigParameterAction
            config_parameter_name: str
            def __init__(self, config_parameter_action: _Optional[_Union[ProfilingSet.ProfiledDuration.PerformanceHint.ConfigParameterAction, str]] = ..., config_parameter_name: _Optional[str] = ...) -> None: ...
        DURATION_FIELD_NUMBER: _ClassVar[int]
        PERFORMANCE_HINTS_FIELD_NUMBER: _ClassVar[int]
        duration: _metadata_pb2.Duration
        performance_hints: _containers.RepeatedCompositeFieldContainer[ProfilingSet.ProfiledDuration.PerformanceHint]
        def __init__(self, duration: _Optional[_Union[_metadata_pb2.Duration, _Mapping]] = ..., performance_hints: _Optional[_Iterable[_Union[ProfilingSet.ProfiledDuration.PerformanceHint, _Mapping]]] = ...) -> None: ...
    class ProfiledDurationMapEntry(_message.Message):
        __slots__ = ("key", "value")
        KEY_FIELD_NUMBER: _ClassVar[int]
        VALUE_FIELD_NUMBER: _ClassVar[int]
        key: str
        value: ProfilingSet.ProfiledDuration
        def __init__(self, key: _Optional[str] = ..., value: _Optional[_Union[ProfilingSet.ProfiledDuration, _Mapping]] = ...) -> None: ...
    class ResourceUsage(_message.Message):
        __slots__ = ("cpu_usage_percentage", "memory_usage")
        CPU_USAGE_PERCENTAGE_FIELD_NUMBER: _ClassVar[int]
        MEMORY_USAGE_FIELD_NUMBER: _ClassVar[int]
        cpu_usage_percentage: int
        memory_usage: int
        def __init__(self, cpu_usage_percentage: _Optional[int] = ..., memory_usage: _Optional[int] = ...) -> None: ...
    PROFILED_SERVICE_FIELD_NUMBER: _ClassVar[int]
    PROCESSING_TIMES_FIELD_NUMBER: _ClassVar[int]
    RESOURCE_USAGE_FIELD_NUMBER: _ClassVar[int]
    profiled_service: ProfilingSet.ProfiledService
    processing_times: _containers.RepeatedCompositeFieldContainer[ProfilingSet.ProfiledDurationMapEntry]
    resource_usage: ProfilingSet.ResourceUsage
    def __init__(self, profiled_service: _Optional[_Union[ProfilingSet.ProfiledService, str]] = ..., processing_times: _Optional[_Iterable[_Union[ProfilingSet.ProfiledDurationMapEntry, _Mapping]]] = ..., resource_usage: _Optional[_Union[ProfilingSet.ResourceUsage, _Mapping]] = ...) -> None: ...

class ServiceProfiling(_message.Message):
    __slots__ = ("details_profiling_set", "total_service_latency", "idle_time_before_processing", "intra_component_idle_time")
    DETAILS_PROFILING_SET_FIELD_NUMBER: _ClassVar[int]
    TOTAL_SERVICE_LATENCY_FIELD_NUMBER: _ClassVar[int]
    IDLE_TIME_BEFORE_PROCESSING_FIELD_NUMBER: _ClassVar[int]
    INTRA_COMPONENT_IDLE_TIME_FIELD_NUMBER: _ClassVar[int]
    details_profiling_set: ProfilingSet
    total_service_latency: _metadata_pb2.Duration
    idle_time_before_processing: _metadata_pb2.Duration
    intra_component_idle_time: _metadata_pb2.Duration
    def __init__(self, details_profiling_set: _Optional[_Union[ProfilingSet, _Mapping]] = ..., total_service_latency: _Optional[_Union[_metadata_pb2.Duration, _Mapping]] = ..., idle_time_before_processing: _Optional[_Union[_metadata_pb2.Duration, _Mapping]] = ..., intra_component_idle_time: _Optional[_Union[_metadata_pb2.Duration, _Mapping]] = ...) -> None: ...

class PreprocessingServicePointCloudProfiling(_message.Message):
    __slots__ = ("service_profiling", "point_cloud_sending_latency")
    SERVICE_PROFILING_FIELD_NUMBER: _ClassVar[int]
    POINT_CLOUD_SENDING_LATENCY_FIELD_NUMBER: _ClassVar[int]
    service_profiling: ServiceProfiling
    point_cloud_sending_latency: _metadata_pb2.Duration
    def __init__(self, service_profiling: _Optional[_Union[ServiceProfiling, _Mapping]] = ..., point_cloud_sending_latency: _Optional[_Union[_metadata_pb2.Duration, _Mapping]] = ...) -> None: ...

class FrameProfiling(_message.Message):
    __slots__ = ("overall_frame_latency", "frame_bundling_latency")
    OVERALL_FRAME_LATENCY_FIELD_NUMBER: _ClassVar[int]
    FRAME_BUNDLING_LATENCY_FIELD_NUMBER: _ClassVar[int]
    overall_frame_latency: _metadata_pb2.Duration
    frame_bundling_latency: _metadata_pb2.Duration
    def __init__(self, overall_frame_latency: _Optional[_Union[_metadata_pb2.Duration, _Mapping]] = ..., frame_bundling_latency: _Optional[_Union[_metadata_pb2.Duration, _Mapping]] = ...) -> None: ...

class BundledFrameProfilingSet(_message.Message):
    __slots__ = ("frame_timestamp", "frame_profiling", "detection_service_profiling", "preprocessing_service_point_cloud_profiling")
    class PreprocessingServicePointCloudProfilingMapEntry(_message.Message):
        __slots__ = ("key", "value")
        KEY_FIELD_NUMBER: _ClassVar[int]
        VALUE_FIELD_NUMBER: _ClassVar[int]
        key: str
        value: PreprocessingServicePointCloudProfiling
        def __init__(self, key: _Optional[str] = ..., value: _Optional[_Union[PreprocessingServicePointCloudProfiling, _Mapping]] = ...) -> None: ...
    FRAME_TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    FRAME_PROFILING_FIELD_NUMBER: _ClassVar[int]
    DETECTION_SERVICE_PROFILING_FIELD_NUMBER: _ClassVar[int]
    PREPROCESSING_SERVICE_POINT_CLOUD_PROFILING_FIELD_NUMBER: _ClassVar[int]
    frame_timestamp: _metadata_pb2.Timestamp
    frame_profiling: FrameProfiling
    detection_service_profiling: ServiceProfiling
    preprocessing_service_point_cloud_profiling: _containers.RepeatedCompositeFieldContainer[BundledFrameProfilingSet.PreprocessingServicePointCloudProfilingMapEntry]
    def __init__(self, frame_timestamp: _Optional[_Union[_metadata_pb2.Timestamp, _Mapping]] = ..., frame_profiling: _Optional[_Union[FrameProfiling, _Mapping]] = ..., detection_service_profiling: _Optional[_Union[ServiceProfiling, _Mapping]] = ..., preprocessing_service_point_cloud_profiling: _Optional[_Iterable[_Union[BundledFrameProfilingSet.PreprocessingServicePointCloudProfilingMapEntry, _Mapping]]] = ...) -> None: ...

class ProfilingInfo(_message.Message):
    __slots__ = ("general_profiling_set", "bundled_frame_profiling_set")
    GENERAL_PROFILING_SET_FIELD_NUMBER: _ClassVar[int]
    BUNDLED_FRAME_PROFILING_SET_FIELD_NUMBER: _ClassVar[int]
    general_profiling_set: ProfilingSet
    bundled_frame_profiling_set: BundledFrameProfilingSet
    def __init__(self, general_profiling_set: _Optional[_Union[ProfilingSet, _Mapping]] = ..., bundled_frame_profiling_set: _Optional[_Union[BundledFrameProfilingSet, _Mapping]] = ...) -> None: ...
