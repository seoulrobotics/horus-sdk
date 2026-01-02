from horus.pb.logs import common_pb2 as _common_pb2
from horus.pb.logs import metadata_pb2 as _metadata_pb2
from google.protobuf.internal import containers as _containers
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from typing import ClassVar as _ClassVar, Iterable as _Iterable, Mapping as _Mapping, Optional as _Optional, Union as _Union

DESCRIPTOR: _descriptor.FileDescriptor

class Generic(_message.Message):
    __slots__ = ("message", "location")
    MESSAGE_FIELD_NUMBER: _ClassVar[int]
    LOCATION_FIELD_NUMBER: _ClassVar[int]
    message: str
    location: _metadata_pb2.LogMetadata.SourceLocation
    def __init__(self, message: _Optional[str] = ..., location: _Optional[_Union[_metadata_pb2.LogMetadata.SourceLocation, _Mapping]] = ...) -> None: ...

class Oom(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class RpcConnectionError(_message.Message):
    __slots__ = ("target_service", "target_uri", "details")
    TARGET_SERVICE_FIELD_NUMBER: _ClassVar[int]
    TARGET_URI_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    target_service: str
    target_uri: str
    details: str
    def __init__(self, target_service: _Optional[str] = ..., target_uri: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class InvalidProjectName(_message.Message):
    __slots__ = ("name", "details")
    NAME_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    name: str
    details: str
    def __init__(self, name: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class ProjectNotFound(_message.Message):
    __slots__ = ("name",)
    NAME_FIELD_NUMBER: _ClassVar[int]
    name: str
    def __init__(self, name: _Optional[str] = ...) -> None: ...

class ProjectAlreadyExists(_message.Message):
    __slots__ = ("name",)
    NAME_FIELD_NUMBER: _ClassVar[int]
    name: str
    def __init__(self, name: _Optional[str] = ...) -> None: ...

class InvalidConfiguration(_message.Message):
    __slots__ = ("validation_errors",)
    VALIDATION_ERRORS_FIELD_NUMBER: _ClassVar[int]
    validation_errors: _containers.RepeatedCompositeFieldContainer[_common_pb2.ValidationError]
    def __init__(self, validation_errors: _Optional[_Iterable[_Union[_common_pb2.ValidationError, _Mapping]]] = ...) -> None: ...

class EntityNotFound(_message.Message):
    __slots__ = ("id", "entity_type")
    ID_FIELD_NUMBER: _ClassVar[int]
    ENTITY_TYPE_FIELD_NUMBER: _ClassVar[int]
    id: str
    entity_type: str
    def __init__(self, id: _Optional[str] = ..., entity_type: _Optional[str] = ...) -> None: ...

class ActiveProjectCannotBeDeleted(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class RpcDisconnectionError(_message.Message):
    __slots__ = ("target_service", "target_uri", "details")
    TARGET_SERVICE_FIELD_NUMBER: _ClassVar[int]
    TARGET_URI_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    target_service: str
    target_uri: str
    details: str
    def __init__(self, target_service: _Optional[str] = ..., target_uri: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class DroppedLogs(_message.Message):
    __slots__ = ("n",)
    N_FIELD_NUMBER: _ClassVar[int]
    n: int
    def __init__(self, n: _Optional[int] = ...) -> None: ...

class OpenedProject(_message.Message):
    __slots__ = ("project_name",)
    PROJECT_NAME_FIELD_NUMBER: _ClassVar[int]
    project_name: str
    def __init__(self, project_name: _Optional[str] = ...) -> None: ...

class CreatedProject(_message.Message):
    __slots__ = ("project_name", "source_name")
    PROJECT_NAME_FIELD_NUMBER: _ClassVar[int]
    SOURCE_NAME_FIELD_NUMBER: _ClassVar[int]
    project_name: str
    source_name: str
    def __init__(self, project_name: _Optional[str] = ..., source_name: _Optional[str] = ...) -> None: ...

class ConfigUnavailable(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class InvalidRequest(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class SanityCheckError(_message.Message):
    __slots__ = ("sanity_check_name", "details")
    SANITY_CHECK_NAME_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    sanity_check_name: str
    details: str
    def __init__(self, sanity_check_name: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class BagFailedToOpen(_message.Message):
    __slots__ = ("horus_bag_path", "details")
    HORUS_BAG_PATH_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    horus_bag_path: str
    details: str
    def __init__(self, horus_bag_path: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class BagFailedToClose(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class BagConversionFailed(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class BagFailedToWrite(_message.Message):
    __slots__ = ("name", "details")
    NAME_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    name: str
    details: str
    def __init__(self, name: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class CalibrationError(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class ProjectManagerFailedToStartRecording(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class ProjectManagerFailedToStopRecording(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class ServiceConnectionTimedOut(_message.Message):
    __slots__ = ("node_type", "node_id")
    NODE_TYPE_FIELD_NUMBER: _ClassVar[int]
    NODE_ID_FIELD_NUMBER: _ClassVar[int]
    node_type: str
    node_id: str
    def __init__(self, node_type: _Optional[str] = ..., node_id: _Optional[str] = ...) -> None: ...

class BagRecorderAlreadyRunning(_message.Message):
    __slots__ = ("path",)
    PATH_FIELD_NUMBER: _ClassVar[int]
    path: str
    def __init__(self, path: _Optional[str] = ...) -> None: ...

class LicenseServerConnectionError(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class LicenseError(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class LicenseNotFoundError(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class LicenseExpiredError(_message.Message):
    __slots__ = ("expiration_time",)
    EXPIRATION_TIME_FIELD_NUMBER: _ClassVar[int]
    expiration_time: _metadata_pb2.LogMetadata.Timestamp
    def __init__(self, expiration_time: _Optional[_Union[_metadata_pb2.LogMetadata.Timestamp, _Mapping]] = ...) -> None: ...

class LicenseExceededError(_message.Message):
    __slots__ = ("lidar_count", "max_lidar_count")
    LIDAR_COUNT_FIELD_NUMBER: _ClassVar[int]
    MAX_LIDAR_COUNT_FIELD_NUMBER: _ClassVar[int]
    lidar_count: int
    max_lidar_count: int
    def __init__(self, lidar_count: _Optional[int] = ..., max_lidar_count: _Optional[int] = ...) -> None: ...

class LicenseHostMachineError(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class LicensePrivilegeError(_message.Message):
    __slots__ = ("missing_privilege", "level_error")
    MISSING_PRIVILEGE_FIELD_NUMBER: _ClassVar[int]
    LEVEL_ERROR_FIELD_NUMBER: _ClassVar[int]
    missing_privilege: str
    level_error: str
    def __init__(self, missing_privilege: _Optional[str] = ..., level_error: _Optional[str] = ...) -> None: ...

class LicenseActiveInfo(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class MultipleLicensesWarning(_message.Message):
    __slots__ = ("non_expired_licenses_count", "active_license_filename")
    NON_EXPIRED_LICENSES_COUNT_FIELD_NUMBER: _ClassVar[int]
    ACTIVE_LICENSE_FILENAME_FIELD_NUMBER: _ClassVar[int]
    non_expired_licenses_count: int
    active_license_filename: str
    def __init__(self, non_expired_licenses_count: _Optional[int] = ..., active_license_filename: _Optional[str] = ...) -> None: ...

class LicenseCurrentLicenseStatusInfo(_message.Message):
    __slots__ = ("expiration_time", "max_lidar_count")
    EXPIRATION_TIME_FIELD_NUMBER: _ClassVar[int]
    MAX_LIDAR_COUNT_FIELD_NUMBER: _ClassVar[int]
    expiration_time: _metadata_pb2.LogMetadata.Timestamp
    max_lidar_count: int
    def __init__(self, expiration_time: _Optional[_Union[_metadata_pb2.LogMetadata.Timestamp, _Mapping]] = ..., max_lidar_count: _Optional[int] = ...) -> None: ...

class BagRecordingStoppedInfo(_message.Message):
    __slots__ = ("path", "details")
    PATH_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    path: str
    details: str
    def __init__(self, path: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class BagRecordingFailedToStart(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class BagRecordingStartedInfo(_message.Message):
    __slots__ = ("path",)
    PATH_FIELD_NUMBER: _ClassVar[int]
    path: str
    def __init__(self, path: _Optional[str] = ...) -> None: ...

class ReplayRestartedInfo(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class InputSourceChangeRequestedInfo(_message.Message):
    __slots__ = ("source",)
    SOURCE_FIELD_NUMBER: _ClassVar[int]
    source: str
    def __init__(self, source: _Optional[str] = ...) -> None: ...

class InputSourceSwitchedInfo(_message.Message):
    __slots__ = ("source",)
    SOURCE_FIELD_NUMBER: _ClassVar[int]
    source: str
    def __init__(self, source: _Optional[str] = ...) -> None: ...

class RpcTimeoutWarning(_message.Message):
    __slots__ = ("request_name", "endpoint", "elapsed")
    REQUEST_NAME_FIELD_NUMBER: _ClassVar[int]
    ENDPOINT_FIELD_NUMBER: _ClassVar[int]
    ELAPSED_FIELD_NUMBER: _ClassVar[int]
    request_name: str
    endpoint: str
    elapsed: _metadata_pb2.LogMetadata.Duration
    def __init__(self, request_name: _Optional[str] = ..., endpoint: _Optional[str] = ..., elapsed: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ...) -> None: ...

class CannotWriteLogFile(_message.Message):
    __slots__ = ("path", "details")
    PATH_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    path: str
    details: str
    def __init__(self, path: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class PointCloudParsingFailureWarning(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class LidarIsDead(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarIsNotDeadAnymore(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarIsObstructed(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarIsNotObstructedAnymore(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarIsTilted(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarIsNotTiltedAnymore(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarHasBeenAutomaticallyRecalibrated(_message.Message):
    __slots__ = ("lidar_id", "lidar_name", "translation", "rotation_rpy")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    TRANSLATION_FIELD_NUMBER: _ClassVar[int]
    ROTATION_RPY_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    translation: str
    rotation_rpy: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ..., translation: _Optional[str] = ..., rotation_rpy: _Optional[str] = ...) -> None: ...

class LidarAutoCorrectionFailed(_message.Message):
    __slots__ = ("lidar_id", "lidar_name", "failure_count")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    FAILURE_COUNT_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    failure_count: int
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ..., failure_count: _Optional[int] = ...) -> None: ...

class LidarIcpFailed(_message.Message):
    __slots__ = ("lidar_id", "lidar_name", "failure_count")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    FAILURE_COUNT_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    failure_count: int
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ..., failure_count: _Optional[int] = ...) -> None: ...

class ReceivedFirstDataForLidar(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class TerminationFailureError(_message.Message):
    __slots__ = ("component", "details")
    COMPONENT_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    component: str
    details: str
    def __init__(self, component: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class FrameProcessingError(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class ThreadPoolUnavailableError(_message.Message):
    __slots__ = ("consequence",)
    CONSEQUENCE_FIELD_NUMBER: _ClassVar[int]
    consequence: str
    def __init__(self, consequence: _Optional[str] = ...) -> None: ...

class InvalidArgument(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class ComponentInitializationFailureFatal(_message.Message):
    __slots__ = ("component", "details")
    COMPONENT_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    component: str
    details: str
    def __init__(self, component: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class UnhandledEnumCaseError(_message.Message):
    __slots__ = ("case_name", "location")
    CASE_NAME_FIELD_NUMBER: _ClassVar[int]
    LOCATION_FIELD_NUMBER: _ClassVar[int]
    case_name: str
    location: _metadata_pb2.LogMetadata.SourceLocation
    def __init__(self, case_name: _Optional[str] = ..., location: _Optional[_Union[_metadata_pb2.LogMetadata.SourceLocation, _Mapping]] = ...) -> None: ...

class BagEmptyError(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class DiscardingDataError(_message.Message):
    __slots__ = ("component", "details")
    COMPONENT_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    component: str
    details: str
    def __init__(self, component: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class DiscardingDataWarning(_message.Message):
    __slots__ = ("component", "details")
    COMPONENT_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    component: str
    details: str
    def __init__(self, component: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class NothingToProcess(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class InvalidComponentConfiguration(_message.Message):
    __slots__ = ("component", "details")
    COMPONENT_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    component: str
    details: str
    def __init__(self, component: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class ChannelReceiverNotFoundWarning(_message.Message):
    __slots__ = ("key",)
    KEY_FIELD_NUMBER: _ClassVar[int]
    key: str
    def __init__(self, key: _Optional[str] = ...) -> None: ...

class ModelLoadFailure(_message.Message):
    __slots__ = ("model", "details")
    MODEL_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    model: str
    details: str
    def __init__(self, model: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class ModelExecutionFailureError(_message.Message):
    __slots__ = ("model", "details")
    MODEL_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    model: str
    details: str
    def __init__(self, model: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class ServiceReadyInfo(_message.Message):
    __slots__ = ("service",)
    SERVICE_FIELD_NUMBER: _ClassVar[int]
    service: str
    def __init__(self, service: _Optional[str] = ...) -> None: ...

class ModelPreparingInfo(_message.Message):
    __slots__ = ("model",)
    MODEL_FIELD_NUMBER: _ClassVar[int]
    model: str
    def __init__(self, model: _Optional[str] = ...) -> None: ...

class ModelInitializedInfo(_message.Message):
    __slots__ = ("name",)
    NAME_FIELD_NUMBER: _ClassVar[int]
    name: str
    def __init__(self, name: _Optional[str] = ...) -> None: ...

class ModelInitializationFailureWarning(_message.Message):
    __slots__ = ("model", "details")
    MODEL_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    model: str
    details: str
    def __init__(self, model: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class RosSpinnerStoppedWarning(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class ActorSystemUnavailableError(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class ConfigNodeNotFoundError(_message.Message):
    __slots__ = ("node",)
    NODE_FIELD_NUMBER: _ClassVar[int]
    node: str
    def __init__(self, node: _Optional[str] = ...) -> None: ...

class BagTimestampOutOfOrderError(_message.Message):
    __slots__ = ("prior_timestamp", "current_timestamp")
    PRIOR_TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    CURRENT_TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    prior_timestamp: _metadata_pb2.LogMetadata.Timestamp
    current_timestamp: _metadata_pb2.LogMetadata.Timestamp
    def __init__(self, prior_timestamp: _Optional[_Union[_metadata_pb2.LogMetadata.Timestamp, _Mapping]] = ..., current_timestamp: _Optional[_Union[_metadata_pb2.LogMetadata.Timestamp, _Mapping]] = ...) -> None: ...

class BagReplayUnexpectedTimestampError(_message.Message):
    __slots__ = ("expected_timestamp", "received_timestamp")
    EXPECTED_TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    RECEIVED_TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    expected_timestamp: _metadata_pb2.LogMetadata.Timestamp
    received_timestamp: _metadata_pb2.LogMetadata.Timestamp
    def __init__(self, expected_timestamp: _Optional[_Union[_metadata_pb2.LogMetadata.Timestamp, _Mapping]] = ..., received_timestamp: _Optional[_Union[_metadata_pb2.LogMetadata.Timestamp, _Mapping]] = ...) -> None: ...

class WebsocketClosedInfo(_message.Message):
    __slots__ = ("uri", "reason")
    URI_FIELD_NUMBER: _ClassVar[int]
    REASON_FIELD_NUMBER: _ClassVar[int]
    uri: str
    reason: str
    def __init__(self, uri: _Optional[str] = ..., reason: _Optional[str] = ...) -> None: ...

class WebsocketOpenedInfo(_message.Message):
    __slots__ = ("uri", "endpoint_is_server_client")
    URI_FIELD_NUMBER: _ClassVar[int]
    ENDPOINT_IS_SERVER_CLIENT_FIELD_NUMBER: _ClassVar[int]
    uri: str
    endpoint_is_server_client: str
    def __init__(self, uri: _Optional[str] = ..., endpoint_is_server_client: _Optional[str] = ...) -> None: ...

class SubscriberDisconnectedInfo(_message.Message):
    __slots__ = ("name", "uri")
    NAME_FIELD_NUMBER: _ClassVar[int]
    URI_FIELD_NUMBER: _ClassVar[int]
    name: str
    uri: str
    def __init__(self, name: _Optional[str] = ..., uri: _Optional[str] = ...) -> None: ...

class ThreadPoolSlowingDownWarning(_message.Message):
    __slots__ = ("number",)
    NUMBER_FIELD_NUMBER: _ClassVar[int]
    number: int
    def __init__(self, number: _Optional[int] = ...) -> None: ...

class ThreadPoolNotRespondingWarning(_message.Message):
    __slots__ = ("not_responding_for",)
    NOT_RESPONDING_FOR_FIELD_NUMBER: _ClassVar[int]
    not_responding_for: _metadata_pb2.LogMetadata.Duration
    def __init__(self, not_responding_for: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ...) -> None: ...

class ThreadPoolBrokenPromiseWarning(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class BoxFittingPointsBelowBaseWarning(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class FailedToRemoveStalePointsWarning(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class IrregularBroadcastingPeriodWarning(_message.Message):
    __slots__ = ("num_irregular", "duration", "mean_deviation")
    NUM_IRREGULAR_FIELD_NUMBER: _ClassVar[int]
    DURATION_FIELD_NUMBER: _ClassVar[int]
    MEAN_DEVIATION_FIELD_NUMBER: _ClassVar[int]
    num_irregular: int
    duration: _metadata_pb2.LogMetadata.Duration
    mean_deviation: _metadata_pb2.LogMetadata.Duration
    def __init__(self, num_irregular: _Optional[int] = ..., duration: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ..., mean_deviation: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ...) -> None: ...

class ClustererPointsOutOfRangeWarning(_message.Message):
    __slots__ = ("num_points", "sample_points")
    NUM_POINTS_FIELD_NUMBER: _ClassVar[int]
    SAMPLE_POINTS_FIELD_NUMBER: _ClassVar[int]
    num_points: int
    sample_points: str
    def __init__(self, num_points: _Optional[int] = ..., sample_points: _Optional[str] = ...) -> None: ...

class InternalError(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class InternalFatal(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class ServiceStartingInfo(_message.Message):
    __slots__ = ("service",)
    SERVICE_FIELD_NUMBER: _ClassVar[int]
    service: str
    def __init__(self, service: _Optional[str] = ...) -> None: ...

class ConfigNodeNotFoundFatal(_message.Message):
    __slots__ = ("node",)
    NODE_FIELD_NUMBER: _ClassVar[int]
    node: str
    def __init__(self, node: _Optional[str] = ...) -> None: ...

class ServiceSetupError(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class FilesystemError(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class InvalidPresetWarning(_message.Message):
    __slots__ = ("preset", "fallback_name", "reason")
    PRESET_FIELD_NUMBER: _ClassVar[int]
    FALLBACK_NAME_FIELD_NUMBER: _ClassVar[int]
    REASON_FIELD_NUMBER: _ClassVar[int]
    preset: str
    fallback_name: str
    reason: str
    def __init__(self, preset: _Optional[str] = ..., fallback_name: _Optional[str] = ..., reason: _Optional[str] = ...) -> None: ...

class WebsocketFailedClearPendingError(_message.Message):
    __slots__ = ("exception",)
    EXCEPTION_FIELD_NUMBER: _ClassVar[int]
    exception: str
    def __init__(self, exception: _Optional[str] = ...) -> None: ...

class WebsocketFailedToStopError(_message.Message):
    __slots__ = ("exception",)
    EXCEPTION_FIELD_NUMBER: _ClassVar[int]
    exception: str
    def __init__(self, exception: _Optional[str] = ...) -> None: ...

class WebsocketFailedToCreateError(_message.Message):
    __slots__ = ("host", "port", "exception")
    HOST_FIELD_NUMBER: _ClassVar[int]
    PORT_FIELD_NUMBER: _ClassVar[int]
    EXCEPTION_FIELD_NUMBER: _ClassVar[int]
    host: str
    port: int
    exception: str
    def __init__(self, host: _Optional[str] = ..., port: _Optional[int] = ..., exception: _Optional[str] = ...) -> None: ...

class UnexpectedRpcError(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class LicensePollFailed(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class LicenseExpiredWarning(_message.Message):
    __slots__ = ("expiration_time",)
    EXPIRATION_TIME_FIELD_NUMBER: _ClassVar[int]
    expiration_time: _metadata_pb2.LogMetadata.Timestamp
    def __init__(self, expiration_time: _Optional[_Union[_metadata_pb2.LogMetadata.Timestamp, _Mapping]] = ...) -> None: ...

class LicenseUsageExceededWarning(_message.Message):
    __slots__ = ("lidar_count", "max_lidar_count")
    LIDAR_COUNT_FIELD_NUMBER: _ClassVar[int]
    MAX_LIDAR_COUNT_FIELD_NUMBER: _ClassVar[int]
    lidar_count: int
    max_lidar_count: int
    def __init__(self, lidar_count: _Optional[int] = ..., max_lidar_count: _Optional[int] = ...) -> None: ...

class StaticThreadPoolSlowTaskWarning(_message.Message):
    __slots__ = ("time_elapsed", "enqueue_location")
    TIME_ELAPSED_FIELD_NUMBER: _ClassVar[int]
    ENQUEUE_LOCATION_FIELD_NUMBER: _ClassVar[int]
    time_elapsed: _metadata_pb2.LogMetadata.Duration
    enqueue_location: _metadata_pb2.LogMetadata.SourceLocation
    def __init__(self, time_elapsed: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ..., enqueue_location: _Optional[_Union[_metadata_pb2.LogMetadata.SourceLocation, _Mapping]] = ...) -> None: ...

class RpcUnsupportedServiceWarning(_message.Message):
    __slots__ = ("service_id",)
    SERVICE_ID_FIELD_NUMBER: _ClassVar[int]
    service_id: int
    def __init__(self, service_id: _Optional[int] = ...) -> None: ...

class WebsocketHandlerProblem(_message.Message):
    __slots__ = ("what",)
    WHAT_FIELD_NUMBER: _ClassVar[int]
    what: str
    def __init__(self, what: _Optional[str] = ...) -> None: ...

class WebsocketDeserializeError(_message.Message):
    __slots__ = ("what",)
    WHAT_FIELD_NUMBER: _ClassVar[int]
    what: str
    def __init__(self, what: _Optional[str] = ...) -> None: ...

class WebsocketExpiredRpcEndpointError(_message.Message):
    __slots__ = ("uri",)
    URI_FIELD_NUMBER: _ClassVar[int]
    uri: str
    def __init__(self, uri: _Optional[str] = ...) -> None: ...

class WebsocketQueueOverloadedWarning(_message.Message):
    __slots__ = ("current", "max")
    CURRENT_FIELD_NUMBER: _ClassVar[int]
    MAX_FIELD_NUMBER: _ClassVar[int]
    current: int
    max: int
    def __init__(self, current: _Optional[int] = ..., max: _Optional[int] = ...) -> None: ...

class RpcFailedToNotifyWarning(_message.Message):
    __slots__ = ("service", "uri", "notification_name", "what")
    SERVICE_FIELD_NUMBER: _ClassVar[int]
    URI_FIELD_NUMBER: _ClassVar[int]
    NOTIFICATION_NAME_FIELD_NUMBER: _ClassVar[int]
    WHAT_FIELD_NUMBER: _ClassVar[int]
    service: str
    uri: str
    notification_name: str
    what: str
    def __init__(self, service: _Optional[str] = ..., uri: _Optional[str] = ..., notification_name: _Optional[str] = ..., what: _Optional[str] = ...) -> None: ...

class ConfigSubscriptionFailedWarning(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class ThreadPoolClampedWorkersWarning(_message.Message):
    __slots__ = ("clamped", "requested")
    CLAMPED_FIELD_NUMBER: _ClassVar[int]
    REQUESTED_FIELD_NUMBER: _ClassVar[int]
    clamped: int
    requested: int
    def __init__(self, clamped: _Optional[int] = ..., requested: _Optional[int] = ...) -> None: ...

class StoppingHorusBagRecorderAlreadyStopped(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class RecorderConfigUpdateWhileRunning(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class ClampingDataWarning(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class LidarIncompatibleValues(_message.Message):
    __slots__ = ("lidar_ip1", "lidar_ip2", "value1", "value2", "value_names", "resolution")
    LIDAR_IP1_FIELD_NUMBER: _ClassVar[int]
    LIDAR_IP2_FIELD_NUMBER: _ClassVar[int]
    VALUE1_FIELD_NUMBER: _ClassVar[int]
    VALUE2_FIELD_NUMBER: _ClassVar[int]
    VALUE_NAMES_FIELD_NUMBER: _ClassVar[int]
    RESOLUTION_FIELD_NUMBER: _ClassVar[int]
    lidar_ip1: str
    lidar_ip2: str
    value1: str
    value2: str
    value_names: str
    resolution: str
    def __init__(self, lidar_ip1: _Optional[str] = ..., lidar_ip2: _Optional[str] = ..., value1: _Optional[str] = ..., value2: _Optional[str] = ..., value_names: _Optional[str] = ..., resolution: _Optional[str] = ...) -> None: ...

class CannotDetermineContainerIdError(_message.Message):
    __slots__ = ("container_id_file_path",)
    CONTAINER_ID_FILE_PATH_FIELD_NUMBER: _ClassVar[int]
    container_id_file_path: str
    def __init__(self, container_id_file_path: _Optional[str] = ...) -> None: ...

class StartedLidarDriver(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class CannotStartLidarDriver(_message.Message):
    __slots__ = ("lidar_id", "details", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    details: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., details: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class StoppedLidarDriver(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class CannotStopLidarDriver(_message.Message):
    __slots__ = ("lidar_id", "details", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    details: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., details: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class RestartedLidarDriver(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class CannotRestartLidarDriver(_message.Message):
    __slots__ = ("lidar_id", "details", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    details: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., details: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class RemovedUnusedLidarDriver(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class CannotRemoveUnusedLidarDriver(_message.Message):
    __slots__ = ("lidar_id", "details", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    details: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., details: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarDriverGcFailure(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class IdSpaceExhausted(_message.Message):
    __slots__ = ("prefix",)
    PREFIX_FIELD_NUMBER: _ClassVar[int]
    prefix: str
    def __init__(self, prefix: _Optional[str] = ...) -> None: ...

class PreprocessingToPointAggregatorPointsSkipped(_message.Message):
    __slots__ = ("num_skipped_points", "check_interval")
    NUM_SKIPPED_POINTS_FIELD_NUMBER: _ClassVar[int]
    CHECK_INTERVAL_FIELD_NUMBER: _ClassVar[int]
    num_skipped_points: int
    check_interval: _metadata_pb2.LogMetadata.Duration
    def __init__(self, num_skipped_points: _Optional[int] = ..., check_interval: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ...) -> None: ...

class MinMsgIntervalLessThanThreshold(_message.Message):
    __slots__ = ("lidar_id", "threshold", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    THRESHOLD_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    threshold: _metadata_pb2.LogMetadata.Duration
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., threshold: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class FailedToCleanupRosWarning(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class RpcDisconnectedWarning(_message.Message):
    __slots__ = ("request_name", "endpoint")
    REQUEST_NAME_FIELD_NUMBER: _ClassVar[int]
    ENDPOINT_FIELD_NUMBER: _ClassVar[int]
    request_name: str
    endpoint: str
    def __init__(self, request_name: _Optional[str] = ..., endpoint: _Optional[str] = ...) -> None: ...

class RpcUnhandledError(_message.Message):
    __slots__ = ("request_name", "details")
    REQUEST_NAME_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    request_name: str
    details: str
    def __init__(self, request_name: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class TimeDiffOutOfRangeWarning(_message.Message):
    __slots__ = ("prev_time", "curr_time", "diff")
    PREV_TIME_FIELD_NUMBER: _ClassVar[int]
    CURR_TIME_FIELD_NUMBER: _ClassVar[int]
    DIFF_FIELD_NUMBER: _ClassVar[int]
    prev_time: _metadata_pb2.LogMetadata.Timestamp
    curr_time: _metadata_pb2.LogMetadata.Timestamp
    diff: _metadata_pb2.LogMetadata.Duration
    def __init__(self, prev_time: _Optional[_Union[_metadata_pb2.LogMetadata.Timestamp, _Mapping]] = ..., curr_time: _Optional[_Union[_metadata_pb2.LogMetadata.Timestamp, _Mapping]] = ..., diff: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ...) -> None: ...

class TensorrtLog(_message.Message):
    __slots__ = ("log",)
    LOG_FIELD_NUMBER: _ClassVar[int]
    log: str
    def __init__(self, log: _Optional[str] = ...) -> None: ...

class BuildingTensorrtEngineInfo(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class LoadingTensorrtEngineInfo(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class CalibrationMapNotFound(_message.Message):
    __slots__ = ("path",)
    PATH_FIELD_NUMBER: _ClassVar[int]
    path: str
    def __init__(self, path: _Optional[str] = ...) -> None: ...

class CalibrationMapNotValid(_message.Message):
    __slots__ = ("path",)
    PATH_FIELD_NUMBER: _ClassVar[int]
    path: str
    def __init__(self, path: _Optional[str] = ...) -> None: ...

class CalibrationMapPathAlreadyExists(_message.Message):
    __slots__ = ("path",)
    PATH_FIELD_NUMBER: _ClassVar[int]
    path: str
    def __init__(self, path: _Optional[str] = ...) -> None: ...

class FailedToSaveCalibrationMap(_message.Message):
    __slots__ = ("path",)
    PATH_FIELD_NUMBER: _ClassVar[int]
    path: str
    def __init__(self, path: _Optional[str] = ...) -> None: ...

class FailedToRemoveCalibrationMap(_message.Message):
    __slots__ = ("path",)
    PATH_FIELD_NUMBER: _ClassVar[int]
    path: str
    def __init__(self, path: _Optional[str] = ...) -> None: ...

class FailedToIterateInDirectory(_message.Message):
    __slots__ = ("directory_name", "directory_path", "details")
    DIRECTORY_NAME_FIELD_NUMBER: _ClassVar[int]
    DIRECTORY_PATH_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    directory_name: str
    directory_path: str
    details: str
    def __init__(self, directory_name: _Optional[str] = ..., directory_path: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class MapBasedCalibrationWithoutMapLoading(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class MapBasedCalibrationAlreadyRunning(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class CancelMapBasedCalibrationNotRunning(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class BagStreamNotFound(_message.Message):
    __slots__ = ("bag_path", "stream_id")
    BAG_PATH_FIELD_NUMBER: _ClassVar[int]
    STREAM_ID_FIELD_NUMBER: _ClassVar[int]
    bag_path: str
    stream_id: str
    def __init__(self, bag_path: _Optional[str] = ..., stream_id: _Optional[str] = ...) -> None: ...

class EvaluationBagStartedInfo(_message.Message):
    __slots__ = ("bag_path",)
    BAG_PATH_FIELD_NUMBER: _ClassVar[int]
    bag_path: str
    def __init__(self, bag_path: _Optional[str] = ...) -> None: ...

class EvaluationBagFinishedInfo(_message.Message):
    __slots__ = ("bag_path",)
    BAG_PATH_FIELD_NUMBER: _ClassVar[int]
    bag_path: str
    def __init__(self, bag_path: _Optional[str] = ...) -> None: ...

class BagNotFound(_message.Message):
    __slots__ = ("bag_path",)
    BAG_PATH_FIELD_NUMBER: _ClassVar[int]
    bag_path: str
    def __init__(self, bag_path: _Optional[str] = ...) -> None: ...

class BuildingPipelineInfo(_message.Message):
    __slots__ = ("pipeline_name",)
    PIPELINE_NAME_FIELD_NUMBER: _ClassVar[int]
    pipeline_name: str
    def __init__(self, pipeline_name: _Optional[str] = ...) -> None: ...

class BagIsNotEvaluation(_message.Message):
    __slots__ = ("bag_path",)
    BAG_PATH_FIELD_NUMBER: _ClassVar[int]
    bag_path: str
    def __init__(self, bag_path: _Optional[str] = ...) -> None: ...

class HorusBagRunning(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class AutoGroundCalibrationWarning(_message.Message):
    __slots__ = ("failed_lidar_ids",)
    FAILED_LIDAR_IDS_FIELD_NUMBER: _ClassVar[int]
    failed_lidar_ids: str
    def __init__(self, failed_lidar_ids: _Optional[str] = ...) -> None: ...

class AutoGroundCalibrationError(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class ObjectDetectorNotLoadedWarning(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class CalibrationIsRunningError(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class ModelInitInProgressInfo(_message.Message):
    __slots__ = ("component",)
    COMPONENT_FIELD_NUMBER: _ClassVar[int]
    component: str
    def __init__(self, component: _Optional[str] = ...) -> None: ...

class RpcTimeoutWithResolutionWarning(_message.Message):
    __slots__ = ("request_name", "endpoint", "elapsed", "resolution")
    REQUEST_NAME_FIELD_NUMBER: _ClassVar[int]
    ENDPOINT_FIELD_NUMBER: _ClassVar[int]
    ELAPSED_FIELD_NUMBER: _ClassVar[int]
    RESOLUTION_FIELD_NUMBER: _ClassVar[int]
    request_name: str
    endpoint: str
    elapsed: _metadata_pb2.LogMetadata.Duration
    resolution: str
    def __init__(self, request_name: _Optional[str] = ..., endpoint: _Optional[str] = ..., elapsed: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ..., resolution: _Optional[str] = ...) -> None: ...

class CalibrationWasCancelledInfo(_message.Message):
    __slots__ = ("calibration_process_name",)
    CALIBRATION_PROCESS_NAME_FIELD_NUMBER: _ClassVar[int]
    calibration_process_name: str
    def __init__(self, calibration_process_name: _Optional[str] = ...) -> None: ...

class CalibrationMapRecordingFailedToStart(_message.Message):
    __slots__ = ("path",)
    PATH_FIELD_NUMBER: _ClassVar[int]
    path: str
    def __init__(self, path: _Optional[str] = ...) -> None: ...

class DetectionPipelineRequestedResetInfo(_message.Message):
    __slots__ = ("reason",)
    REASON_FIELD_NUMBER: _ClassVar[int]
    reason: str
    def __init__(self, reason: _Optional[str] = ...) -> None: ...

class PreprocessingServicePipelineUnavailable(_message.Message):
    __slots__ = ("reason",)
    REASON_FIELD_NUMBER: _ClassVar[int]
    reason: str
    def __init__(self, reason: _Optional[str] = ...) -> None: ...

class CircularRecordingDisabledWarning(_message.Message):
    __slots__ = ("reason",)
    REASON_FIELD_NUMBER: _ClassVar[int]
    reason: str
    def __init__(self, reason: _Optional[str] = ...) -> None: ...

class SnapshotAlreadyRunningWarning(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class ActiveProjectChangedInfo(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class ProjectConfigUpdatedInfo(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class InvalidLidarTimestamp(_message.Message):
    __slots__ = ("lidar_id", "timestamp", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    TIMESTAMP_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    timestamp: _metadata_pb2.LogMetadata.Timestamp
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., timestamp: _Optional[_Union[_metadata_pb2.LogMetadata.Timestamp, _Mapping]] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class CalibrationAccumulatingPointsInfo(_message.Message):
    __slots__ = ("time",)
    TIME_FIELD_NUMBER: _ClassVar[int]
    time: _metadata_pb2.LogMetadata.Duration
    def __init__(self, time: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ...) -> None: ...

class SparseNoiseFilterUsageNonRotationalLidars(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class FileWriteWarning(_message.Message):
    __slots__ = ("filename", "details")
    FILENAME_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    filename: str
    details: str
    def __init__(self, filename: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class LicenseForbiddenFeature(_message.Message):
    __slots__ = ("feature_name",)
    FEATURE_NAME_FIELD_NUMBER: _ClassVar[int]
    feature_name: str
    def __init__(self, feature_name: _Optional[str] = ...) -> None: ...

class FailedToUpdateConfiguration(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class ObstructionDetectorBadReferenceWarning(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class ProjectFileInvalidPermissionsError(_message.Message):
    __slots__ = ("filename",)
    FILENAME_FIELD_NUMBER: _ClassVar[int]
    filename: str
    def __init__(self, filename: _Optional[str] = ...) -> None: ...

class PipelineSchedulerError(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class MultiLidarCalibrationWarning(_message.Message):
    __slots__ = ("failed_lidar_ids",)
    FAILED_LIDAR_IDS_FIELD_NUMBER: _ClassVar[int]
    failed_lidar_ids: str
    def __init__(self, failed_lidar_ids: _Optional[str] = ...) -> None: ...

class MultiLidarCalibrationError(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class OldBagWarning(_message.Message):
    __slots__ = ("bag_path", "version")
    BAG_PATH_FIELD_NUMBER: _ClassVar[int]
    VERSION_FIELD_NUMBER: _ClassVar[int]
    bag_path: str
    version: int
    def __init__(self, bag_path: _Optional[str] = ..., version: _Optional[int] = ...) -> None: ...

class UpgradingBagInfo(_message.Message):
    __slots__ = ("bag_path",)
    BAG_PATH_FIELD_NUMBER: _ClassVar[int]
    bag_path: str
    def __init__(self, bag_path: _Optional[str] = ...) -> None: ...

class BagCalibrationSaveFailed(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class BagUpgradeFailed(_message.Message):
    __slots__ = ("bag_path", "details")
    BAG_PATH_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    bag_path: str
    details: str
    def __init__(self, bag_path: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class UnknownLidarError(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class InvalidPointCloudWarning(_message.Message):
    __slots__ = ("lidar_id", "reason", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    REASON_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    reason: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., reason: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarIsDroppingPackets(_message.Message):
    __slots__ = ("lidar_id", "num_total_dropped_packets", "num_total_expected_packets", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    NUM_TOTAL_DROPPED_PACKETS_FIELD_NUMBER: _ClassVar[int]
    NUM_TOTAL_EXPECTED_PACKETS_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    num_total_dropped_packets: int
    num_total_expected_packets: int
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., num_total_dropped_packets: _Optional[int] = ..., num_total_expected_packets: _Optional[int] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class RemovedInvalidLidarsFromConfigWarning(_message.Message):
    __slots__ = ("backup_path",)
    BACKUP_PATH_FIELD_NUMBER: _ClassVar[int]
    backup_path: str
    def __init__(self, backup_path: _Optional[str] = ...) -> None: ...

class CalibrationMapRecordingFailed(_message.Message):
    __slots__ = ("message",)
    MESSAGE_FIELD_NUMBER: _ClassVar[int]
    message: str
    def __init__(self, message: _Optional[str] = ...) -> None: ...

class DetectionNodeNotFoundError(_message.Message):
    __slots__ = ("node_name",)
    NODE_NAME_FIELD_NUMBER: _ClassVar[int]
    node_name: str
    def __init__(self, node_name: _Optional[str] = ...) -> None: ...

class CreatedVersionBackupInfo(_message.Message):
    __slots__ = ("old_horus_version", "new_horus_version", "backup_path")
    OLD_HORUS_VERSION_FIELD_NUMBER: _ClassVar[int]
    NEW_HORUS_VERSION_FIELD_NUMBER: _ClassVar[int]
    BACKUP_PATH_FIELD_NUMBER: _ClassVar[int]
    old_horus_version: str
    new_horus_version: str
    backup_path: str
    def __init__(self, old_horus_version: _Optional[str] = ..., new_horus_version: _Optional[str] = ..., backup_path: _Optional[str] = ...) -> None: ...

class PlyFileLoadFailedError(_message.Message):
    __slots__ = ("file_path", "details")
    FILE_PATH_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    file_path: str
    details: str
    def __init__(self, file_path: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class HesaiDriverLifecycle(_message.Message):
    __slots__ = ("action", "lidar_id", "lidar_name")
    ACTION_FIELD_NUMBER: _ClassVar[int]
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    action: str
    lidar_id: str
    lidar_name: str
    def __init__(self, action: _Optional[str] = ..., lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class HesaiDriverError(_message.Message):
    __slots__ = ("details", "lidar_id", "lidar_name")
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    details: str
    lidar_id: str
    lidar_name: str
    def __init__(self, details: _Optional[str] = ..., lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class HesaiPacketProcessingFailed(_message.Message):
    __slots__ = ("lidar_id", "details", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    details: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., details: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class HesaiCorrectionFileError(_message.Message):
    __slots__ = ("file_type", "details", "lidar_id", "lidar_name")
    FILE_TYPE_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    file_type: str
    details: str
    lidar_id: str
    lidar_name: str
    def __init__(self, file_type: _Optional[str] = ..., details: _Optional[str] = ..., lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class HesaiPacketStatistics(_message.Message):
    __slots__ = ("packets_received", "packets_published", "packets_dropped", "packets_decode_failed", "success_rate", "lidar_id", "lidar_name")
    PACKETS_RECEIVED_FIELD_NUMBER: _ClassVar[int]
    PACKETS_PUBLISHED_FIELD_NUMBER: _ClassVar[int]
    PACKETS_DROPPED_FIELD_NUMBER: _ClassVar[int]
    PACKETS_DECODE_FAILED_FIELD_NUMBER: _ClassVar[int]
    SUCCESS_RATE_FIELD_NUMBER: _ClassVar[int]
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    packets_received: int
    packets_published: int
    packets_dropped: int
    packets_decode_failed: int
    success_rate: float
    lidar_id: str
    lidar_name: str
    def __init__(self, packets_received: _Optional[int] = ..., packets_published: _Optional[int] = ..., packets_dropped: _Optional[int] = ..., packets_decode_failed: _Optional[int] = ..., success_rate: _Optional[float] = ..., lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class PlyFileWriteFailedError(_message.Message):
    __slots__ = ("file_path", "details")
    FILE_PATH_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    file_path: str
    details: str
    def __init__(self, file_path: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class ProjectSaveError(_message.Message):
    __slots__ = ("error_message",)
    ERROR_MESSAGE_FIELD_NUMBER: _ClassVar[int]
    error_message: str
    def __init__(self, error_message: _Optional[str] = ...) -> None: ...

class SaveStaticEnvironmentSuccess(_message.Message):
    __slots__ = ("path",)
    PATH_FIELD_NUMBER: _ClassVar[int]
    path: str
    def __init__(self, path: _Optional[str] = ...) -> None: ...

class SaveStaticEnvironmentFailed(_message.Message):
    __slots__ = ("path", "details")
    PATH_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    path: str
    details: str
    def __init__(self, path: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class LoadStaticEnvironmentSuccess(_message.Message):
    __slots__ = ("path",)
    PATH_FIELD_NUMBER: _ClassVar[int]
    path: str
    def __init__(self, path: _Optional[str] = ...) -> None: ...

class LoadStaticEnvironmentFailed(_message.Message):
    __slots__ = ("path", "details")
    PATH_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    path: str
    details: str
    def __init__(self, path: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...

class AttemptToInjectInvalidLidarIdWarning(_message.Message):
    __slots__ = ("lidar_id",)
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    def __init__(self, lidar_id: _Optional[str] = ...) -> None: ...

class ResetBundledPacketDueToUnexpectedPacket(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class PacketBundlerDroppedPacketsWarning(_message.Message):
    __slots__ = ("lidar_id", "num_dropped", "duration", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    NUM_DROPPED_FIELD_NUMBER: _ClassVar[int]
    DURATION_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    num_dropped: int
    duration: _metadata_pb2.LogMetadata.Duration
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., num_dropped: _Optional[int] = ..., duration: _Optional[_Union[_metadata_pb2.LogMetadata.Duration, _Mapping]] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class PacketBundlerFrameJumpWarning(_message.Message):
    __slots__ = ("lidar_id", "frame_id", "next_frame_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    FRAME_ID_FIELD_NUMBER: _ClassVar[int]
    NEXT_FRAME_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    frame_id: int
    next_frame_id: int
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., frame_id: _Optional[int] = ..., next_frame_id: _Optional[int] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarCorrectionLoadingSuccess(_message.Message):
    __slots__ = ("correction_type", "lidar_id", "lidar_name")
    CORRECTION_TYPE_FIELD_NUMBER: _ClassVar[int]
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    correction_type: str
    lidar_id: str
    lidar_name: str
    def __init__(self, correction_type: _Optional[str] = ..., lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarCorrectionLoadingFailure(_message.Message):
    __slots__ = ("correction_type", "details", "lidar_id", "lidar_name")
    CORRECTION_TYPE_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    correction_type: str
    details: str
    lidar_id: str
    lidar_name: str
    def __init__(self, correction_type: _Optional[str] = ..., details: _Optional[str] = ..., lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class HesaiPacketStatisticsLidar(_message.Message):
    __slots__ = ("packets_received", "packets_published", "packets_dropped", "packets_decode_failed", "success_rate", "lidar_id")
    PACKETS_RECEIVED_FIELD_NUMBER: _ClassVar[int]
    PACKETS_PUBLISHED_FIELD_NUMBER: _ClassVar[int]
    PACKETS_DROPPED_FIELD_NUMBER: _ClassVar[int]
    PACKETS_DECODE_FAILED_FIELD_NUMBER: _ClassVar[int]
    SUCCESS_RATE_FIELD_NUMBER: _ClassVar[int]
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    packets_received: int
    packets_published: int
    packets_dropped: int
    packets_decode_failed: int
    success_rate: float
    lidar_id: str
    def __init__(self, packets_received: _Optional[int] = ..., packets_published: _Optional[int] = ..., packets_dropped: _Optional[int] = ..., packets_decode_failed: _Optional[int] = ..., success_rate: _Optional[float] = ..., lidar_id: _Optional[str] = ...) -> None: ...

class LidarTiltDetectionAlignedToCalibrationMapInfo(_message.Message):
    __slots__ = ("lidar_id", "lidar_name")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ...) -> None: ...

class LidarTiltDetectionMisalignedToCalibrationMapWarning(_message.Message):
    __slots__ = ("lidar_id", "lidar_name", "angle", "distance")
    LIDAR_ID_FIELD_NUMBER: _ClassVar[int]
    LIDAR_NAME_FIELD_NUMBER: _ClassVar[int]
    ANGLE_FIELD_NUMBER: _ClassVar[int]
    DISTANCE_FIELD_NUMBER: _ClassVar[int]
    lidar_id: str
    lidar_name: str
    angle: float
    distance: float
    def __init__(self, lidar_id: _Optional[str] = ..., lidar_name: _Optional[str] = ..., angle: _Optional[float] = ..., distance: _Optional[float] = ...) -> None: ...

class LidarOriginalPoseDiffersForAutoCorrectionError(_message.Message):
    __slots__ = ()
    def __init__(self) -> None: ...

class RecoveredCarIdsInfo(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class FailedToRecoverCarIds(_message.Message):
    __slots__ = ("details",)
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    details: str
    def __init__(self, details: _Optional[str] = ...) -> None: ...

class FailedToEmplaceRecoveredCarId(_message.Message):
    __slots__ = ("object_id",)
    OBJECT_ID_FIELD_NUMBER: _ClassVar[int]
    object_id: int
    def __init__(self, object_id: _Optional[int] = ...) -> None: ...

class PersistentStorageError(_message.Message):
    __slots__ = ("operation", "filepath")
    OPERATION_FIELD_NUMBER: _ClassVar[int]
    FILEPATH_FIELD_NUMBER: _ClassVar[int]
    operation: str
    filepath: str
    def __init__(self, operation: _Optional[str] = ..., filepath: _Optional[str] = ...) -> None: ...

class TrackCapacityExceededWarning(_message.Message):
    __slots__ = ("attempted_count", "max_tracks")
    ATTEMPTED_COUNT_FIELD_NUMBER: _ClassVar[int]
    MAX_TRACKS_FIELD_NUMBER: _ClassVar[int]
    attempted_count: int
    max_tracks: int
    def __init__(self, attempted_count: _Optional[int] = ..., max_tracks: _Optional[int] = ...) -> None: ...

class TrackerStatePathUnavailableWarning(_message.Message):
    __slots__ = ("project_name",)
    PROJECT_NAME_FIELD_NUMBER: _ClassVar[int]
    project_name: str
    def __init__(self, project_name: _Optional[str] = ...) -> None: ...

class TrackerIdRecoveryFailedError(_message.Message):
    __slots__ = ("id", "error_message")
    ID_FIELD_NUMBER: _ClassVar[int]
    ERROR_MESSAGE_FIELD_NUMBER: _ClassVar[int]
    id: int
    error_message: str
    def __init__(self, id: _Optional[int] = ..., error_message: _Optional[str] = ...) -> None: ...

class TrackerIdFastForwardFailedError(_message.Message):
    __slots__ = ("target_id", "error_message")
    TARGET_ID_FIELD_NUMBER: _ClassVar[int]
    ERROR_MESSAGE_FIELD_NUMBER: _ClassVar[int]
    target_id: int
    error_message: str
    def __init__(self, target_id: _Optional[int] = ..., error_message: _Optional[str] = ...) -> None: ...

class CircularRecordingSnapshotCreated(_message.Message):
    __slots__ = ("message_count", "actual_duration_seconds", "max_buffer_duration_seconds")
    MESSAGE_COUNT_FIELD_NUMBER: _ClassVar[int]
    ACTUAL_DURATION_SECONDS_FIELD_NUMBER: _ClassVar[int]
    MAX_BUFFER_DURATION_SECONDS_FIELD_NUMBER: _ClassVar[int]
    message_count: int
    actual_duration_seconds: float
    max_buffer_duration_seconds: float
    def __init__(self, message_count: _Optional[int] = ..., actual_duration_seconds: _Optional[float] = ..., max_buffer_duration_seconds: _Optional[float] = ...) -> None: ...

class CircularRecordingFileOperationError(_message.Message):
    __slots__ = ("operation", "file_path", "details")
    OPERATION_FIELD_NUMBER: _ClassVar[int]
    FILE_PATH_FIELD_NUMBER: _ClassVar[int]
    DETAILS_FIELD_NUMBER: _ClassVar[int]
    operation: str
    file_path: str
    details: str
    def __init__(self, operation: _Optional[str] = ..., file_path: _Optional[str] = ..., details: _Optional[str] = ...) -> None: ...
