#include "horus/logs/format.h"

#include <chrono>
#include <cstdint>

#include "horus/pb/logs/message_pb.h"
#include "horus/pb/logs/metadata_pb.h"
#include "horus/strings/chrono.h"
#include "horus/strings/str_sink.h"
#include "horus/strings/str_sink_erased.h"

namespace horus {
namespace sdk {
namespace pb {

void HorusStringify(const ErasedSink& sink, const LogMetadata_Duration& duration) {
  std::chrono::milliseconds const milliseconds{static_cast<std::int64_t>(duration.ms())};
  HorusStringify(sink, AnyDuration{milliseconds});
}

void HorusStringify(const ErasedSink& sink, const LogMetadata_SourceLocation& source_location) {
  StrAppendToSink(sink, source_location.file(), ":", source_location.line());
}

void HorusStringify(const ErasedSink& sink, const LogMetadata_Timestamp& timestamp) {
  std::chrono::milliseconds const ms_since_epoch{static_cast<std::int64_t>(timestamp.ms_since_epoch())};
  std::chrono::system_clock::time_point const time_point{ms_since_epoch};
  HorusStringify(sink, Iso8601{time_point});
}

void HorusStringify(const ErasedSink& sink, const LogData& log_data) {
  switch (log_data.data_case()) {
  case LogData::DataOneof::kGeneric: {
    const logs::Generic& data{log_data.generic()};
    StrAppendToSink(sink, data.message());
    break;
  }
  case LogData::DataOneof::kOom: {
    StrAppendToSink(sink, "Out of memory");
    break;
  }
  case LogData::DataOneof::kRpcConnectionError: {
    const logs::RpcConnectionError& data{log_data.rpc_connection_error()};
    StrAppendToSink(sink, "Cannot initiate connection to ", data.target_service(), " at ", data.target_uri(), ": ", data.details());
    break;
  }
  case LogData::DataOneof::kInvalidProjectName: {
    const logs::InvalidProjectName& data{log_data.invalid_project_name()};
    StrAppendToSink(sink, "Project name `", data.name(), "` is invalid: ", data.details());
    break;
  }
  case LogData::DataOneof::kProjectNotFound: {
    const logs::ProjectNotFound& data{log_data.project_not_found()};
    StrAppendToSink(sink, "Project `", data.name(), "` not found");
    break;
  }
  case LogData::DataOneof::kProjectAlreadyExists: {
    const logs::ProjectAlreadyExists& data{log_data.project_already_exists()};
    StrAppendToSink(sink, "Project `", data.name(), "` already exists");
    break;
  }
  case LogData::DataOneof::kInvalidConfiguration: {
    StrAppendToSink(sink, "Configuration is invalid");
    break;
  }
  case LogData::DataOneof::kEntityNotFound: {
    const logs::EntityNotFound& data{log_data.entity_not_found()};
    StrAppendToSink(sink, data.entity_type(), " `", data.id(), "` not found");
    break;
  }
  case LogData::DataOneof::kActiveProjectCannotBeDeleted: {
    StrAppendToSink(sink, "Active project cannot be deleted");
    break;
  }
  case LogData::DataOneof::kRpcDisconnectionError: {
    const logs::RpcDisconnectionError& data{log_data.rpc_disconnection_error()};
    StrAppendToSink(sink, "Cannot terminate connection to ", data.target_service(), " at ", data.target_uri(), ": ", data.details());
    break;
  }
  case LogData::DataOneof::kDroppedLogs: {
    const logs::DroppedLogs& data{log_data.dropped_logs()};
    StrAppendToSink(sink, "Dropped ", data.n(), " log messages");
    break;
  }
  case LogData::DataOneof::kOpenedProject: {
    const logs::OpenedProject& data{log_data.opened_project()};
    StrAppendToSink(sink, "Opened project ", data.project_name());
    break;
  }
  case LogData::DataOneof::kCreatedProject: {
    const logs::CreatedProject& data{log_data.created_project()};
    StrAppendToSink(sink, "Created project ", data.project_name(), " from ", data.source_name());
    break;
  }
  case LogData::DataOneof::kConfigUnavailable: {
    StrAppendToSink(sink, "Configuration is unavailable; request cannot be processed.");
    break;
  }
  case LogData::DataOneof::kInvalidRequest: {
    const logs::InvalidRequest& data{log_data.invalid_request()};
    StrAppendToSink(sink, "Invalid request received: ", data.details());
    break;
  }
  case LogData::DataOneof::kSanityCheckError: {
    const logs::SanityCheckError& data{log_data.sanity_check_error()};
    StrAppendToSink(sink, data.sanity_check_name(), " failed: ", data.details());
    break;
  }
  case LogData::DataOneof::kBagFailedToOpen: {
    const logs::BagFailedToOpen& data{log_data.bag_failed_to_open()};
    StrAppendToSink(sink, "The bag file (", data.horus_bag_path(), ") could not be opened: ", data.details());
    break;
  }
  case LogData::DataOneof::kBagFailedToClose: {
    const logs::BagFailedToClose& data{log_data.bag_failed_to_close()};
    StrAppendToSink(sink, "Bag file could not be closed: ", data.details());
    break;
  }
  case LogData::DataOneof::kBagConversionFailed: {
    const logs::BagConversionFailed& data{log_data.bag_conversion_failed()};
    StrAppendToSink(sink, "Bag file conversion failed: ", data.details());
    break;
  }
  case LogData::DataOneof::kBagFailedToWrite: {
    const logs::BagFailedToWrite& data{log_data.bag_failed_to_write()};
    StrAppendToSink(sink, "Bag file writing failed ", data.name(), ": ", data.details());
    break;
  }
  case LogData::DataOneof::kCalibrationError: {
    const logs::CalibrationError& data{log_data.calibration_error()};
    StrAppendToSink(sink, "Calibration failed: ", data.details());
    break;
  }
  case LogData::DataOneof::kProjectManagerFailedToStartRecording: {
    const logs::ProjectManagerFailedToStartRecording& data{log_data.project_manager_failed_to_start_recording()};
    StrAppendToSink(sink, "Failed to start recording: ", data.details());
    break;
  }
  case LogData::DataOneof::kProjectManagerFailedToStopRecording: {
    const logs::ProjectManagerFailedToStopRecording& data{log_data.project_manager_failed_to_stop_recording()};
    StrAppendToSink(sink, "Failed to stop recording: ", data.details());
    break;
  }
  case LogData::DataOneof::kServiceConnectionTimedOut: {
    const logs::ServiceConnectionTimedOut& data{log_data.service_connection_timed_out()};
    StrAppendToSink(sink, "The connection with the requested ", data.node_type(), " node with ID ", data.node_id(), " timed out");
    break;
  }
  case LogData::DataOneof::kBagRecorderAlreadyRunning: {
    const logs::BagRecorderAlreadyRunning& data{log_data.bag_recorder_already_running()};
    StrAppendToSink(sink, "Bag recorder is already running for ", data.path());
    break;
  }
  case LogData::DataOneof::kLicenseServerConnectionError: {
    const logs::LicenseServerConnectionError& data{log_data.license_server_connection_error()};
    StrAppendToSink(sink, "Could not connect to license server: ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kLicenseError: {
    const logs::LicenseError& data{log_data.license_error()};
    StrAppendToSink(sink, "Could not read license: ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kLicenseNotFoundError: {
    StrAppendToSink(sink, "Found no valid license.");
    break;
  }
  case LogData::DataOneof::kLicenseExpiredError: {
    const logs::LicenseExpiredError& data{log_data.license_expired_error()};
    StrAppendToSink(sink, "License expired on ", data.expiration_time(), ".");
    break;
  }
  case LogData::DataOneof::kLicenseExceededError: {
    const logs::LicenseExceededError& data{log_data.license_exceeded_error()};
    StrAppendToSink(sink, "Project exceeds number of LiDARs allowed by license (", data.lidar_count(), " > ", data.max_lidar_count(), ").");
    break;
  }
  case LogData::DataOneof::kLicenseHostMachineError: {
    const logs::LicenseHostMachineError& data{log_data.license_host_machine_error()};
    StrAppendToSink(sink, "Could not validate license with current hardware - ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kLicensePrivilegeError: {
    const logs::LicensePrivilegeError& data{log_data.license_privilege_error()};
    StrAppendToSink(sink, "Endpoint requires \"", data.missing_privilege(), "\" privilege: ", data.level_error(), ".");
    break;
  }
  case LogData::DataOneof::kLicenseActiveInfo: {
    StrAppendToSink(sink, "License is active.");
    break;
  }
  case LogData::DataOneof::kMultipleLicensesWarning: {
    const logs::MultipleLicensesWarning& data{log_data.multiple_licenses_warning()};
    StrAppendToSink(sink, "Found ", data.non_expired_licenses_count(), " non-expired concurrent licenses. Only \"", data.active_license_filename(), "\" is used.");
    break;
  }
  case LogData::DataOneof::kLicenseCurrentLicenseStatusInfo: {
    const logs::LicenseCurrentLicenseStatusInfo& data{log_data.license_current_license_status_info()};
    StrAppendToSink(sink, "Current license accepts up to ", data.max_lidar_count(), " and expires on ", data.expiration_time(), ".");
    break;
  }
  case LogData::DataOneof::kBagRecordingStoppedInfo: {
    const logs::BagRecordingStoppedInfo& data{log_data.bag_recording_stopped_info()};
    StrAppendToSink(sink, "Bag recording stopped for ", data.path(), ": ", data.details());
    break;
  }
  case LogData::DataOneof::kBagRecordingFailedToStart: {
    const logs::BagRecordingFailedToStart& data{log_data.bag_recording_failed_to_start()};
    StrAppendToSink(sink, "Bag recording failed to start: ", data.details());
    break;
  }
  case LogData::DataOneof::kBagRecordingStartedInfo: {
    const logs::BagRecordingStartedInfo& data{log_data.bag_recording_started_info()};
    StrAppendToSink(sink, "Bag recording started for ", data.path());
    break;
  }
  case LogData::DataOneof::kReplayRestartedInfo: {
    StrAppendToSink(sink, "Replay Restarted");
    break;
  }
  case LogData::DataOneof::kInputSourceChangeRequestedInfo: {
    const logs::InputSourceChangeRequestedInfo& data{log_data.input_source_change_requested_info()};
    StrAppendToSink(sink, "Input source ", data.source(), " requested");
    break;
  }
  case LogData::DataOneof::kInputSourceSwitchedInfo: {
    const logs::InputSourceSwitchedInfo& data{log_data.input_source_switched_info()};
    StrAppendToSink(sink, "Input source changed to ", data.source());
    break;
  }
  case LogData::DataOneof::kRpcTimeoutWarning: {
    const logs::RpcTimeoutWarning& data{log_data.rpc_timeout_warning()};
    StrAppendToSink(sink, "RPC request ", data.request_name(), " to ", data.endpoint(), " timed out after ", data.elapsed(), ".");
    break;
  }
  case LogData::DataOneof::kCannotWriteLogFile: {
    const logs::CannotWriteLogFile& data{log_data.cannot_write_log_file()};
    StrAppendToSink(sink, "Notification service cannot write log in ", data.path(), ": ", data.details());
    break;
  }
  case LogData::DataOneof::kPointCloudParsingFailureWarning: {
    const logs::PointCloudParsingFailureWarning& data{log_data.point_cloud_parsing_failure_warning()};
    StrAppendToSink(sink, "Point cloud parsing failed: ", data.details());
    break;
  }
  case LogData::DataOneof::kLidarIsDead: {
    const logs::LidarIsDead& data{log_data.lidar_is_dead()};
    StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is considered dead. No data has been received for a while.");
    break;
  }
  case LogData::DataOneof::kLidarIsNotDeadAnymore: {
    const logs::LidarIsNotDeadAnymore& data{log_data.lidar_is_not_dead_anymore()};
    StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is not considered dead anymore. A point cloud has been received again after some time.");
    break;
  }
  case LogData::DataOneof::kLidarIsObstructed: {
    const logs::LidarIsObstructed& data{log_data.lidar_is_obstructed()};
    StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is obstructed.");
    break;
  }
  case LogData::DataOneof::kLidarIsNotObstructedAnymore: {
    const logs::LidarIsNotObstructedAnymore& data{log_data.lidar_is_not_obstructed_anymore()};
    StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is not obstructed anymore.");
    break;
  }
  case LogData::DataOneof::kLidarIsTilted: {
    const logs::LidarIsTilted& data{log_data.lidar_is_tilted()};
    StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is tilted.");
    break;
  }
  case LogData::DataOneof::kLidarIsNotTiltedAnymore: {
    const logs::LidarIsNotTiltedAnymore& data{log_data.lidar_is_not_tilted_anymore()};
    StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is not tilted anymore.");
    break;
  }
  case LogData::DataOneof::kLidarHasBeenAutomaticallyRecalibrated: {
    const logs::LidarHasBeenAutomaticallyRecalibrated& data{log_data.lidar_has_been_automatically_recalibrated()};
    StrAppendToSink(sink, "The lidar ", data.lidar_id(), " has been automatically re-calibrated.");
    break;
  }
  case LogData::DataOneof::kReceivedFirstDataForLidar: {
    const logs::ReceivedFirstDataForLidar& data{log_data.received_first_data_for_lidar()};
    StrAppendToSink(sink, "Data has been received for the first time for the lidar ", data.lidar_id(), ".");
    break;
  }
  case LogData::DataOneof::kTerminationFailureError: {
    const logs::TerminationFailureError& data{log_data.termination_failure_error()};
    StrAppendToSink(sink, "Failed to destruct ", data.component(), ": ", data.details());
    break;
  }
  case LogData::DataOneof::kFrameProcessingError: {
    const logs::FrameProcessingError& data{log_data.frame_processing_error()};
    StrAppendToSink(sink, "The Frame Task Scheduler failed to process frame task: ", data.details());
    break;
  }
  case LogData::DataOneof::kThreadPoolUnavailableError: {
    const logs::ThreadPoolUnavailableError& data{log_data.thread_pool_unavailable_error()};
    StrAppendToSink(sink, "Thread pool is not available: ", data.consequence());
    break;
  }
  case LogData::DataOneof::kInvalidArgument: {
    const logs::InvalidArgument& data{log_data.invalid_argument()};
    StrAppendToSink(sink, "Invalid argument: ", data.details());
    break;
  }
  case LogData::DataOneof::kComponentInitializationFailureFatal: {
    const logs::ComponentInitializationFailureFatal& data{log_data.component_initialization_failure_fatal()};
    StrAppendToSink(sink, "Failed to initialize ", data.component(), ": ", data.details());
    break;
  }
  case LogData::DataOneof::kUnhandledEnumCaseError: {
    const logs::UnhandledEnumCaseError& data{log_data.unhandled_enum_case_error()};
    StrAppendToSink(sink, "Unhandled enum case ", data.case_name(), " at ", data.location());
    break;
  }
  case LogData::DataOneof::kBagEmptyError: {
    StrAppendToSink(sink, "The replay bag file is empty.");
    break;
  }
  case LogData::DataOneof::kDiscardingDataError: {
    const logs::DiscardingDataError& data{log_data.discarding_data_error()};
    StrAppendToSink(sink, "Discarding data in ", data.component(), ": ", data.details());
    break;
  }
  case LogData::DataOneof::kDiscardingDataWarning: {
    const logs::DiscardingDataWarning& data{log_data.discarding_data_warning()};
    StrAppendToSink(sink, "Discarding data in ", data.component(), ": ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kNothingToProcess: {
    const logs::NothingToProcess& data{log_data.nothing_to_process()};
    StrAppendToSink(sink, "Nothing to process: ", data.details());
    break;
  }
  case LogData::DataOneof::kInvalidComponentConfiguration: {
    const logs::InvalidComponentConfiguration& data{log_data.invalid_component_configuration()};
    StrAppendToSink(sink, "Invalid configuration for ", data.component(), ": ", data.details());
    break;
  }
  case LogData::DataOneof::kChannelReceiverNotFoundWarning: {
    const logs::ChannelReceiverNotFoundWarning& data{log_data.channel_receiver_not_found_warning()};
    StrAppendToSink(sink, "Channel receiver ", data.key(), " not found");
    break;
  }
  case LogData::DataOneof::kModelLoadFailure: {
    const logs::ModelLoadFailure& data{log_data.model_load_failure()};
    StrAppendToSink(sink, "Model ", data.model(), " failed to load: ", data.details());
    break;
  }
  case LogData::DataOneof::kModelExecutionFailureError: {
    const logs::ModelExecutionFailureError& data{log_data.model_execution_failure_error()};
    StrAppendToSink(sink, "Model ", data.model(), " failed to execute: ", data.details());
    break;
  }
  case LogData::DataOneof::kServiceReadyInfo: {
    const logs::ServiceReadyInfo& data{log_data.service_ready_info()};
    StrAppendToSink(sink, data.service(), " service is ready");
    break;
  }
  case LogData::DataOneof::kModelPreparingInfo: {
    const logs::ModelPreparingInfo& data{log_data.model_preparing_info()};
    StrAppendToSink(sink, "Preparing model ", data.model(), ", it may take several seconds to stabilize.");
    break;
  }
  case LogData::DataOneof::kModelInitializedInfo: {
    const logs::ModelInitializedInfo& data{log_data.model_initialized_info()};
    StrAppendToSink(sink, data.name(), " model has been initialized and is ready for use.");
    break;
  }
  case LogData::DataOneof::kModelInitializationFailureWarning: {
    const logs::ModelInitializationFailureWarning& data{log_data.model_initialization_failure_warning()};
    StrAppendToSink(sink, "Failed to initialize model ", data.model(), ": ", data.details());
    break;
  }
  case LogData::DataOneof::kRosSpinnerStoppedWarning: {
    const logs::RosSpinnerStoppedWarning& data{log_data.ros_spinner_stopped_warning()};
    StrAppendToSink(sink, "ROS spinner stopped: ", data.details());
    break;
  }
  case LogData::DataOneof::kActorSystemUnavailableError: {
    StrAppendToSink(sink, "Actor system is not available. Please reinstantiate the pipeline.");
    break;
  }
  case LogData::DataOneof::kConfigNodeNotFoundError: {
    const logs::ConfigNodeNotFoundError& data{log_data.config_node_not_found_error()};
    StrAppendToSink(sink, "Configuration for node ", data.node(), " is unavailable");
    break;
  }
  case LogData::DataOneof::kBagTimestampOutOfOrderError: {
    const logs::BagTimestampOutOfOrderError& data{log_data.bag_timestamp_out_of_order_error()};
    StrAppendToSink(sink, "Timestamps are out of order: ", data.prior_timestamp(), " > ", data.current_timestamp());
    break;
  }
  case LogData::DataOneof::kBagReplayUnexpectedTimestampError: {
    const logs::BagReplayUnexpectedTimestampError& data{log_data.bag_replay_unexpected_timestamp_error()};
    StrAppendToSink(sink, "Unexpected timestamp: expected ", data.expected_timestamp(), " but received ", data.received_timestamp());
    break;
  }
  case LogData::DataOneof::kWebsocketClosedInfo: {
    const logs::WebsocketClosedInfo& data{log_data.websocket_closed_info()};
    StrAppendToSink(sink, "WebSocket RPC connection to ", data.uri(), " closed");
    break;
  }
  case LogData::DataOneof::kWebsocketOpenedInfo: {
    const logs::WebsocketOpenedInfo& data{log_data.websocket_opened_info()};
    StrAppendToSink(sink, "WebSocket RPC connection to ", data.uri(), " opened (as ", data.endpoint_is_server_client(), ")");
    break;
  }
  case LogData::DataOneof::kSubscriberDisconnectedInfo: {
    const logs::SubscriberDisconnectedInfo& data{log_data.subscriber_disconnected_info()};
    StrAppendToSink(sink, "Subscriber ", data.name(), " disconnected (", data.uri(), ")");
    break;
  }
  case LogData::DataOneof::kThreadPoolSlowingDownWarning: {
    const logs::ThreadPoolSlowingDownWarning& data{log_data.thread_pool_slowing_down_warning()};
    StrAppendToSink(sink, "Thread pool is slowing down for ", data.number(), " times!");
    break;
  }
  case LogData::DataOneof::kThreadPoolNotRespondingWarning: {
    const logs::ThreadPoolNotRespondingWarning& data{log_data.thread_pool_not_responding_warning()};
    StrAppendToSink(sink, "Thread pool is not responding for ", data.not_responding_for(), ".");
    break;
  }
  case LogData::DataOneof::kThreadPoolBrokenPromiseWarning: {
    StrAppendToSink(sink, "Thread pool task future is no longer available. The thread pool might have been restarted during events such as input source change.");
    break;
  }
  case LogData::DataOneof::kBoxFittingPointsBelowBaseWarning: {
    StrAppendToSink(sink, "Some points are below the base Z-coordinate when fitting the box. This might affect the detection results.");
    break;
  }
  case LogData::DataOneof::kFailedToRemoveStalePointsWarning: {
    const logs::FailedToRemoveStalePointsWarning& data{log_data.failed_to_remove_stale_points_warning()};
    StrAppendToSink(sink, "Failed to remove stale points: ", data.details());
    break;
  }
  case LogData::DataOneof::kIrregularBroadcastingPeriodWarning: {
    const logs::IrregularBroadcastingPeriodWarning& data{log_data.irregular_broadcasting_period_warning()};
    StrAppendToSink(sink, data.num_irregular(), " irregular broadcast(s) detected in the last ", data.duration(), " (mean deviation: ", data.mean_deviation(), ").");
    break;
  }
  case LogData::DataOneof::kClustererPointsOutOfRangeWarning: {
    const logs::ClustererPointsOutOfRangeWarning& data{log_data.clusterer_points_out_of_range_warning()};
    StrAppendToSink(sink, "There are ", data.num_points(), " points out of range in the clusterer. Some points: ", data.sample_points(), ".");
    break;
  }
  case LogData::DataOneof::kInternalError: {
    const logs::InternalError& data{log_data.internal_error()};
    StrAppendToSink(sink, "An internal error occurred: ", data.details());
    break;
  }
  case LogData::DataOneof::kInternalFatal: {
    const logs::InternalFatal& data{log_data.internal_fatal()};
    StrAppendToSink(sink, "An internal fatal error occurred: ", data.details());
    break;
  }
  case LogData::DataOneof::kServiceStartingInfo: {
    const logs::ServiceStartingInfo& data{log_data.service_starting_info()};
    StrAppendToSink(sink, data.service(), " is starting");
    break;
  }
  case LogData::DataOneof::kConfigNodeNotFoundFatal: {
    const logs::ConfigNodeNotFoundFatal& data{log_data.config_node_not_found_fatal()};
    StrAppendToSink(sink, "Configuration for node ", data.node(), " is unavailable");
    break;
  }
  case LogData::DataOneof::kServiceSetupError: {
    const logs::ServiceSetupError& data{log_data.service_setup_error()};
    StrAppendToSink(sink, "Failed to setup: ", data.details());
    break;
  }
  case LogData::DataOneof::kFilesystemError: {
    const logs::FilesystemError& data{log_data.filesystem_error()};
    StrAppendToSink(sink, "Filesystem error: ", data.details());
    break;
  }
  case LogData::DataOneof::kInvalidPresetWarning: {
    const logs::InvalidPresetWarning& data{log_data.invalid_preset_warning()};
    StrAppendToSink(sink, "Tried to apply invalid preset ", data.preset(), " due to ", data.reason(), ", falling back to ", data.fallback_name(), ".");
    break;
  }
  case LogData::DataOneof::kWebsocketFailedClearPendingError: {
    const logs::WebsocketFailedClearPendingError& data{log_data.websocket_failed_clear_pending_error()};
    StrAppendToSink(sink, "WebSocket failed to clear pending queues: ", data.exception());
    break;
  }
  case LogData::DataOneof::kWebsocketFailedToStopError: {
    const logs::WebsocketFailedToStopError& data{log_data.websocket_failed_to_stop_error()};
    StrAppendToSink(sink, "WebSocket failed to stop: ", data.exception());
    break;
  }
  case LogData::DataOneof::kWebsocketFailedToCreateError: {
    const logs::WebsocketFailedToCreateError& data{log_data.websocket_failed_to_create_error()};
    StrAppendToSink(sink, "Failed to create RPC server ws://", data.host(), ":", data.port(), "/: ", data.exception());
    break;
  }
  case LogData::DataOneof::kUnexpectedRpcError: {
    const logs::UnexpectedRpcError& data{log_data.unexpected_rpc_error()};
    StrAppendToSink(sink, "Unexpected RPC error: ", data.details());
    break;
  }
  case LogData::DataOneof::kLicensePollFailed: {
    const logs::LicensePollFailed& data{log_data.license_poll_failed()};
    StrAppendToSink(sink, "Failed to poll license server: ", data.details());
    break;
  }
  case LogData::DataOneof::kLicenseExpiredWarning: {
    const logs::LicenseExpiredWarning& data{log_data.license_expired_warning()};
    StrAppendToSink(sink, "License expired on ", data.expiration_time(), ".");
    break;
  }
  case LogData::DataOneof::kLicenseUsageExceededWarning: {
    const logs::LicenseUsageExceededWarning& data{log_data.license_usage_exceeded_warning()};
    StrAppendToSink(sink, "License count of ", data.lidar_count(), " exceeds allowed usage of ", data.max_lidar_count());
    break;
  }
  case LogData::DataOneof::kStaticThreadPoolSlowTaskWarning: {
    const logs::StaticThreadPoolSlowTaskWarning& data{log_data.static_thread_pool_slow_task_warning()};
    StrAppendToSink(sink, "ThreadPool task took ", data.time_elapsed(), " to complete task enqueued in ", data.enqueue_location());
    break;
  }
  case LogData::DataOneof::kRpcUnsupportedServiceWarning: {
    const logs::RpcUnsupportedServiceWarning& data{log_data.rpc_unsupported_service_warning()};
    StrAppendToSink(sink, "Received request for unsupported service: ", data.service_id());
    break;
  }
  case LogData::DataOneof::kWebsocketHandlerProblem: {
    const logs::WebsocketHandlerProblem& data{log_data.websocket_handler_problem()};
    StrAppendToSink(sink, "WebSocket RPC handler encountered an error: ", data.what());
    break;
  }
  case LogData::DataOneof::kWebsocketDeserializeError: {
    const logs::WebsocketDeserializeError& data{log_data.websocket_deserialize_error()};
    StrAppendToSink(sink, "WebSocket RPC received message cannot be deserialized: ", data.what());
    break;
  }
  case LogData::DataOneof::kWebsocketExpiredRpcEndpointError: {
    const logs::WebsocketExpiredRpcEndpointError& data{log_data.websocket_expired_rpc_endpoint_error()};
    StrAppendToSink(sink, "Attempting to access expired WebSocket RPC endpoint ", data.uri());
    break;
  }
  case LogData::DataOneof::kWebsocketQueueOverloadedWarning: {
    const logs::WebsocketQueueOverloadedWarning& data{log_data.websocket_queue_overloaded_warning()};
    StrAppendToSink(sink, "Websocket message queue overloaded: ", data.current(), " / ", data.max());
    break;
  }
  case LogData::DataOneof::kRpcFailedToNotifyWarning: {
    const logs::RpcFailedToNotifyWarning& data{log_data.rpc_failed_to_notify_warning()};
    StrAppendToSink(sink, "Could not notify ", data.notification_name(), " to ", data.service(), " ", data.uri(), ": ", data.what());
    break;
  }
  case LogData::DataOneof::kConfigSubscriptionFailedWarning: {
    const logs::ConfigSubscriptionFailedWarning& data{log_data.config_subscription_failed_warning()};
    StrAppendToSink(sink, "Failed to subscribe to config: ", data.details());
    break;
  }
  case LogData::DataOneof::kThreadPoolClampedWorkersWarning: {
    const logs::ThreadPoolClampedWorkersWarning& data{log_data.thread_pool_clamped_workers_warning()};
    StrAppendToSink(sink, "Clamped thread pool workers to ", data.clamped(), " instead of ", data.requested());
    break;
  }
  case LogData::DataOneof::kStoppingHorusBagRecorderAlreadyStopped: {
    StrAppendToSink(sink, "Trying to stop the Horus bag recorder which is already stopped.");
    break;
  }
  case LogData::DataOneof::kRecorderConfigUpdateWhileRunning: {
    StrAppendToSink(sink, "An update for the Horus bag recorder has been received while it was running. This update will not be taken into account until the next recording.");
    break;
  }
  case LogData::DataOneof::kClampingDataWarning: {
    const logs::ClampingDataWarning& data{log_data.clamping_data_warning()};
    StrAppendToSink(sink, "Clamping point cloud: ", data.details());
    break;
  }
  case LogData::DataOneof::kLidarIncompatibleValues: {
    const logs::LidarIncompatibleValues& data{log_data.lidar_incompatible_values()};
    StrAppendToSink(sink, "Lidars ", data.lidar_ip1(), " and ", data.lidar_ip2(), " have incompatible ", data.value_names(), ": ", data.value1(), " vs. ", data.value2(), "; ", data.resolution(), ".");
    break;
  }
  case LogData::DataOneof::kCannotDetermineContainerIdError: {
    const logs::CannotDetermineContainerIdError& data{log_data.cannot_determine_container_id_error()};
    StrAppendToSink(sink, "Cannot determine current Docker container ID; unknown ", data.container_id_file_path(), " format.");
    break;
  }
  case LogData::DataOneof::kStartedLidarDriver: {
    const logs::StartedLidarDriver& data{log_data.started_lidar_driver()};
    StrAppendToSink(sink, "Started lidar driver container ", data.lidar_id(), ".");
    break;
  }
  case LogData::DataOneof::kCannotStartLidarDriver: {
    const logs::CannotStartLidarDriver& data{log_data.cannot_start_lidar_driver()};
    StrAppendToSink(sink, "Cannot start lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kStoppedLidarDriver: {
    const logs::StoppedLidarDriver& data{log_data.stopped_lidar_driver()};
    StrAppendToSink(sink, "Stopped lidar driver container ", data.lidar_id(), ".");
    break;
  }
  case LogData::DataOneof::kCannotStopLidarDriver: {
    const logs::CannotStopLidarDriver& data{log_data.cannot_stop_lidar_driver()};
    StrAppendToSink(sink, "Cannot stop lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kRestartedLidarDriver: {
    const logs::RestartedLidarDriver& data{log_data.restarted_lidar_driver()};
    StrAppendToSink(sink, "Restarted lidar driver container ", data.lidar_id(), ".");
    break;
  }
  case LogData::DataOneof::kCannotRestartLidarDriver: {
    const logs::CannotRestartLidarDriver& data{log_data.cannot_restart_lidar_driver()};
    StrAppendToSink(sink, "Cannot restart lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kRemovedUnusedLidarDriver: {
    const logs::RemovedUnusedLidarDriver& data{log_data.removed_unused_lidar_driver()};
    StrAppendToSink(sink, "Removed unused lidar driver container ", data.lidar_id(), ".");
    break;
  }
  case LogData::DataOneof::kCannotRemoveUnusedLidarDriver: {
    const logs::CannotRemoveUnusedLidarDriver& data{log_data.cannot_remove_unused_lidar_driver()};
    StrAppendToSink(sink, "Cannot remove unused lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kLidarDriverGcFailure: {
    const logs::LidarDriverGcFailure& data{log_data.lidar_driver_gc_failure()};
    StrAppendToSink(sink, "Error encountered while removing unused lidar driver containers: ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kIdSpaceExhausted: {
    const logs::IdSpaceExhausted& data{log_data.id_space_exhausted()};
    StrAppendToSink(sink, "Cannot allocate new identifier with prefix \"", data.prefix(), "\".");
    break;
  }
  case LogData::DataOneof::kPreprocessingToPointAggregatorPointsSkipped: {
    const logs::PreprocessingToPointAggregatorPointsSkipped& data{log_data.preprocessing_to_point_aggregator_points_skipped()};
    StrAppendToSink(sink, "The point cloud publishing to the point aggregator service has been skipped ", data.num_skipped_points(), " time(s) in the last ", data.check_interval(), ".");
    break;
  }
  case LogData::DataOneof::kMinMsgIntervalLessThanThreshold: {
    const logs::MinMsgIntervalLessThanThreshold& data{log_data.min_msg_interval_less_than_threshold()};
    StrAppendToSink(sink, "Discarding lidar points from ", data.lidar_id(), " since the time interval between two point-cloud messages is too close (<", data.threshold(), "). Adjust the Min-Message Interval parameter to change this behavior.");
    break;
  }
  case LogData::DataOneof::kFailedToCleanupRosWarning: {
    const logs::FailedToCleanupRosWarning& data{log_data.failed_to_cleanup_ros_warning()};
    StrAppendToSink(sink, "Failed to clean up ROS nodes and processes: ", data.details());
    break;
  }
  case LogData::DataOneof::kRpcDisconnectedWarning: {
    const logs::RpcDisconnectedWarning& data{log_data.rpc_disconnected_warning()};
    StrAppendToSink(sink, "RPC request ", data.request_name(), " to ", data.endpoint(), " failed since endpoint is disconnected.");
    break;
  }
  case LogData::DataOneof::kRpcUnhandledError: {
    const logs::RpcUnhandledError& data{log_data.rpc_unhandled_error()};
    StrAppendToSink(sink, "RPC request handler for ", data.request_name(), " failed due to an unhandled internal error: ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kTimeDiffOutOfRangeWarning: {
    const logs::TimeDiffOutOfRangeWarning& data{log_data.time_diff_out_of_range_warning()};
    StrAppendToSink(sink, "Time difference is out of range: ", data.diff(), ". Previous timestamp: ", data.prev_time(), ", Current timestamp: ", data.curr_time(), ".");
    break;
  }
  case LogData::DataOneof::kTensorrtLog: {
    const logs::TensorrtLog& data{log_data.tensorrt_log()};
    StrAppendToSink(sink, "[TensorRT] ", data.log());
    break;
  }
  case LogData::DataOneof::kBuildingTensorrtEngineInfo: {
    StrAppendToSink(sink, "Building TensorRT engine since the model has changed or the engine is not available. This may take a while.");
    break;
  }
  case LogData::DataOneof::kLoadingTensorrtEngineInfo: {
    StrAppendToSink(sink, "Loading TensorRT engine. This may take a while.");
    break;
  }
  case LogData::DataOneof::kCalibrationMapNotFound: {
    const logs::CalibrationMapNotFound& data{log_data.calibration_map_not_found()};
    StrAppendToSink(sink, "The calibration map ", data.path(), " was not found.");
    break;
  }
  case LogData::DataOneof::kCalibrationMapNotValid: {
    const logs::CalibrationMapNotValid& data{log_data.calibration_map_not_valid()};
    StrAppendToSink(sink, "The calibration map ", data.path(), " is not valid.");
    break;
  }
  case LogData::DataOneof::kCalibrationMapPathAlreadyExists: {
    const logs::CalibrationMapPathAlreadyExists& data{log_data.calibration_map_path_already_exists()};
    StrAppendToSink(sink, "The calibration map path ", data.path(), " already exists.");
    break;
  }
  case LogData::DataOneof::kFailedToSaveCalibrationMap: {
    const logs::FailedToSaveCalibrationMap& data{log_data.failed_to_save_calibration_map()};
    StrAppendToSink(sink, "Failed to save the calibration map to ", data.path(), ".");
    break;
  }
  case LogData::DataOneof::kFailedToRemoveCalibrationMap: {
    const logs::FailedToRemoveCalibrationMap& data{log_data.failed_to_remove_calibration_map()};
    StrAppendToSink(sink, "Failed to remove the calibration map at ", data.path(), ".");
    break;
  }
  case LogData::DataOneof::kFailedToIterateInDirectory: {
    const logs::FailedToIterateInDirectory& data{log_data.failed_to_iterate_in_directory()};
    StrAppendToSink(sink, "Failed to iterate in the ", data.directory_name(), " directory ", data.directory_path(), ": ", data.details(), ".");
    break;
  }
  case LogData::DataOneof::kMapBasedCalibrationWithoutMapLoading: {
    StrAppendToSink(sink, "The calibration map must be running before triggering the map-based calibration.");
    break;
  }
  case LogData::DataOneof::kMapBasedCalibrationAlreadyRunning: {
    StrAppendToSink(sink, "The map-based calibration is already running.");
    break;
  }
  case LogData::DataOneof::kCancelMapBasedCalibrationNotRunning: {
    StrAppendToSink(sink, "A cancel map-based calibration request has been received while the calibration is not running.");
    break;
  }
  case LogData::DataOneof::kBagStreamNotFound: {
    const logs::BagStreamNotFound& data{log_data.bag_stream_not_found()};
    StrAppendToSink(sink, "The stream ", data.stream_id(), " was not found from ", data.bag_path(), ".");
    break;
  }
  case LogData::DataOneof::kEvaluationBagStartedInfo: {
    const logs::EvaluationBagStartedInfo& data{log_data.evaluation_bag_started_info()};
    StrAppendToSink(sink, "Evaluation bag started for ", data.bag_path());
    break;
  }
  case LogData::DataOneof::kEvaluationBagFinishedInfo: {
    const logs::EvaluationBagFinishedInfo& data{log_data.evaluation_bag_finished_info()};
    StrAppendToSink(sink, "Evaluation bag finished for ", data.bag_path());
    break;
  }
  case LogData::DataOneof::kBagNotFound: {
    const logs::BagNotFound& data{log_data.bag_not_found()};
    StrAppendToSink(sink, "The bag ", data.bag_path(), " was not found.");
    break;
  }
  case LogData::DataOneof::kBuildingPipelineInfo: {
    const logs::BuildingPipelineInfo& data{log_data.building_pipeline_info()};
    StrAppendToSink(sink, "Building the ", data.pipeline_name(), " pipeline.");
    break;
  }
  case LogData::DataOneof::kBagIsNotEvaluation: {
    const logs::BagIsNotEvaluation& data{log_data.bag_is_not_evaluation()};
    StrAppendToSink(sink, "The bag ", data.bag_path(), " is not an evaluation bag.");
    break;
  }
  case LogData::DataOneof::kHorusBagRunning: {
    StrAppendToSink(sink, "A Horus bag is currently running.");
    break;
  }
  case LogData::DataOneof::kAutoGroundCalibrationWarning: {
    const logs::AutoGroundCalibrationWarning& data{log_data.auto_ground_calibration_warning()};
    StrAppendToSink(sink, "Automatic ground calibration failed for the following lidars: ", data.failed_lidar_ids(), ".");
    break;
  }
  case LogData::DataOneof::kAutoGroundCalibrationError: {
    StrAppendToSink(sink, "Automatic ground calibration failed for all lidars.");
    break;
  }
  case LogData::DataOneof::kObjectDetectorNotLoadedWarning: {
    StrAppendToSink(sink, "The object detector is not loaded yet. Temporarily using rule based detection which may result in degraded detection results.");
    break;
  }
  case LogData::DataOneof::kCalibrationIsRunningError: {
    StrAppendToSink(sink, "A calibration process is already running.");
    break;
  }
  case LogData::DataOneof::kModelInitInProgressInfo: {
    const logs::ModelInitInProgressInfo& data{log_data.model_init_in_progress_info()};
    StrAppendToSink(sink, data.component(), " will start processing data once all models have finished initializing.");
    break;
  }
  case LogData::DataOneof::kRpcTimeoutWithResolutionWarning: {
    const logs::RpcTimeoutWithResolutionWarning& data{log_data.rpc_timeout_with_resolution_warning()};
    StrAppendToSink(sink, "RPC request \"", data.request_name(), "\" to \"", data.endpoint(), "\" timed out after ", data.elapsed(), ". Resolution: ", data.resolution());
    break;
  }
  case LogData::DataOneof::kCalibrationWasCancelledInfo: {
    const logs::CalibrationWasCancelledInfo& data{log_data.calibration_was_cancelled_info()};
    StrAppendToSink(sink, "The ", data.calibration_process_name(), " process was cancelled.");
    break;
  }
  case LogData::DataOneof::kCalibrationMapRecordingFailedToStart: {
    const logs::CalibrationMapRecordingFailedToStart& data{log_data.calibration_map_recording_failed_to_start()};
    StrAppendToSink(sink, "Failed to start recording the calibration map: ", data.path());
    break;
  }
  case LogData::DataOneof::kDetectionPipelineRequestedResetInfo: {
    const logs::DetectionPipelineRequestedResetInfo& data{log_data.detection_pipeline_requested_reset_info()};
    StrAppendToSink(sink, "The detection pipeline has been requested to be reset because ", data.reason(), ".");
    break;
  }
  case LogData::DataOneof::kPreprocessingServicePipelineUnavailable: {
    const logs::PreprocessingServicePipelineUnavailable& data{log_data.preprocessing_service_pipeline_unavailable()};
    StrAppendToSink(sink, "Preprocessing service pipeline is not available yet due to ", data.reason(), ".");
    break;
  }
  case LogData::DataOneof::kCircularRecordingDisabledWarning: {
    const logs::CircularRecordingDisabledWarning& data{log_data.circular_recording_disabled_warning()};
    StrAppendToSink(sink, "The circular recording is disabled: ", data.reason());
    break;
  }
  case LogData::DataOneof::kSnapshotAlreadyRunningWarning: {
    StrAppendToSink(sink, "A snapshot is already running.");
    break;
  }
  case LogData::DataOneof::kNotSet:
  default: {
    sink.Append("Unknown log message");
    break;
  }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
