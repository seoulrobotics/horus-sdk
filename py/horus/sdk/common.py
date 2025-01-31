import datetime
import typing
from horus.pb.config import metadata_pb2


def duration_to_timedelta(duration: metadata_pb2.Duration) -> datetime.timedelta:
    """Convert a protobuf Duration to a Python timedelta object."""
    return datetime.timedelta(
        seconds=duration.seconds, microseconds=duration.nanos / 1000
    )


def timestamp_to_datetime(timestamp: metadata_pb2.Timestamp) -> datetime.datetime:
    """Convert a protobuf Timestamp to a Python datetime object."""
    return datetime.datetime.fromtimestamp(timestamp.seconds + timestamp.nanos / 1e9)


Vector2f = typing.NamedTuple("Vector2f", [("x", float), ("y", float)])

Vector3f = typing.NamedTuple("Vector3f", [("x", float), ("y", float), ("z", float)])
