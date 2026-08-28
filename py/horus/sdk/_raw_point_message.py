"""
Wire-compatible aliases of the point message protobufs, exposing bulk point arrays as raw bytes
rather than as sequences of boxed Python objects.

They are built at import time in a private descriptor pool, so the generated sources stay
untouched, and declare only the fields the SDK reads: the rest is skipped as unknown, which keeps
decoding proportional to the arrays actually used.

Field numbers come from the generated descriptors, so renumbering is picked up automatically. A
rename or a change of wire type is not, and raises at import instead of aliasing the wrong field.
"""

import sys, typing

from google.protobuf import descriptor, descriptor_pb2, descriptor_pool
from google.protobuf import message_factory
from google.protobuf.message import Message

from horus.pb.point import point_message_pb2

if sys.byteorder != "little":
    raise ImportError(
        "the Horus SDK requires a little-endian platform: packed Protobuf arrays are little-endian"
        " on the wire and are exposed as zero-copy native-order views"
    )

_PACKAGE = "horus.sdk.internal"

# `FieldDescriptorProto` and `FieldDescriptor` number their type and label enums identically, so
# these serve both to declare a field below and to assert the shape of the one it aliases.
_FIELD = descriptor_pb2.FieldDescriptorProto
_REPEATED = _FIELD.LABEL_REPEATED
_SINGULAR = _FIELD.LABEL_OPTIONAL

# Name, number, type, label and, for message fields, the fully qualified type name.
_Field = typing.Tuple[
    str, int, _FIELD.Type.ValueType, _FIELD.Label.ValueType, typing.Optional[str]
]


def _aliased_field(
    message: Message,
    name: str,
    field_type: _FIELD.Type.ValueType,
    label: _FIELD.Label.ValueType,
) -> descriptor.FieldDescriptor:
    """
    Returns the generated descriptor of the field `name` of `message`.

    @throws ImportError if the field is missing or no longer has this wire type and label, since
            the alias would then decode the wrong bytes without failing.
    """
    # `typing.Any` because the protobuf stubs do not declare `label` on either of the
    # descriptor implementations `Message.DESCRIPTOR` may be.
    field: typing.Any = message.DESCRIPTOR.fields_by_name.get(name)

    if field is None:
        raise ImportError(
            f"{message.DESCRIPTOR.full_name} has no field '{name}': the Horus SDK aliases that "
            f"field to decode point clouds and must be updated alongside point_message.proto"
        )

    if field.type != field_type or field.label != label:
        raise ImportError(
            f"{field.full_name} is "
            f"{_FIELD.Label.Name(field.label)} {_FIELD.Type.Name(field.type)}, not the "
            f"{_FIELD.Label.Name(label)} {_FIELD.Type.Name(field_type)} the Horus SDK aliases to "
            f"decode point clouds: the SDK must be updated alongside point_message.proto"
        )

    return typing.cast(descriptor.FieldDescriptor, field)


def _packed_as_bytes(
    message: Message, name: str, element_type: _FIELD.Type.ValueType
) -> _Field:
    """
    Describes a packed scalar array as `repeated bytes`, carrying its payload verbatim.

    A packed field and a `bytes` field share the same wire tag, so this parses the identical bytes
    as one memcpy instead of one boxed Python object per element. It stays `repeated` because a
    packed field may legally be split across several records.
    """
    field = _aliased_field(message, name, element_type, _REPEATED)

    return (name, field.number, _FIELD.TYPE_BYTES, _REPEATED, None)


def _submessage_as_bytes(message: Message, name: str) -> _Field:
    """Describes a submessage as opaque `bytes`, so that it is parsed only when it is read."""
    field = _aliased_field(message, name, _FIELD.TYPE_MESSAGE, _SINGULAR)

    return (name, field.number, _FIELD.TYPE_BYTES, _SINGULAR, None)


def _raw_submessage(
    message: Message, name: str, raw_type: str, label: _FIELD.Label.ValueType
) -> _Field:
    """Describes a submessage as the raw alias `raw_type` declared in this module."""
    field = _aliased_field(message, name, _FIELD.TYPE_MESSAGE, label)

    return (name, field.number, _FIELD.TYPE_MESSAGE, label, f".{_PACKAGE}.{raw_type}")


def _build_file() -> descriptor_pb2.FileDescriptorProto:
    """Returns the `FileDescriptorProto` describing the raw point messages."""
    file = descriptor_pb2.FileDescriptorProto(
        name="horus/sdk/internal/raw_point_message.proto",
        package=_PACKAGE,
        syntax="proto3",
    )

    def add_message(name: str, fields: typing.Sequence[_Field]) -> None:
        descriptor = file.message_type.add()
        descriptor.name = name

        for field_name, number, field_type, label, type_name in fields:
            field = descriptor.field.add()
            field.name = field_name
            field.number = number
            field.type = field_type
            field.label = label

            if type_name is not None:
                field.type_name = type_name

    attributed_points = point_message_pb2.AttributedPoints()
    point_frame = point_message_pb2.PointFrame()
    processed_points = point_message_pb2.ProcessedPointsEvent()
    aggregated = point_message_pb2.AggregatedPointEvents()

    add_message(
        "RawAttributedPoints",
        [
            _packed_as_bytes(attributed_points, "flattened_points", _FIELD.TYPE_FLOAT),
            _packed_as_bytes(attributed_points, "attributes", _FIELD.TYPE_FIXED32),
            _packed_as_bytes(attributed_points, "intensities", _FIELD.TYPE_FIXED32),
        ],
    )

    add_message(
        "RawPointFrame",
        [
            # Small and read in full, so parsed with the generated `PointFrame.Header`.
            _submessage_as_bytes(point_frame, "header"),
            _raw_submessage(point_frame, "points", "RawAttributedPoints", _SINGULAR),
        ],
    )

    add_message(
        "RawProcessedPointsEvent",
        [_raw_submessage(processed_points, "point_frame", "RawPointFrame", _SINGULAR)],
    )

    add_message(
        "RawAggregatedPointEvents",
        [_raw_submessage(aggregated, "events", "RawProcessedPointsEvent", _REPEATED)],
    )

    return file


_POOL = descriptor_pool.DescriptorPool()
_POOL.Add(_build_file())


def _message_class(name: str) -> typing.Type[Message]:
    """Returns the generated message class for `horus.sdk.internal.{name}`."""
    return message_factory.GetMessageClass(
        _POOL.FindMessageTypeByName(f"{_PACKAGE}.{name}")
    )


if typing.TYPE_CHECKING:
    # The raw messages are built at runtime, so no generated source describes them. The
    # declarations below stand in for the missing stubs, mirroring the fields declared in
    # `_build_file()` above, and are replaced by the generated classes at run time.

    class RawAttributedPoints(Message):
        """
        `AttributedPoints`, with each packed array left as its records of raw bytes.

        Each array stays a sequence because a packed field may legally be split across several
        records; `join_packed()` concatenates them back.
        """

        flattened_points: typing.Sequence[bytes]
        attributes: typing.Sequence[bytes]
        intensities: typing.Sequence[bytes]

    class RawPointFrame(Message):
        """`PointFrame`, with its header left as raw bytes."""

        header: bytes
        points: RawAttributedPoints

    class RawProcessedPointsEvent(Message):
        """`ProcessedPointsEvent`."""

        point_frame: RawPointFrame

    class RawAggregatedPointEvents(Message):
        """`AggregatedPointEvents`."""

        events: typing.Sequence[RawProcessedPointsEvent]

else:
    RawAttributedPoints = _message_class("RawAttributedPoints")
    RawPointFrame = _message_class("RawPointFrame")
    RawProcessedPointsEvent = _message_class("RawProcessedPointsEvent")
    RawAggregatedPointEvents = _message_class("RawAggregatedPointEvents")


def join_packed(chunks: typing.Sequence[bytes]) -> bytes:
    """Concatenates the length-delimited records of a packed field."""
    if not chunks:
        return b""
    if len(chunks) == 1:
        return chunks[0]
    return b"".join(chunks)
