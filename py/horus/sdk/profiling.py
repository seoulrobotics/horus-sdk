import dataclasses, datetime, enum, typing
from horus.pb import profiling_pb2, resources_pb2
from horus.sdk.common import duration_to_timedelta, timestamp_to_datetime


class ProfiledService(enum.Enum):
    """The type of service that was profiled."""

    PREPROCESSING_SERVICE = 1
    DETECTION_SERVICE = 2
    DETECTION_MERGER_SERVICE = 3


@dataclasses.dataclass(frozen=True)
class CpuAndMemoryUsage:
    """CPU and memory usage of a single process or the host."""

    cpu_usage_percentage: int
    memory_usage_bytes: int

    @staticmethod
    def _from_pb(pb: resources_pb2.CpuAndMemoryUsage) -> "CpuAndMemoryUsage":
        return CpuAndMemoryUsage(
            cpu_usage_percentage=pb.cpu_usage_percentage,
            memory_usage_bytes=pb.memory_usage_bytes,
        )


@dataclasses.dataclass(frozen=True)
class GpuInfo:
    """Resource usage for a single GPU."""

    gpu_id: int
    gpu_name: str
    temperature_celsius: int
    utilization_percentage: int
    memory_used_bytes: int
    memory_total_bytes: int

    @staticmethod
    def _from_pb(pb: resources_pb2.GpuInfo) -> "GpuInfo":
        return GpuInfo(
            gpu_id=pb.gpu_id,
            gpu_name=pb.gpu_name,
            temperature_celsius=pb.temperature_celsius,
            utilization_percentage=pb.utilization_percentage,
            memory_used_bytes=pb.memory_used_bytes,
            memory_total_bytes=pb.memory_total_bytes,
        )


@dataclasses.dataclass(frozen=True)
class ResourceUsage:
    """Full resource usage snapshot: per-process, host-level, and per-GPU metrics."""

    process: CpuAndMemoryUsage
    host: CpuAndMemoryUsage
    gpus: typing.List[GpuInfo]

    @staticmethod
    def _from_pb(pb: resources_pb2.ResourceUsage) -> "ResourceUsage":
        return ResourceUsage(
            process=CpuAndMemoryUsage._from_pb(pb.process),
            host=CpuAndMemoryUsage._from_pb(pb.host),
            gpus=[GpuInfo._from_pb(gpu) for gpu in pb.gpus],
        )


@dataclasses.dataclass(frozen=True)
class ProfilingSet:
    """A set of profiling information for a service."""

    profiled_service: ProfiledService
    processing_times: typing.Dict[str, datetime.timedelta]
    resource_usage: ResourceUsage

    @staticmethod
    def _from_pb(pb: profiling_pb2.ProfilingSet) -> "ProfilingSet":
        return ProfilingSet(
            profiled_service=ProfiledService(pb.profiled_service),
            processing_times={
                entry.key: duration_to_timedelta(entry.value.duration)
                for entry in pb.processing_times
            },
            resource_usage=ResourceUsage._from_pb(pb.resource_usage),
        )


@dataclasses.dataclass(frozen=True)
class ServiceProfiling:
    """Profiling information for a service."""

    details_profiling_set: ProfilingSet
    total_service_latency: datetime.timedelta
    idle_time_before_processing: datetime.timedelta
    intra_component_idle_time: datetime.timedelta
    node_id: str

    @staticmethod
    def _from_pb(pb: profiling_pb2.ServiceProfiling) -> "ServiceProfiling":
        return ServiceProfiling(
            details_profiling_set=ProfilingSet._from_pb(pb.details_profiling_set),
            total_service_latency=duration_to_timedelta(pb.total_service_latency),
            idle_time_before_processing=duration_to_timedelta(
                pb.idle_time_before_processing
            ),
            intra_component_idle_time=duration_to_timedelta(
                pb.intra_component_idle_time
            ),
            node_id=pb.node_id,
        )


@dataclasses.dataclass(frozen=True)
class PreprocessingFrameProfiling:
    """Profiling information for a single LiDAR frame produced by the preprocessing service."""

    frame_timestamp: datetime.datetime
    lidar_id: str
    service_profiling: ServiceProfiling

    @staticmethod
    def _from_pb(
        pb: profiling_pb2.PreprocessingFrameProfiling,
    ) -> "PreprocessingFrameProfiling":
        return PreprocessingFrameProfiling(
            frame_timestamp=timestamp_to_datetime(pb.frame_timestamp),
            lidar_id=pb.lidar_id,
            service_profiling=ServiceProfiling._from_pb(pb.service_profiling),
        )


@dataclasses.dataclass(frozen=True)
class FrameProfiling:
    """Profiling information for a frame."""

    overall_frame_latency: datetime.timedelta
    frame_bundling_latency: datetime.timedelta
    preprocessing_overhead: datetime.timedelta

    @staticmethod
    def _from_pb(pb: profiling_pb2.FrameProfiling) -> "FrameProfiling":
        return FrameProfiling(
            overall_frame_latency=duration_to_timedelta(pb.overall_frame_latency),
            frame_bundling_latency=duration_to_timedelta(pb.frame_bundling_latency),
            preprocessing_overhead=duration_to_timedelta(pb.preprocessing_overhead),
        )


@dataclasses.dataclass(frozen=True)
class BundledFrameProfilingSet:
    """A set of profiling information for a frame."""

    frame_timestamp: datetime.datetime
    frame_profiling: FrameProfiling
    detection_service_profiling: ServiceProfiling

    @staticmethod
    def _from_pb(
        pb: profiling_pb2.BundledFrameProfilingSet,
    ) -> "BundledFrameProfilingSet":
        return BundledFrameProfilingSet(
            detection_service_profiling=ServiceProfiling._from_pb(
                pb.detection_service_profiling
            ),
            frame_timestamp=timestamp_to_datetime(pb.frame_timestamp),
            frame_profiling=FrameProfiling._from_pb(pb.frame_profiling),
        )


class ProfileType(enum.Enum):
    """The type of profiling information."""

    GENERAL = 1
    BUNDLED = 2
    PREPROCESSING_FRAME = 3


@dataclasses.dataclass(frozen=True)
class ProfilingInfo:
    """Profiling information for a service or frame."""

    Service = ProfiledService
    Set = ProfilingSet
    BundledFrameSet = BundledFrameProfilingSet

    general_profiling_set: typing.Optional[ProfilingSet]
    bundled_frame_profiling_set: typing.Optional[BundledFrameProfilingSet]
    preprocessing_frame_profiling: typing.Optional[PreprocessingFrameProfiling]
    profile_type: ProfileType

    @staticmethod
    def _from_pb(pb: profiling_pb2.ProfilingInfo) -> "ProfilingInfo":
        general = None
        bundled = None
        preprocessing_frame = None
        profile_type = None

        field = pb.WhichOneof("profiling_set")

        if field == "general_profiling_set":
            general = ProfilingSet._from_pb(pb.general_profiling_set)
            profile_type = ProfileType.GENERAL
        elif field == "bundled_frame_profiling_set":
            bundled = BundledFrameProfilingSet._from_pb(pb.bundled_frame_profiling_set)
            profile_type = ProfileType.BUNDLED
        elif field == "preprocessing_frame_profiling":
            preprocessing_frame = PreprocessingFrameProfiling._from_pb(
                pb.preprocessing_frame_profiling
            )
            profile_type = ProfileType.PREPROCESSING_FRAME
        else:
            raise TypeError("Invalid ProfilingInfo protobuf")

        return ProfilingInfo(
            general_profiling_set=general,
            bundled_frame_profiling_set=bundled,
            preprocessing_frame_profiling=preprocessing_frame,
            profile_type=profile_type,
        )
