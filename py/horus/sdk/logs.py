import dataclasses, datetime, enum, typing

from horus.logs.format import format_log_data, format_log_field_no
from horus.pb.logs.metadata_pb2 import LogMetadata
from horus.pb.logs import message_pb2
from horus.sdk.sensor import SensorInfoEvent

if typing.TYPE_CHECKING:
    from typing_extensions import override, TypeAliasType
else:
    TypeAliasType = lambda name, t: t
    override = lambda f: f


class LogSeverity(enum.Enum):
    """The severity of a `Log` message."""

    DEBUG = LogMetadata.SEVERITY_DEBUG
    INFO = LogMetadata.SEVERITY_INFO
    WARNING = LogMetadata.SEVERITY_WARNING
    ERROR = LogMetadata.SEVERITY_ERROR
    FATAL = LogMetadata.SEVERITY_FATAL


@dataclasses.dataclass(frozen=True)
class Log:
    """A log message."""

    # Workaround for https://github.com/python/mypy/issues/17031:
    Severity = TypeAliasType("Severity", LogSeverity)

    message: str
    """Human-readable message of the log."""
    id: int
    """Unique identifier of the log."""
    node_id: str
    """Unique identifier of the node which emitted the log."""
    severity: Severity
    """Severity of the log message."""
    time: datetime.datetime
    """Time at which the node emitted the log."""

    @staticmethod
    def _from_pb(pb: message_pb2.LogMessage) -> "Log":
        log_metadata = pb.metadata

        return Log(
            message=format_log_data(pb.data),
            id=format_log_field_no(pb.data),
            node_id=log_metadata.node_id,
            severity=LogSeverity(log_metadata.severity),
            time=datetime.datetime.fromtimestamp(log_metadata.ms_since_epoch / 1000),
        )
