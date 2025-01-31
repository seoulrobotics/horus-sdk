import dataclasses, datetime, enum, typing

from horus.pb.detection_service import detection_pb2

from horus.sdk.common import timestamp_to_datetime, Vector2f, Vector3f
from horus.sdk.point_frame import PointFrame


class ObjectLabel(enum.Enum):
    """The type of service that was profiled."""

    MISC = 1
    CAR = 2
    CYCLIST = 3
    PEDESTRIAN = 4


class TrackingStatus(enum.Enum):
    """The tracking status of an object."""

    INVALIDATING = 1
    VALIDATING = 2
    DRIFTING = 3
    TRACKING = 4


@dataclasses.dataclass(frozen=True)
class BoundingBox:
    """A bounding box around an object."""

    base: Vector3f
    size: Vector3f
    yaw: float

    @staticmethod
    def _from_pb(pb: detection_pb2.BoundingBox) -> "BoundingBox":
        return BoundingBox(
            base=Vector3f(x=pb.base.x, y=pb.base.y, z=pb.base.z),
            size=Vector3f(x=pb.size.x, y=pb.size.y, z=pb.size.z),
            yaw=pb.yaw,
        )


@dataclasses.dataclass(frozen=True)
class Classification:
    """The classification of an object."""

    class_label: ObjectLabel

    @staticmethod
    def _from_pb(pb: detection_pb2.DetectedObject.Classification) -> "Classification":
        return Classification(class_label=ObjectLabel(int(pb.class_label)))


@dataclasses.dataclass(frozen=True)
class Kinematics:
    """The kinematics of an object."""

    linear_velocity: Vector2f
    yaw_rate: float

    @staticmethod
    def _from_pb(pb: detection_pb2.DetectedObject.Kinematics) -> "Kinematics":
        v = pb.linear_velocity
        return Kinematics(linear_velocity=Vector2f(x=v.x, y=v.y), yaw_rate=pb.yaw_rate)


@dataclasses.dataclass(frozen=True)
class Shape:
    """The shape of an object."""

    bounding_box: BoundingBox

    @staticmethod
    def _from_pb(pb: detection_pb2.DetectedObject.Shape) -> "Shape":
        return Shape(bounding_box=BoundingBox._from_pb(pb.bounding_box))


@dataclasses.dataclass(frozen=True)
class Status:
    """The status of an object."""

    id: int
    tracking_status: TrackingStatus

    @staticmethod
    def _from_pb(pb: detection_pb2.DetectedObject.Status) -> "Status":
        return Status(
            id=pb.id,
            tracking_status=TrackingStatus(pb.tracking_status),
        )


@dataclasses.dataclass(frozen=True)
class DetectedObject:
    """An object detected in a frame."""

    classification: Classification
    kinematics: Kinematics
    shape: Shape
    status: Status

    @staticmethod
    def _from_pb(pb: detection_pb2.DetectedObject) -> "DetectedObject":
        return DetectedObject(
            classification=Classification._from_pb(pb.classification),
            kinematics=Kinematics._from_pb(pb.kinematics),
            shape=Shape._from_pb(pb.shape),
            status=Status._from_pb(pb.status),
        )


@dataclasses.dataclass(frozen=True)
class FrameInfo:
    """The frame information."""

    frame_timestamp: datetime.datetime

    @staticmethod
    def _from_pb(pb: detection_pb2.DetectionEvent.FrameInfo) -> "FrameInfo":
        return FrameInfo(timestamp_to_datetime(pb.frame_timestamp))


@dataclasses.dataclass(frozen=True)
class LabeledPointCloud:
    """A labeled point cloud."""

    point_cloud: PointFrame
    point_index_to_object_id: typing.List[int]

    @staticmethod
    def _from_pb(pb: detection_pb2.LabeledPointCloud) -> "LabeledPointCloud":
        return LabeledPointCloud(
            point_cloud=PointFrame._from_pb(pb.point_cloud),
            point_index_to_object_id=list(pb.point_index_to_object_id.values),
        )


@dataclasses.dataclass(frozen=True)
class DetectionEvent:
    """A detection event."""

    frame_info: FrameInfo
    objects: typing.List[DetectedObject]
    labeled_point_clouds: typing.List[LabeledPointCloud]

    @staticmethod
    def _from_pb(pb: detection_pb2.DetectionEvent) -> "DetectionEvent":
        return DetectionEvent(
            frame_info=FrameInfo(timestamp_to_datetime(pb.frame_info.frame_timestamp)),
            objects=[DetectedObject._from_pb(obj) for obj in pb.objects],
            labeled_point_clouds=[
                LabeledPointCloud._from_pb(labeled_cloud)
                for labeled_cloud in pb.labeled_point_clouds
            ],
        )
