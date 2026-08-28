import dataclasses, datetime, enum, functools, typing

from horus.pb.point import point_message_pb2
from horus.pb.point.point_message_pb2 import PointAttribute
from horus.sdk import _raw_point_message
from horus.sdk.common import timestamp_to_datetime, Vector3f

__all__ = ["PointAttribute", "PointFrame", "PointFrameFields", "Vector3fList"]

_EMPTY = memoryview(b"")

_BYTES_PER_POINT = 12
"""Size of one point: three `float32` coordinates."""


class PointFrameFields(enum.IntFlag):
    """
    Per-point arrays a subscriber wants decoded, besides the coordinates.

    Arrays left out are exposed as empty, both by `PointFrame.raw` and by the `PointFrame`
    accessors reading it.
    """

    NONE = 0
    ATTRIBUTES = 1
    INTENSITIES = 2
    ALL = ATTRIBUTES | INTENSITIES


@dataclasses.dataclass(frozen=True)
class Vector3fList:
    """A list of 3-dimensional vectors."""

    _buffer: memoryview
    """Coordinates as `float32`, stored as [x0, y0, z0, x1, y1, z1, ...]."""

    @functools.cached_property
    def raw_point_buffer(self) -> typing.List[float]:
        """
        Get the raw point buffer. The points are stored as [x0, y0, z0, x1, y1, z1, ...].

        The list is built on first access, then cached. `PointFrame.raw.flattened_points` exposes
        the same coordinates without casting each of them into a Python float.
        """
        # `memoryview.tolist()` is typed as returning integers, whatever the format of the view.
        return typing.cast(typing.List[float], self._buffer.tolist())

    def __getitem__(self, index: int) -> Vector3f:
        flat_index = index * 3
        if flat_index + 3 > len(self._buffer):
            raise IndexError("Index out of range.")

        return Vector3f(*self._buffer[flat_index : flat_index + 3])

    def __len__(self) -> int:
        return len(self._buffer) // 3


@dataclasses.dataclass(frozen=True)
class PointFrame:
    """
    A point frame.

    `points`, `attributes` and `intensities` expose the frame as Python objects, as they always
    have. Each of them is built on first access, then cached, so a subscriber which only reads
    `raw` never pays for boxing the arrays it does not use.
    """

    header: "Header"

    num_points: int
    """Number of points in the frame."""

    raw: "Raw"
    """The per-point arrays as they were received, without boxing; see `PointFrame.Raw`."""

    @functools.cached_property
    def points(self) -> Vector3fList:
        """Points in the frame."""
        return Vector3fList(_buffer=self.raw.flattened_points)

    @functools.cached_property
    def attributes(self) -> typing.List[int]:
        """
        Attributes of the points, as bitmasks of `PointAttribute` values.

        Empty if not requested, or not published by the server.
        """
        return self.raw.attributes.tolist()

    @functools.cached_property
    def intensities(self) -> typing.List[int]:
        """
        Intensities of the points.

        Empty if not requested, or if intensity publishing is disabled server-side.
        """
        return self.raw.intensities.tolist()

    @staticmethod
    def _from_pb(pb: point_message_pb2.PointFrame) -> "PointFrame":
        """Builds a `PointFrame` from an already-parsed `PointFrame` message."""
        raw_points = _raw_point_message.RawAttributedPoints()
        raw_points.ParseFromString(pb.points.SerializeToString())

        return PointFrame._build(
            PointFrame.Header._from_pb(pb.header), raw_points, PointFrameFields.ALL
        )

    @staticmethod
    def _from_raw(
        raw: _raw_point_message.RawPointFrame, fields: PointFrameFields
    ) -> "PointFrame":
        """Builds a `PointFrame` from a `_raw_point_message.RawPointFrame`."""
        header_pb = point_message_pb2.PointFrame.Header()
        header_pb.ParseFromString(raw.header)

        return PointFrame._build(
            PointFrame.Header._from_pb(header_pb), raw.points, fields
        )

    @staticmethod
    def _build(
        header: "PointFrame.Header",
        raw_points: _raw_point_message.RawAttributedPoints,
        fields: PointFrameFields,
    ) -> "PointFrame":
        """Builds a `PointFrame` from a `_raw_point_message.RawAttributedPoints`."""
        points_bytes = _raw_point_message.join_packed(raw_points.flattened_points)

        if len(points_bytes) % _BYTES_PER_POINT != 0:
            raise ValueError(
                f"point buffer of {len(points_bytes)} bytes is not a multiple of {_BYTES_PER_POINT} bytes per point"
            )

        num_points = len(points_bytes) // _BYTES_PER_POINT

        return PointFrame(
            header=header,
            num_points=num_points,
            raw=PointFrame.Raw(
                flattened_points=memoryview(points_bytes).cast("f"),
                attributes=(
                    PointFrame._unpack_u16s(raw_points.attributes, num_points)
                    if PointFrameFields.ATTRIBUTES in fields
                    else _EMPTY.cast("H")
                ),
                intensities=(
                    PointFrame._unpack_u16s(raw_points.intensities, num_points)
                    if PointFrameFields.INTENSITIES in fields
                    else _EMPTY.cast("H")
                ),
            ),
        )

    @staticmethod
    def _unpack_u16s(chunks: typing.Sequence[bytes], num_points: int) -> memoryview:
        """
        Returns a `uint16` view over a packed per-point array, or an empty view if it was
        not published.

        The array is transmitted as `fixed32` values holding two `uint16` each, so an odd
        `num_points` leaves one trailing padding value, which the slice below discards.
        """
        packed = _raw_point_message.join_packed(chunks)

        if not packed:
            return _EMPTY.cast("H")

        if len(packed) % 2 != 0:
            raise ValueError(
                f"per-point buffer of {len(packed)} bytes is not a multiple of 2 bytes"
            )

        values = memoryview(packed).cast("H")

        if len(values) < num_points:
            raise ValueError(
                f"per-point buffer holds {len(values)} values, but the frame has {num_points} points"
            )

        return values[:num_points]

    @dataclasses.dataclass(frozen=True)
    class Raw:
        """
        Read-only views over the per-point arrays of a frame, in the layout they arrived in.

        Reading them costs nothing per point, unlike `PointFrame.points`,
        `PointFrame.attributes` and `PointFrame.intensities`, which box every value into a
        Python object. Prefer them for large frames, e.g. by handing them to
        `numpy.frombuffer()`.

        The views stay valid for as long as the frame is referenced.
        """

        flattened_points: memoryview
        """
        Coordinates of the points as `float32`, stored as [x0, y0, z0, x1, y1, z1, ...].

        length = 3 * `PointFrame.num_points`
        """

        attributes: memoryview
        """
        Attributes of the points as `uint16` bitmasks of `PointAttribute` values.

        length = `PointFrame.num_points`

        Empty if not requested, or not published by the server.
        """

        intensities: memoryview
        """
        Intensities of the points as `uint16`.

        length = `PointFrame.num_points`

        Empty if not requested, or if intensity publishing is disabled server-side.
        """

    @dataclasses.dataclass(frozen=True)
    class Header:
        """The header of a point frame."""

        lidar_id: str
        point_cloud_creation_timestamp: datetime.datetime

        calibration_transform: typing.List[float]
        """Sensor-to-origin transform: 16 floats, row-major 4x4 matrix."""

        @staticmethod
        def _from_pb(pb: point_message_pb2.PointFrame.Header) -> "PointFrame.Header":
            return PointFrame.Header(
                lidar_id=pb.lidar_id,
                point_cloud_creation_timestamp=timestamp_to_datetime(
                    pb.point_cloud_creation_timestamp
                ),
                calibration_transform=list(pb.calibration_transform.data),
            )
