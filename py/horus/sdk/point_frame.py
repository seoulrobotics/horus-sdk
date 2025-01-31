import dataclasses, datetime, itertools, typing

from horus.pb.point import point_message_pb2
from horus.sdk.common import timestamp_to_datetime, Vector3f


@dataclasses.dataclass(frozen=True)
class Vector3fList:
    """A list of 3-dimensional vectors."""

    _buffer: typing.List[float]

    @staticmethod
    def _from_buffer(buffer: typing.Sequence[float]) -> "Vector3fList":
        if len(buffer) % 3 != 0:
            raise ValueError("The buffer must have a length that is a multiple of 3.")

        return Vector3fList(_buffer=list(buffer))

    @property
    def raw_point_buffer(self) -> typing.List[float]:
        """Get the raw point buffer. The points are stored as [x0, y0, z0, x1, y1, z1, ...]."""
        return self._buffer

    def __getitem__(self, index: int) -> Vector3f:
        flat_index = index * 3
        if flat_index + 3 > len(self._buffer):
            raise IndexError("Index out of range.")

        return Vector3f(*self._buffer[flat_index : flat_index + 3])

    def __len__(self) -> int:
        return len(self._buffer) // 3


@dataclasses.dataclass(frozen=True)
class PointFrame:
    """A point frame."""

    header: "Header"

    points: Vector3fList
    """Points in the frame."""

    attributes: typing.List[int]
    """Attributes of the points."""

    intensities: typing.List[int]
    """Intensities of the points."""

    @staticmethod
    def _from_pb(pb: point_message_pb2.PointFrame) -> "PointFrame":
        def unpack_u32s_to_u16s(
            packed_array: typing.Sequence[int], pop_last: bool
        ) -> typing.List[int]:
            if not packed_array:
                return []

            unpacked_array = list(
                itertools.chain.from_iterable(
                    [(x & 0xFFFF, (x >> 16) & 0xFFFF) for x in packed_array]
                )
            )
            if pop_last:
                unpacked_array.pop()
            return unpacked_array

        num_points = len(pb.points.flattened_points) // 3
        is_num_points_odd = num_points % 2 == 1

        return PointFrame(
            header=PointFrame.Header._from_pb(pb.header),
            points=Vector3fList._from_buffer(pb.points.flattened_points),
            attributes=unpack_u32s_to_u16s(
                pb.points.attributes, pop_last=is_num_points_odd
            ),
            intensities=unpack_u32s_to_u16s(
                pb.points.intensities, pop_last=is_num_points_odd
            ),
        )

    @dataclasses.dataclass(frozen=True)
    class Header:
        """The header of a point frame."""

        lidar_id: str
        point_cloud_creation_timestamp: datetime.datetime

        @staticmethod
        def _from_pb(pb: point_message_pb2.PointFrame.Header) -> "PointFrame.Header":
            return PointFrame.Header(
                lidar_id=pb.lidar_id,
                point_cloud_creation_timestamp=timestamp_to_datetime(
                    pb.point_cloud_creation_timestamp
                ),
            )
