from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class GpuProcessInfo(_message.Message):
    __slots__ = ("pid", "gpu_memory_usage_bytes")
    PID_FIELD_NUMBER: _ClassVar[int]
    GPU_MEMORY_USAGE_BYTES_FIELD_NUMBER: _ClassVar[int]
    pid: int
    gpu_memory_usage_bytes: int
    def __init__(self, pid: _Optional[int] = ..., gpu_memory_usage_bytes: _Optional[int] = ...) -> None: ...

class GpuInfo(_message.Message):
    __slots__ = ("gpu_id", "gpu_name", "temperature_celsius", "utilization_percentage", "memory_used_bytes", "memory_total_bytes", "running_processes")
    GPU_ID_FIELD_NUMBER: _ClassVar[int]
    GPU_NAME_FIELD_NUMBER: _ClassVar[int]
    TEMPERATURE_CELSIUS_FIELD_NUMBER: _ClassVar[int]
    UTILIZATION_PERCENTAGE_FIELD_NUMBER: _ClassVar[int]
    MEMORY_USED_BYTES_FIELD_NUMBER: _ClassVar[int]
    MEMORY_TOTAL_BYTES_FIELD_NUMBER: _ClassVar[int]
    RUNNING_PROCESSES_FIELD_NUMBER: _ClassVar[int]
    gpu_id: int
    gpu_name: str
    temperature_celsius: int
    utilization_percentage: int
    memory_used_bytes: int
    memory_total_bytes: int
    running_processes: _containers.RepeatedCompositeFieldContainer[GpuProcessInfo]
    def __init__(self, gpu_id: _Optional[int] = ..., gpu_name: _Optional[str] = ..., temperature_celsius: _Optional[int] = ..., utilization_percentage: _Optional[int] = ..., memory_used_bytes: _Optional[int] = ..., memory_total_bytes: _Optional[int] = ..., running_processes: _Optional[_Iterable[_Union[GpuProcessInfo, _Mapping]]] = ...) -> None: ...

class CpuAndMemoryUsage(_message.Message):
    __slots__ = ("cpu_usage_percentage", "memory_usage_bytes")
    CPU_USAGE_PERCENTAGE_FIELD_NUMBER: _ClassVar[int]
    MEMORY_USAGE_BYTES_FIELD_NUMBER: _ClassVar[int]
    cpu_usage_percentage: int
    memory_usage_bytes: int
    def __init__(self, cpu_usage_percentage: _Optional[int] = ..., memory_usage_bytes: _Optional[int] = ...) -> None: ...

class ResourceUsage(_message.Message):
    __slots__ = ("process", "host", "gpus", "cpu_cores")
    PROCESS_FIELD_NUMBER: _ClassVar[int]
    HOST_FIELD_NUMBER: _ClassVar[int]
    GPUS_FIELD_NUMBER: _ClassVar[int]
    CPU_CORES_FIELD_NUMBER: _ClassVar[int]
    process: CpuAndMemoryUsage
    host: CpuAndMemoryUsage
    gpus: _containers.RepeatedCompositeFieldContainer[GpuInfo]
    cpu_cores: int
    def __init__(self, process: _Optional[_Union[CpuAndMemoryUsage, _Mapping]] = ..., host: _Optional[_Union[CpuAndMemoryUsage, _Mapping]] = ..., gpus: _Optional[_Iterable[_Union[GpuInfo, _Mapping]]] = ..., cpu_cores: _Optional[int] = ...) -> None: ...
