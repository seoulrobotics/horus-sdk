import dataclasses, datetime, enum, typing
from horus.pb import profiling_pb2
from horus.sdk.common import duration_to_timedelta, timestamp_to_datetime


class ProfiledService(enum.Enum):
    """The type of service that was profiled."""

    PREPROCESSING_SERVICE = 1
    DETECTION_SERVICE = 2


@dataclasses.dataclass(frozen=True)
class ResourceUsage:
    """A set of profiling information for a service."""

    cpu_usage_percentage: int
    memory_usage: int

    @staticmethod
    def _from_pb(pb: profiling_pb2.ProfilingSet.ResourceUsage) -> "ResourceUsage":
        return ResourceUsage(
            cpu_usage_percentage=pb.cpu_usage_percentage,
            memory_usage=pb.memory_usage,
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
        )


@dataclasses.dataclass(frozen=True)
class PreprocessingServicePointCloudProfiling:
    """Profiling information for a preprocessing service point cloud."""

    service_profiling: ServiceProfiling
    point_cloud_sending_latency: datetime.timedelta

    @staticmethod
    def _from_pb(
        pb: profiling_pb2.PreprocessingServicePointCloudProfiling,
    ) -> "PreprocessingServicePointCloudProfiling":
        return PreprocessingServicePointCloudProfiling(
            service_profiling=ServiceProfiling._from_pb(pb.service_profiling),
            point_cloud_sending_latency=duration_to_timedelta(
                pb.point_cloud_sending_latency
            ),
        )


@dataclasses.dataclass(frozen=True)
class FrameProfiling:
    """Profiling information for a frame."""

    overall_frame_latency: datetime.timedelta
    frame_bundling_latency: datetime.timedelta

    @staticmethod
    def _from_pb(pb: profiling_pb2.FrameProfiling) -> "FrameProfiling":
        return FrameProfiling(
            overall_frame_latency=duration_to_timedelta(pb.overall_frame_latency),
            frame_bundling_latency=duration_to_timedelta(pb.frame_bundling_latency),
        )


@dataclasses.dataclass(frozen=True)
class BundledFrameProfilingSet:
    """A set of profiling information for a frame."""

    frame_timestamp: datetime.datetime
    frame_profiling: FrameProfiling
    detection_service_profiling: ServiceProfiling
    preprocessing_service_point_cloud_profiling: typing.Dict[
        str, PreprocessingServicePointCloudProfiling
    ]

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
            preprocessing_service_point_cloud_profiling={
                entry.key: PreprocessingServicePointCloudProfiling._from_pb(entry.value)
                for entry in pb.preprocessing_service_point_cloud_profiling
            },
        )


class ProfileType(enum.Enum):
    """The type of profiling information."""

    GENERAL = 1
    BUNDLED = 2


@dataclasses.dataclass(frozen=True)
class ProfilingInfo:
    """Profiling information for a service or frame."""

    Service = ProfiledService
    Set = ProfilingSet
    BundledFrameSet = BundledFrameProfilingSet

    general_profiling_set: typing.Optional[ProfilingSet]
    bundled_frame_profiling_set: typing.Optional[BundledFrameProfilingSet]
    profile_type: ProfileType

    @staticmethod
    def _from_pb(pb: profiling_pb2.ProfilingInfo) -> "ProfilingInfo":
        general = None
        bundled = None
        profile_type = None

        field = pb.WhichOneof("profiling_set")

        if field == "general_profiling_set":
            general = ProfilingSet._from_pb(pb.general_profiling_set)
            profile_type = ProfileType.GENERAL
        elif field == "bundled_frame_profiling_set":
            bundled = BundledFrameProfilingSet._from_pb(pb.bundled_frame_profiling_set)
            profile_type = ProfileType.BUNDLED
        else:
            raise TypeError("Invalid ProfilingInfo protobuf")

        return ProfilingInfo(
            general_profiling_set=general,
            bundled_frame_profiling_set=bundled,
            profile_type=profile_type,
        )
