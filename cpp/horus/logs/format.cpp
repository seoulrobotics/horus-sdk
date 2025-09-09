#include "horus/logs/format.h"

#include <chrono>
#include <cstdint>

#include "horus/pb/logs/logs_pb.h"
#include "horus/pb/logs/message_pb.h"
#include "horus/pb/logs/metadata_pb.h"
#include "horus/strings/chrono.h"
#include "horus/strings/stringify.h"
#include "horus/strings/erased_sink.h"

namespace horus {
namespace sdk {
namespace pb {

void HorusStringify(const ErasedSink& sink, const LogMetadata_Duration& duration) {
  std::chrono::milliseconds const milliseconds{static_cast<std::int64_t>(duration.ms())};
  HorusStringify(sink, AnyDuration{milliseconds});
}

void HorusStringify(const ErasedSink& sink, const LogMetadata_SourceLocation& source_location) {
  StringifyTo(sink, source_location.file(), ":", source_location.line());
}

void HorusStringify(const ErasedSink& sink, const LogMetadata_Timestamp& timestamp) {
  std::chrono::milliseconds const ms_since_epoch{static_cast<std::int64_t>(timestamp.ms_since_epoch())};
  std::chrono::system_clock::time_point const time_point{ms_since_epoch};
  HorusStringify(sink, Iso8601{time_point});
}

void HorusStringify(const ErasedSink& sink, const LogData& log_data) {
  switch (log_data.data_case()) {
  case LogData::DataOneof::kGeneric: {
    logs::HorusStringify(sink, log_data.generic());
    break;
  }
  case LogData::DataOneof::kOom: {
    logs::HorusStringify(sink, log_data.oom());
    break;
  }
  case LogData::DataOneof::kRpcConnectionError: {
    logs::HorusStringify(sink, log_data.rpc_connection_error());
    break;
  }
  case LogData::DataOneof::kInvalidProjectName: {
    logs::HorusStringify(sink, log_data.invalid_project_name());
    break;
  }
  case LogData::DataOneof::kProjectNotFound: {
    logs::HorusStringify(sink, log_data.project_not_found());
    break;
  }
  case LogData::DataOneof::kProjectAlreadyExists: {
    logs::HorusStringify(sink, log_data.project_already_exists());
    break;
  }
  case LogData::DataOneof::kInvalidConfiguration: {
    logs::HorusStringify(sink, log_data.invalid_configuration());
    break;
  }
  case LogData::DataOneof::kEntityNotFound: {
    logs::HorusStringify(sink, log_data.entity_not_found());
    break;
  }
  case LogData::DataOneof::kActiveProjectCannotBeDeleted: {
    logs::HorusStringify(sink, log_data.active_project_cannot_be_deleted());
    break;
  }
  case LogData::DataOneof::kRpcDisconnectionError: {
    logs::HorusStringify(sink, log_data.rpc_disconnection_error());
    break;
  }
  case LogData::DataOneof::kDroppedLogs: {
    logs::HorusStringify(sink, log_data.dropped_logs());
    break;
  }
  case LogData::DataOneof::kOpenedProject: {
    logs::HorusStringify(sink, log_data.opened_project());
    break;
  }
  case LogData::DataOneof::kCreatedProject: {
    logs::HorusStringify(sink, log_data.created_project());
    break;
  }
  case LogData::DataOneof::kConfigUnavailable: {
    logs::HorusStringify(sink, log_data.config_unavailable());
    break;
  }
  case LogData::DataOneof::kInvalidRequest: {
    logs::HorusStringify(sink, log_data.invalid_request());
    break;
  }
  case LogData::DataOneof::kSanityCheckError: {
    logs::HorusStringify(sink, log_data.sanity_check_error());
    break;
  }
  case LogData::DataOneof::kBagFailedToOpen: {
    logs::HorusStringify(sink, log_data.bag_failed_to_open());
    break;
  }
  case LogData::DataOneof::kBagFailedToClose: {
    logs::HorusStringify(sink, log_data.bag_failed_to_close());
    break;
  }
  case LogData::DataOneof::kBagConversionFailed: {
    logs::HorusStringify(sink, log_data.bag_conversion_failed());
    break;
  }
  case LogData::DataOneof::kBagFailedToWrite: {
    logs::HorusStringify(sink, log_data.bag_failed_to_write());
    break;
  }
  case LogData::DataOneof::kCalibrationError: {
    logs::HorusStringify(sink, log_data.calibration_error());
    break;
  }
  case LogData::DataOneof::kProjectManagerFailedToStartRecording: {
    logs::HorusStringify(sink, log_data.project_manager_failed_to_start_recording());
    break;
  }
  case LogData::DataOneof::kProjectManagerFailedToStopRecording: {
    logs::HorusStringify(sink, log_data.project_manager_failed_to_stop_recording());
    break;
  }
  case LogData::DataOneof::kServiceConnectionTimedOut: {
    logs::HorusStringify(sink, log_data.service_connection_timed_out());
    break;
  }
  case LogData::DataOneof::kBagRecorderAlreadyRunning: {
    logs::HorusStringify(sink, log_data.bag_recorder_already_running());
    break;
  }
  case LogData::DataOneof::kLicenseServerConnectionError: {
    logs::HorusStringify(sink, log_data.license_server_connection_error());
    break;
  }
  case LogData::DataOneof::kLicenseError: {
    logs::HorusStringify(sink, log_data.license_error());
    break;
  }
  case LogData::DataOneof::kLicenseNotFoundError: {
    logs::HorusStringify(sink, log_data.license_not_found_error());
    break;
  }
  case LogData::DataOneof::kLicenseExpiredError: {
    logs::HorusStringify(sink, log_data.license_expired_error());
    break;
  }
  case LogData::DataOneof::kLicenseExceededError: {
    logs::HorusStringify(sink, log_data.license_exceeded_error());
    break;
  }
  case LogData::DataOneof::kLicenseHostMachineError: {
    logs::HorusStringify(sink, log_data.license_host_machine_error());
    break;
  }
  case LogData::DataOneof::kLicensePrivilegeError: {
    logs::HorusStringify(sink, log_data.license_privilege_error());
    break;
  }
  case LogData::DataOneof::kLicenseActiveInfo: {
    logs::HorusStringify(sink, log_data.license_active_info());
    break;
  }
  case LogData::DataOneof::kMultipleLicensesWarning: {
    logs::HorusStringify(sink, log_data.multiple_licenses_warning());
    break;
  }
  case LogData::DataOneof::kLicenseCurrentLicenseStatusInfo: {
    logs::HorusStringify(sink, log_data.license_current_license_status_info());
    break;
  }
  case LogData::DataOneof::kBagRecordingStoppedInfo: {
    logs::HorusStringify(sink, log_data.bag_recording_stopped_info());
    break;
  }
  case LogData::DataOneof::kBagRecordingFailedToStart: {
    logs::HorusStringify(sink, log_data.bag_recording_failed_to_start());
    break;
  }
  case LogData::DataOneof::kBagRecordingStartedInfo: {
    logs::HorusStringify(sink, log_data.bag_recording_started_info());
    break;
  }
  case LogData::DataOneof::kReplayRestartedInfo: {
    logs::HorusStringify(sink, log_data.replay_restarted_info());
    break;
  }
  case LogData::DataOneof::kInputSourceChangeRequestedInfo: {
    logs::HorusStringify(sink, log_data.input_source_change_requested_info());
    break;
  }
  case LogData::DataOneof::kInputSourceSwitchedInfo: {
    logs::HorusStringify(sink, log_data.input_source_switched_info());
    break;
  }
  case LogData::DataOneof::kRpcTimeoutWarning: {
    logs::HorusStringify(sink, log_data.rpc_timeout_warning());
    break;
  }
  case LogData::DataOneof::kCannotWriteLogFile: {
    logs::HorusStringify(sink, log_data.cannot_write_log_file());
    break;
  }
  case LogData::DataOneof::kPointCloudParsingFailureWarning: {
    logs::HorusStringify(sink, log_data.point_cloud_parsing_failure_warning());
    break;
  }
  case LogData::DataOneof::kLidarIsDead: {
    logs::HorusStringify(sink, log_data.lidar_is_dead());
    break;
  }
  case LogData::DataOneof::kLidarIsNotDeadAnymore: {
    logs::HorusStringify(sink, log_data.lidar_is_not_dead_anymore());
    break;
  }
  case LogData::DataOneof::kLidarIsObstructed: {
    logs::HorusStringify(sink, log_data.lidar_is_obstructed());
    break;
  }
  case LogData::DataOneof::kLidarIsNotObstructedAnymore: {
    logs::HorusStringify(sink, log_data.lidar_is_not_obstructed_anymore());
    break;
  }
  case LogData::DataOneof::kLidarIsTilted: {
    logs::HorusStringify(sink, log_data.lidar_is_tilted());
    break;
  }
  case LogData::DataOneof::kLidarIsNotTiltedAnymore: {
    logs::HorusStringify(sink, log_data.lidar_is_not_tilted_anymore());
    break;
  }
  case LogData::DataOneof::kLidarHasBeenAutomaticallyRecalibrated: {
    logs::HorusStringify(sink, log_data.lidar_has_been_automatically_recalibrated());
    break;
  }
  case LogData::DataOneof::kReceivedFirstDataForLidar: {
    logs::HorusStringify(sink, log_data.received_first_data_for_lidar());
    break;
  }
  case LogData::DataOneof::kTerminationFailureError: {
    logs::HorusStringify(sink, log_data.termination_failure_error());
    break;
  }
  case LogData::DataOneof::kFrameProcessingError: {
    logs::HorusStringify(sink, log_data.frame_processing_error());
    break;
  }
  case LogData::DataOneof::kThreadPoolUnavailableError: {
    logs::HorusStringify(sink, log_data.thread_pool_unavailable_error());
    break;
  }
  case LogData::DataOneof::kInvalidArgument: {
    logs::HorusStringify(sink, log_data.invalid_argument());
    break;
  }
  case LogData::DataOneof::kComponentInitializationFailureFatal: {
    logs::HorusStringify(sink, log_data.component_initialization_failure_fatal());
    break;
  }
  case LogData::DataOneof::kUnhandledEnumCaseError: {
    logs::HorusStringify(sink, log_data.unhandled_enum_case_error());
    break;
  }
  case LogData::DataOneof::kBagEmptyError: {
    logs::HorusStringify(sink, log_data.bag_empty_error());
    break;
  }
  case LogData::DataOneof::kDiscardingDataError: {
    logs::HorusStringify(sink, log_data.discarding_data_error());
    break;
  }
  case LogData::DataOneof::kDiscardingDataWarning: {
    logs::HorusStringify(sink, log_data.discarding_data_warning());
    break;
  }
  case LogData::DataOneof::kNothingToProcess: {
    logs::HorusStringify(sink, log_data.nothing_to_process());
    break;
  }
  case LogData::DataOneof::kInvalidComponentConfiguration: {
    logs::HorusStringify(sink, log_data.invalid_component_configuration());
    break;
  }
  case LogData::DataOneof::kChannelReceiverNotFoundWarning: {
    logs::HorusStringify(sink, log_data.channel_receiver_not_found_warning());
    break;
  }
  case LogData::DataOneof::kModelLoadFailure: {
    logs::HorusStringify(sink, log_data.model_load_failure());
    break;
  }
  case LogData::DataOneof::kModelExecutionFailureError: {
    logs::HorusStringify(sink, log_data.model_execution_failure_error());
    break;
  }
  case LogData::DataOneof::kServiceReadyInfo: {
    logs::HorusStringify(sink, log_data.service_ready_info());
    break;
  }
  case LogData::DataOneof::kModelPreparingInfo: {
    logs::HorusStringify(sink, log_data.model_preparing_info());
    break;
  }
  case LogData::DataOneof::kModelInitializedInfo: {
    logs::HorusStringify(sink, log_data.model_initialized_info());
    break;
  }
  case LogData::DataOneof::kModelInitializationFailureWarning: {
    logs::HorusStringify(sink, log_data.model_initialization_failure_warning());
    break;
  }
  case LogData::DataOneof::kRosSpinnerStoppedWarning: {
    logs::HorusStringify(sink, log_data.ros_spinner_stopped_warning());
    break;
  }
  case LogData::DataOneof::kActorSystemUnavailableError: {
    logs::HorusStringify(sink, log_data.actor_system_unavailable_error());
    break;
  }
  case LogData::DataOneof::kConfigNodeNotFoundError: {
    logs::HorusStringify(sink, log_data.config_node_not_found_error());
    break;
  }
  case LogData::DataOneof::kBagTimestampOutOfOrderError: {
    logs::HorusStringify(sink, log_data.bag_timestamp_out_of_order_error());
    break;
  }
  case LogData::DataOneof::kBagReplayUnexpectedTimestampError: {
    logs::HorusStringify(sink, log_data.bag_replay_unexpected_timestamp_error());
    break;
  }
  case LogData::DataOneof::kWebsocketClosedInfo: {
    logs::HorusStringify(sink, log_data.websocket_closed_info());
    break;
  }
  case LogData::DataOneof::kWebsocketOpenedInfo: {
    logs::HorusStringify(sink, log_data.websocket_opened_info());
    break;
  }
  case LogData::DataOneof::kSubscriberDisconnectedInfo: {
    logs::HorusStringify(sink, log_data.subscriber_disconnected_info());
    break;
  }
  case LogData::DataOneof::kThreadPoolSlowingDownWarning: {
    logs::HorusStringify(sink, log_data.thread_pool_slowing_down_warning());
    break;
  }
  case LogData::DataOneof::kThreadPoolNotRespondingWarning: {
    logs::HorusStringify(sink, log_data.thread_pool_not_responding_warning());
    break;
  }
  case LogData::DataOneof::kThreadPoolBrokenPromiseWarning: {
    logs::HorusStringify(sink, log_data.thread_pool_broken_promise_warning());
    break;
  }
  case LogData::DataOneof::kBoxFittingPointsBelowBaseWarning: {
    logs::HorusStringify(sink, log_data.box_fitting_points_below_base_warning());
    break;
  }
  case LogData::DataOneof::kFailedToRemoveStalePointsWarning: {
    logs::HorusStringify(sink, log_data.failed_to_remove_stale_points_warning());
    break;
  }
  case LogData::DataOneof::kIrregularBroadcastingPeriodWarning: {
    logs::HorusStringify(sink, log_data.irregular_broadcasting_period_warning());
    break;
  }
  case LogData::DataOneof::kClustererPointsOutOfRangeWarning: {
    logs::HorusStringify(sink, log_data.clusterer_points_out_of_range_warning());
    break;
  }
  case LogData::DataOneof::kInternalError: {
    logs::HorusStringify(sink, log_data.internal_error());
    break;
  }
  case LogData::DataOneof::kInternalFatal: {
    logs::HorusStringify(sink, log_data.internal_fatal());
    break;
  }
  case LogData::DataOneof::kServiceStartingInfo: {
    logs::HorusStringify(sink, log_data.service_starting_info());
    break;
  }
  case LogData::DataOneof::kConfigNodeNotFoundFatal: {
    logs::HorusStringify(sink, log_data.config_node_not_found_fatal());
    break;
  }
  case LogData::DataOneof::kServiceSetupError: {
    logs::HorusStringify(sink, log_data.service_setup_error());
    break;
  }
  case LogData::DataOneof::kFilesystemError: {
    logs::HorusStringify(sink, log_data.filesystem_error());
    break;
  }
  case LogData::DataOneof::kInvalidPresetWarning: {
    logs::HorusStringify(sink, log_data.invalid_preset_warning());
    break;
  }
  case LogData::DataOneof::kWebsocketFailedClearPendingError: {
    logs::HorusStringify(sink, log_data.websocket_failed_clear_pending_error());
    break;
  }
  case LogData::DataOneof::kWebsocketFailedToStopError: {
    logs::HorusStringify(sink, log_data.websocket_failed_to_stop_error());
    break;
  }
  case LogData::DataOneof::kWebsocketFailedToCreateError: {
    logs::HorusStringify(sink, log_data.websocket_failed_to_create_error());
    break;
  }
  case LogData::DataOneof::kUnexpectedRpcError: {
    logs::HorusStringify(sink, log_data.unexpected_rpc_error());
    break;
  }
  case LogData::DataOneof::kLicensePollFailed: {
    logs::HorusStringify(sink, log_data.license_poll_failed());
    break;
  }
  case LogData::DataOneof::kLicenseExpiredWarning: {
    logs::HorusStringify(sink, log_data.license_expired_warning());
    break;
  }
  case LogData::DataOneof::kLicenseUsageExceededWarning: {
    logs::HorusStringify(sink, log_data.license_usage_exceeded_warning());
    break;
  }
  case LogData::DataOneof::kStaticThreadPoolSlowTaskWarning: {
    logs::HorusStringify(sink, log_data.static_thread_pool_slow_task_warning());
    break;
  }
  case LogData::DataOneof::kRpcUnsupportedServiceWarning: {
    logs::HorusStringify(sink, log_data.rpc_unsupported_service_warning());
    break;
  }
  case LogData::DataOneof::kWebsocketHandlerProblem: {
    logs::HorusStringify(sink, log_data.websocket_handler_problem());
    break;
  }
  case LogData::DataOneof::kWebsocketDeserializeError: {
    logs::HorusStringify(sink, log_data.websocket_deserialize_error());
    break;
  }
  case LogData::DataOneof::kWebsocketExpiredRpcEndpointError: {
    logs::HorusStringify(sink, log_data.websocket_expired_rpc_endpoint_error());
    break;
  }
  case LogData::DataOneof::kWebsocketQueueOverloadedWarning: {
    logs::HorusStringify(sink, log_data.websocket_queue_overloaded_warning());
    break;
  }
  case LogData::DataOneof::kRpcFailedToNotifyWarning: {
    logs::HorusStringify(sink, log_data.rpc_failed_to_notify_warning());
    break;
  }
  case LogData::DataOneof::kConfigSubscriptionFailedWarning: {
    logs::HorusStringify(sink, log_data.config_subscription_failed_warning());
    break;
  }
  case LogData::DataOneof::kThreadPoolClampedWorkersWarning: {
    logs::HorusStringify(sink, log_data.thread_pool_clamped_workers_warning());
    break;
  }
  case LogData::DataOneof::kStoppingHorusBagRecorderAlreadyStopped: {
    logs::HorusStringify(sink, log_data.stopping_horus_bag_recorder_already_stopped());
    break;
  }
  case LogData::DataOneof::kRecorderConfigUpdateWhileRunning: {
    logs::HorusStringify(sink, log_data.recorder_config_update_while_running());
    break;
  }
  case LogData::DataOneof::kClampingDataWarning: {
    logs::HorusStringify(sink, log_data.clamping_data_warning());
    break;
  }
  case LogData::DataOneof::kLidarIncompatibleValues: {
    logs::HorusStringify(sink, log_data.lidar_incompatible_values());
    break;
  }
  case LogData::DataOneof::kCannotDetermineContainerIdError: {
    logs::HorusStringify(sink, log_data.cannot_determine_container_id_error());
    break;
  }
  case LogData::DataOneof::kStartedLidarDriver: {
    logs::HorusStringify(sink, log_data.started_lidar_driver());
    break;
  }
  case LogData::DataOneof::kCannotStartLidarDriver: {
    logs::HorusStringify(sink, log_data.cannot_start_lidar_driver());
    break;
  }
  case LogData::DataOneof::kStoppedLidarDriver: {
    logs::HorusStringify(sink, log_data.stopped_lidar_driver());
    break;
  }
  case LogData::DataOneof::kCannotStopLidarDriver: {
    logs::HorusStringify(sink, log_data.cannot_stop_lidar_driver());
    break;
  }
  case LogData::DataOneof::kRestartedLidarDriver: {
    logs::HorusStringify(sink, log_data.restarted_lidar_driver());
    break;
  }
  case LogData::DataOneof::kCannotRestartLidarDriver: {
    logs::HorusStringify(sink, log_data.cannot_restart_lidar_driver());
    break;
  }
  case LogData::DataOneof::kRemovedUnusedLidarDriver: {
    logs::HorusStringify(sink, log_data.removed_unused_lidar_driver());
    break;
  }
  case LogData::DataOneof::kCannotRemoveUnusedLidarDriver: {
    logs::HorusStringify(sink, log_data.cannot_remove_unused_lidar_driver());
    break;
  }
  case LogData::DataOneof::kLidarDriverGcFailure: {
    logs::HorusStringify(sink, log_data.lidar_driver_gc_failure());
    break;
  }
  case LogData::DataOneof::kIdSpaceExhausted: {
    logs::HorusStringify(sink, log_data.id_space_exhausted());
    break;
  }
  case LogData::DataOneof::kPreprocessingToPointAggregatorPointsSkipped: {
    logs::HorusStringify(sink, log_data.preprocessing_to_point_aggregator_points_skipped());
    break;
  }
  case LogData::DataOneof::kMinMsgIntervalLessThanThreshold: {
    logs::HorusStringify(sink, log_data.min_msg_interval_less_than_threshold());
    break;
  }
  case LogData::DataOneof::kFailedToCleanupRosWarning: {
    logs::HorusStringify(sink, log_data.failed_to_cleanup_ros_warning());
    break;
  }
  case LogData::DataOneof::kRpcDisconnectedWarning: {
    logs::HorusStringify(sink, log_data.rpc_disconnected_warning());
    break;
  }
  case LogData::DataOneof::kRpcUnhandledError: {
    logs::HorusStringify(sink, log_data.rpc_unhandled_error());
    break;
  }
  case LogData::DataOneof::kTimeDiffOutOfRangeWarning: {
    logs::HorusStringify(sink, log_data.time_diff_out_of_range_warning());
    break;
  }
  case LogData::DataOneof::kTensorrtLog: {
    logs::HorusStringify(sink, log_data.tensorrt_log());
    break;
  }
  case LogData::DataOneof::kBuildingTensorrtEngineInfo: {
    logs::HorusStringify(sink, log_data.building_tensorrt_engine_info());
    break;
  }
  case LogData::DataOneof::kLoadingTensorrtEngineInfo: {
    logs::HorusStringify(sink, log_data.loading_tensorrt_engine_info());
    break;
  }
  case LogData::DataOneof::kCalibrationMapNotFound: {
    logs::HorusStringify(sink, log_data.calibration_map_not_found());
    break;
  }
  case LogData::DataOneof::kCalibrationMapNotValid: {
    logs::HorusStringify(sink, log_data.calibration_map_not_valid());
    break;
  }
  case LogData::DataOneof::kCalibrationMapPathAlreadyExists: {
    logs::HorusStringify(sink, log_data.calibration_map_path_already_exists());
    break;
  }
  case LogData::DataOneof::kFailedToSaveCalibrationMap: {
    logs::HorusStringify(sink, log_data.failed_to_save_calibration_map());
    break;
  }
  case LogData::DataOneof::kFailedToRemoveCalibrationMap: {
    logs::HorusStringify(sink, log_data.failed_to_remove_calibration_map());
    break;
  }
  case LogData::DataOneof::kFailedToIterateInDirectory: {
    logs::HorusStringify(sink, log_data.failed_to_iterate_in_directory());
    break;
  }
  case LogData::DataOneof::kMapBasedCalibrationWithoutMapLoading: {
    logs::HorusStringify(sink, log_data.map_based_calibration_without_map_loading());
    break;
  }
  case LogData::DataOneof::kMapBasedCalibrationAlreadyRunning: {
    logs::HorusStringify(sink, log_data.map_based_calibration_already_running());
    break;
  }
  case LogData::DataOneof::kCancelMapBasedCalibrationNotRunning: {
    logs::HorusStringify(sink, log_data.cancel_map_based_calibration_not_running());
    break;
  }
  case LogData::DataOneof::kBagStreamNotFound: {
    logs::HorusStringify(sink, log_data.bag_stream_not_found());
    break;
  }
  case LogData::DataOneof::kEvaluationBagStartedInfo: {
    logs::HorusStringify(sink, log_data.evaluation_bag_started_info());
    break;
  }
  case LogData::DataOneof::kEvaluationBagFinishedInfo: {
    logs::HorusStringify(sink, log_data.evaluation_bag_finished_info());
    break;
  }
  case LogData::DataOneof::kBagNotFound: {
    logs::HorusStringify(sink, log_data.bag_not_found());
    break;
  }
  case LogData::DataOneof::kBuildingPipelineInfo: {
    logs::HorusStringify(sink, log_data.building_pipeline_info());
    break;
  }
  case LogData::DataOneof::kBagIsNotEvaluation: {
    logs::HorusStringify(sink, log_data.bag_is_not_evaluation());
    break;
  }
  case LogData::DataOneof::kHorusBagRunning: {
    logs::HorusStringify(sink, log_data.horus_bag_running());
    break;
  }
  case LogData::DataOneof::kAutoGroundCalibrationWarning: {
    logs::HorusStringify(sink, log_data.auto_ground_calibration_warning());
    break;
  }
  case LogData::DataOneof::kAutoGroundCalibrationError: {
    logs::HorusStringify(sink, log_data.auto_ground_calibration_error());
    break;
  }
  case LogData::DataOneof::kObjectDetectorNotLoadedWarning: {
    logs::HorusStringify(sink, log_data.object_detector_not_loaded_warning());
    break;
  }
  case LogData::DataOneof::kCalibrationIsRunningError: {
    logs::HorusStringify(sink, log_data.calibration_is_running_error());
    break;
  }
  case LogData::DataOneof::kModelInitInProgressInfo: {
    logs::HorusStringify(sink, log_data.model_init_in_progress_info());
    break;
  }
  case LogData::DataOneof::kRpcTimeoutWithResolutionWarning: {
    logs::HorusStringify(sink, log_data.rpc_timeout_with_resolution_warning());
    break;
  }
  case LogData::DataOneof::kCalibrationWasCancelledInfo: {
    logs::HorusStringify(sink, log_data.calibration_was_cancelled_info());
    break;
  }
  case LogData::DataOneof::kCalibrationMapRecordingFailedToStart: {
    logs::HorusStringify(sink, log_data.calibration_map_recording_failed_to_start());
    break;
  }
  case LogData::DataOneof::kDetectionPipelineRequestedResetInfo: {
    logs::HorusStringify(sink, log_data.detection_pipeline_requested_reset_info());
    break;
  }
  case LogData::DataOneof::kPreprocessingServicePipelineUnavailable: {
    logs::HorusStringify(sink, log_data.preprocessing_service_pipeline_unavailable());
    break;
  }
  case LogData::DataOneof::kCircularRecordingDisabledWarning: {
    logs::HorusStringify(sink, log_data.circular_recording_disabled_warning());
    break;
  }
  case LogData::DataOneof::kSnapshotAlreadyRunningWarning: {
    logs::HorusStringify(sink, log_data.snapshot_already_running_warning());
    break;
  }
  case LogData::DataOneof::kActiveProjectChangedInfo: {
    logs::HorusStringify(sink, log_data.active_project_changed_info());
    break;
  }
  case LogData::DataOneof::kProjectConfigUpdatedInfo: {
    logs::HorusStringify(sink, log_data.project_config_updated_info());
    break;
  }
  case LogData::DataOneof::kInvalidLidarTimestamp: {
    logs::HorusStringify(sink, log_data.invalid_lidar_timestamp());
    break;
  }
  case LogData::DataOneof::kCalibrationAccumulatingPointsInfo: {
    logs::HorusStringify(sink, log_data.calibration_accumulating_points_info());
    break;
  }
  case LogData::DataOneof::kSparseNoiseFilterUsageNonRotationalLidars: {
    logs::HorusStringify(sink, log_data.sparse_noise_filter_usage_non_rotational_lidars());
    break;
  }
  case LogData::DataOneof::kFileWriteWarning: {
    logs::HorusStringify(sink, log_data.file_write_warning());
    break;
  }
  case LogData::DataOneof::kLicenseForbiddenFeature: {
    logs::HorusStringify(sink, log_data.license_forbidden_feature());
    break;
  }
  case LogData::DataOneof::kFailedToUpdateConfiguration: {
    logs::HorusStringify(sink, log_data.failed_to_update_configuration());
    break;
  }
  case LogData::DataOneof::kObstructionDetectorBadReferenceWarning: {
    logs::HorusStringify(sink, log_data.obstruction_detector_bad_reference_warning());
    break;
  }
  case LogData::DataOneof::kProjectFileInvalidPermissionsError: {
    logs::HorusStringify(sink, log_data.project_file_invalid_permissions_error());
    break;
  }
  case LogData::DataOneof::kPipelineSchedulerError: {
    logs::HorusStringify(sink, log_data.pipeline_scheduler_error());
    break;
  }
  case LogData::DataOneof::kMultiLidarCalibrationWarning: {
    logs::HorusStringify(sink, log_data.multi_lidar_calibration_warning());
    break;
  }
  case LogData::DataOneof::kMultiLidarCalibrationError: {
    logs::HorusStringify(sink, log_data.multi_lidar_calibration_error());
    break;
  }
  case LogData::DataOneof::kOldBagWarning: {
    logs::HorusStringify(sink, log_data.old_bag_warning());
    break;
  }
  case LogData::DataOneof::kUpgradingBagInfo: {
    logs::HorusStringify(sink, log_data.upgrading_bag_info());
    break;
  }
  case LogData::DataOneof::kBagCalibrationSaveFailed: {
    logs::HorusStringify(sink, log_data.bag_calibration_save_failed());
    break;
  }
  case LogData::DataOneof::kBagUpgradeFailed: {
    logs::HorusStringify(sink, log_data.bag_upgrade_failed());
    break;
  }
  case LogData::DataOneof::kUnknownLidarError: {
    logs::HorusStringify(sink, log_data.unknown_lidar_error());
    break;
  }
  case LogData::DataOneof::kInvalidPointCloudWarning: {
    logs::HorusStringify(sink, log_data.invalid_point_cloud_warning());
    break;
  }
  case LogData::DataOneof::kLidarIsDroppingPackets: {
    logs::HorusStringify(sink, log_data.lidar_is_dropping_packets());
    break;
  }
  case LogData::DataOneof::kRemovedInvalidLidarsFromConfigWarning: {
    logs::HorusStringify(sink, log_data.removed_invalid_lidars_from_config_warning());
    break;
  }
  case LogData::DataOneof::kCalibrationMapRecordingFailed: {
    logs::HorusStringify(sink, log_data.calibration_map_recording_failed());
    break;
  }
  case LogData::DataOneof::kDetectionNodeNotFoundError: {
    logs::HorusStringify(sink, log_data.detection_node_not_found_error());
    break;
  }
  case LogData::DataOneof::kCreatedVersionBackupInfo: {
    logs::HorusStringify(sink, log_data.created_version_backup_info());
    break;
  }
  case LogData::DataOneof::kPlyFileLoadFailedError: {
    logs::HorusStringify(sink, log_data.ply_file_load_failed_error());
    break;
  }
  case LogData::DataOneof::kHesaiDriverLifecycle: {
    logs::HorusStringify(sink, log_data.hesai_driver_lifecycle());
    break;
  }
  case LogData::DataOneof::kHesaiDriverError: {
    logs::HorusStringify(sink, log_data.hesai_driver_error());
    break;
  }
  case LogData::DataOneof::kHesaiPacketProcessingFailed: {
    logs::HorusStringify(sink, log_data.hesai_packet_processing_failed());
    break;
  }
  case LogData::DataOneof::kHesaiCorrectionFileError: {
    logs::HorusStringify(sink, log_data.hesai_correction_file_error());
    break;
  }
  case LogData::DataOneof::kHesaiPacketStatistics: {
    logs::HorusStringify(sink, log_data.hesai_packet_statistics());
    break;
  }
  case LogData::DataOneof::kPlyFileWriteFailedError: {
    logs::HorusStringify(sink, log_data.ply_file_write_failed_error());
    break;
  }
  case LogData::DataOneof::kProjectSaveError: {
    logs::HorusStringify(sink, log_data.project_save_error());
    break;
  }
  case LogData::DataOneof::kSaveStaticEnvironmentSuccess: {
    logs::HorusStringify(sink, log_data.save_static_environment_success());
    break;
  }
  case LogData::DataOneof::kSaveStaticEnvironmentFailed: {
    logs::HorusStringify(sink, log_data.save_static_environment_failed());
    break;
  }
  case LogData::DataOneof::kLoadStaticEnvironmentSuccess: {
    logs::HorusStringify(sink, log_data.load_static_environment_success());
    break;
  }
  case LogData::DataOneof::kLoadStaticEnvironmentFailed: {
    logs::HorusStringify(sink, log_data.load_static_environment_failed());
    break;
  }
  case LogData::DataOneof::kAttemptToInjectInvalidLidarIdWarning: {
    logs::HorusStringify(sink, log_data.attempt_to_inject_invalid_lidar_id_warning());
    break;
  }
  case LogData::DataOneof::kResetBundledPacketDueToUnexpectedPacket: {
    logs::HorusStringify(sink, log_data.reset_bundled_packet_due_to_unexpected_packet());
    break;
  }
  case LogData::DataOneof::kPacketBundlerDroppedPacketsWarning: {
    logs::HorusStringify(sink, log_data.packet_bundler_dropped_packets_warning());
    break;
  }
  case LogData::DataOneof::kPacketBundlerFrameJumpWarning: {
    logs::HorusStringify(sink, log_data.packet_bundler_frame_jump_warning());
    break;
  }
  case LogData::DataOneof::kLidarCorrectionLoadingSuccess: {
    logs::HorusStringify(sink, log_data.lidar_correction_loading_success());
    break;
  }
  case LogData::DataOneof::kLidarCorrectionLoadingFailure: {
    logs::HorusStringify(sink, log_data.lidar_correction_loading_failure());
    break;
  }
  case LogData::DataOneof::kHesaiPacketStatisticsLidar: {
    logs::HorusStringify(sink, log_data.hesai_packet_statistics_lidar());
    break;
  }
  case LogData::DataOneof::kNotSet:
  default: {
    sink.Append("Unknown log message");
    break;
  }
  }
}

namespace logs {

void HorusStringify(const ErasedSink& sink, const Generic& data) {
  StringifyTo(sink, data.message());
}

void HorusStringify(const ErasedSink& sink, const Oom& /*data*/) {
  StringifyTo(sink, "Out of memory");
}

void HorusStringify(const ErasedSink& sink, const RpcConnectionError& data) {
  StringifyTo(sink, "Cannot initiate connection to ", data.target_service(), " at ", data.target_uri(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const InvalidProjectName& data) {
  StringifyTo(sink, "Project name `", data.name(), "` is invalid: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ProjectNotFound& data) {
  StringifyTo(sink, "Project `", data.name(), "` not found");
}

void HorusStringify(const ErasedSink& sink, const ProjectAlreadyExists& data) {
  StringifyTo(sink, "Project `", data.name(), "` already exists");
}

void HorusStringify(const ErasedSink& sink, const InvalidConfiguration& /*data*/) {
  StringifyTo(sink, "Configuration is invalid");
}

void HorusStringify(const ErasedSink& sink, const EntityNotFound& data) {
  StringifyTo(sink, data.entity_type(), " `", data.id(), "` not found");
}

void HorusStringify(const ErasedSink& sink, const ActiveProjectCannotBeDeleted& /*data*/) {
  StringifyTo(sink, "Active project cannot be deleted");
}

void HorusStringify(const ErasedSink& sink, const RpcDisconnectionError& data) {
  StringifyTo(sink, "Cannot terminate connection to ", data.target_service(), " at ", data.target_uri(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const DroppedLogs& data) {
  StringifyTo(sink, "Dropped ", data.n(), " log messages");
}

void HorusStringify(const ErasedSink& sink, const OpenedProject& data) {
  StringifyTo(sink, "Opened project ", data.project_name());
}

void HorusStringify(const ErasedSink& sink, const CreatedProject& data) {
  StringifyTo(sink, "Created project ", data.project_name(), " from ", data.source_name());
}

void HorusStringify(const ErasedSink& sink, const ConfigUnavailable& /*data*/) {
  StringifyTo(sink, "Configuration is unavailable; request cannot be processed.");
}

void HorusStringify(const ErasedSink& sink, const InvalidRequest& data) {
  StringifyTo(sink, "Invalid request received: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const SanityCheckError& data) {
  StringifyTo(sink, data.sanity_check_name(), " failed: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagFailedToOpen& data) {
  StringifyTo(sink, "The bag file (", data.horus_bag_path(), ") could not be opened: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagFailedToClose& data) {
  StringifyTo(sink, "Bag file could not be closed: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagConversionFailed& data) {
  StringifyTo(sink, "Bag file conversion failed: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagFailedToWrite& data) {
  StringifyTo(sink, "Bag file writing failed ", data.name(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const CalibrationError& data) {
  StringifyTo(sink, "Calibration failed: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ProjectManagerFailedToStartRecording& data) {
  StringifyTo(sink, "Failed to start recording: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ProjectManagerFailedToStopRecording& data) {
  StringifyTo(sink, "Failed to stop recording: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ServiceConnectionTimedOut& data) {
  StringifyTo(sink, "The connection with the requested ", data.node_type(), " node with ID ", data.node_id(), " timed out");
}

void HorusStringify(const ErasedSink& sink, const BagRecorderAlreadyRunning& data) {
  StringifyTo(sink, "Bag recorder is already running for ", data.path());
}

void HorusStringify(const ErasedSink& sink, const LicenseServerConnectionError& data) {
  StringifyTo(sink, "Could not connect to license server: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicenseError& data) {
  StringifyTo(sink, "Could not read license: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicenseNotFoundError& /*data*/) {
  StringifyTo(sink, "Found no valid license.");
}

void HorusStringify(const ErasedSink& sink, const LicenseExpiredError& data) {
  StringifyTo(sink, "License expired on ", data.expiration_time(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicenseExceededError& data) {
  StringifyTo(sink, "Project exceeds number of LiDARs allowed by license (", data.lidar_count(), " > ", data.max_lidar_count(), ").");
}

void HorusStringify(const ErasedSink& sink, const LicenseHostMachineError& data) {
  StringifyTo(sink, "Could not validate license with current hardware - ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicensePrivilegeError& data) {
  StringifyTo(sink, "Endpoint requires \"", data.missing_privilege(), "\" privilege: ", data.level_error(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicenseActiveInfo& /*data*/) {
  StringifyTo(sink, "License is active.");
}

void HorusStringify(const ErasedSink& sink, const MultipleLicensesWarning& data) {
  StringifyTo(sink, "Found ", data.non_expired_licenses_count(), " non-expired concurrent licenses. Only \"", data.active_license_filename(), "\" is used.");
}

void HorusStringify(const ErasedSink& sink, const LicenseCurrentLicenseStatusInfo& data) {
  StringifyTo(sink, "Current license accepts up to ", data.max_lidar_count(), " and expires on ", data.expiration_time(), ".");
}

void HorusStringify(const ErasedSink& sink, const BagRecordingStoppedInfo& data) {
  StringifyTo(sink, "Bag recording stopped for ", data.path(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagRecordingFailedToStart& data) {
  StringifyTo(sink, "Bag recording failed to start: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagRecordingStartedInfo& data) {
  StringifyTo(sink, "Bag recording started for ", data.path());
}

void HorusStringify(const ErasedSink& sink, const ReplayRestartedInfo& /*data*/) {
  StringifyTo(sink, "Replay Restarted");
}

void HorusStringify(const ErasedSink& sink, const InputSourceChangeRequestedInfo& data) {
  StringifyTo(sink, "Input source ", data.source(), " requested");
}

void HorusStringify(const ErasedSink& sink, const InputSourceSwitchedInfo& data) {
  StringifyTo(sink, "Input source changed to ", data.source());
}

void HorusStringify(const ErasedSink& sink, const RpcTimeoutWarning& data) {
  StringifyTo(sink, "RPC request ", data.request_name(), " to ", data.endpoint(), " timed out after ", data.elapsed(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotWriteLogFile& data) {
  StringifyTo(sink, "Notification service cannot write log in ", data.path(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const PointCloudParsingFailureWarning& data) {
  StringifyTo(sink, "Point cloud parsing failed: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const LidarIsDead& data) {
  StringifyTo(sink, "The lidar ", data.lidar_id(), " is considered dead. No data has been received for a while.");
}

void HorusStringify(const ErasedSink& sink, const LidarIsNotDeadAnymore& data) {
  StringifyTo(sink, "The lidar ", data.lidar_id(), " is not considered dead anymore. A point cloud has been received again after some time.");
}

void HorusStringify(const ErasedSink& sink, const LidarIsObstructed& data) {
  StringifyTo(sink, "The lidar ", data.lidar_id(), " is obstructed.");
}

void HorusStringify(const ErasedSink& sink, const LidarIsNotObstructedAnymore& data) {
  StringifyTo(sink, "The lidar ", data.lidar_id(), " is not obstructed anymore.");
}

void HorusStringify(const ErasedSink& sink, const LidarIsTilted& data) {
  StringifyTo(sink, "The lidar ", data.lidar_id(), " is tilted.");
}

void HorusStringify(const ErasedSink& sink, const LidarIsNotTiltedAnymore& data) {
  StringifyTo(sink, "The lidar ", data.lidar_id(), " is not tilted anymore.");
}

void HorusStringify(const ErasedSink& sink, const LidarHasBeenAutomaticallyRecalibrated& data) {
  StringifyTo(sink, "The lidar ", data.lidar_id(), " has been automatically re-calibrated.");
}

void HorusStringify(const ErasedSink& sink, const ReceivedFirstDataForLidar& data) {
  StringifyTo(sink, "Data has been received for the first time for the lidar ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const TerminationFailureError& data) {
  StringifyTo(sink, "Failed to destruct ", data.component(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const FrameProcessingError& data) {
  StringifyTo(sink, "The Frame Task Scheduler failed to process frame task: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ThreadPoolUnavailableError& data) {
  StringifyTo(sink, "Thread pool is not available: ", data.consequence());
}

void HorusStringify(const ErasedSink& sink, const InvalidArgument& data) {
  StringifyTo(sink, "Invalid argument: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ComponentInitializationFailureFatal& data) {
  StringifyTo(sink, "Failed to initialize ", data.component(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const UnhandledEnumCaseError& data) {
  StringifyTo(sink, "Unhandled enum case ", data.case_name(), " at ", data.location());
}

void HorusStringify(const ErasedSink& sink, const BagEmptyError& /*data*/) {
  StringifyTo(sink, "The replay bag file is empty.");
}

void HorusStringify(const ErasedSink& sink, const DiscardingDataError& data) {
  StringifyTo(sink, "Discarding data in ", data.component(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const DiscardingDataWarning& data) {
  StringifyTo(sink, "Discarding data in ", data.component(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const NothingToProcess& data) {
  StringifyTo(sink, "Nothing to process: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const InvalidComponentConfiguration& data) {
  StringifyTo(sink, "Invalid configuration for ", data.component(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ChannelReceiverNotFoundWarning& data) {
  StringifyTo(sink, "Channel receiver ", data.key(), " not found");
}

void HorusStringify(const ErasedSink& sink, const ModelLoadFailure& data) {
  StringifyTo(sink, "Model ", data.model(), " failed to load: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ModelExecutionFailureError& data) {
  StringifyTo(sink, "Model ", data.model(), " failed to execute: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ServiceReadyInfo& data) {
  StringifyTo(sink, data.service(), " service is ready");
}

void HorusStringify(const ErasedSink& sink, const ModelPreparingInfo& data) {
  StringifyTo(sink, "Preparing model ", data.model(), ", it may take several seconds to stabilize.");
}

void HorusStringify(const ErasedSink& sink, const ModelInitializedInfo& data) {
  StringifyTo(sink, data.name(), " model has been initialized and is ready for use.");
}

void HorusStringify(const ErasedSink& sink, const ModelInitializationFailureWarning& data) {
  StringifyTo(sink, "Failed to initialize model ", data.model(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const RosSpinnerStoppedWarning& data) {
  StringifyTo(sink, "ROS spinner stopped: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ActorSystemUnavailableError& /*data*/) {
  StringifyTo(sink, "Actor system is not available. Please reinstantiate the pipeline.");
}

void HorusStringify(const ErasedSink& sink, const ConfigNodeNotFoundError& data) {
  StringifyTo(sink, "Configuration for node ", data.node(), " is unavailable");
}

void HorusStringify(const ErasedSink& sink, const BagTimestampOutOfOrderError& data) {
  StringifyTo(sink, "Timestamps are out of order: ", data.prior_timestamp(), " > ", data.current_timestamp());
}

void HorusStringify(const ErasedSink& sink, const BagReplayUnexpectedTimestampError& data) {
  StringifyTo(sink, "Unexpected timestamp: expected ", data.expected_timestamp(), " but received ", data.received_timestamp());
}

void HorusStringify(const ErasedSink& sink, const WebsocketClosedInfo& data) {
  StringifyTo(sink, "WebSocket RPC connection to ", data.uri(), " closed: ", data.reason());
}

void HorusStringify(const ErasedSink& sink, const WebsocketOpenedInfo& data) {
  StringifyTo(sink, "WebSocket RPC connection to ", data.uri(), " opened (as ", data.endpoint_is_server_client(), ")");
}

void HorusStringify(const ErasedSink& sink, const SubscriberDisconnectedInfo& data) {
  StringifyTo(sink, "Subscriber ", data.name(), " disconnected (", data.uri(), ")");
}

void HorusStringify(const ErasedSink& sink, const ThreadPoolSlowingDownWarning& data) {
  StringifyTo(sink, "Thread pool is slowing down for ", data.number(), " times!");
}

void HorusStringify(const ErasedSink& sink, const ThreadPoolNotRespondingWarning& data) {
  StringifyTo(sink, "Thread pool is not responding for ", data.not_responding_for(), ".");
}

void HorusStringify(const ErasedSink& sink, const ThreadPoolBrokenPromiseWarning& /*data*/) {
  StringifyTo(sink, "Thread pool task future is no longer available. The thread pool might have been restarted during events such as input source change.");
}

void HorusStringify(const ErasedSink& sink, const BoxFittingPointsBelowBaseWarning& /*data*/) {
  StringifyTo(sink, "Some points are below the base Z-coordinate when fitting the box. This might affect the detection results.");
}

void HorusStringify(const ErasedSink& sink, const FailedToRemoveStalePointsWarning& data) {
  StringifyTo(sink, "Failed to remove stale points: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const IrregularBroadcastingPeriodWarning& data) {
  StringifyTo(sink, data.num_irregular(), " irregular broadcast(s) detected in the last ", data.duration(), " (mean deviation: ", data.mean_deviation(), ").");
}

void HorusStringify(const ErasedSink& sink, const ClustererPointsOutOfRangeWarning& data) {
  StringifyTo(sink, "There are ", data.num_points(), " points out of range in the clusterer. Some points: ", data.sample_points(), ".");
}

void HorusStringify(const ErasedSink& sink, const InternalError& data) {
  StringifyTo(sink, "An internal error occurred: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const InternalFatal& data) {
  StringifyTo(sink, "An internal fatal error occurred: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ServiceStartingInfo& data) {
  StringifyTo(sink, data.service(), " is starting");
}

void HorusStringify(const ErasedSink& sink, const ConfigNodeNotFoundFatal& data) {
  StringifyTo(sink, "Configuration for node ", data.node(), " is unavailable");
}

void HorusStringify(const ErasedSink& sink, const ServiceSetupError& data) {
  StringifyTo(sink, "Failed to setup: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const FilesystemError& data) {
  StringifyTo(sink, "Filesystem error: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const InvalidPresetWarning& data) {
  StringifyTo(sink, "Tried to apply invalid preset ", data.preset(), " due to ", data.reason(), ", falling back to ", data.fallback_name(), ".");
}

void HorusStringify(const ErasedSink& sink, const WebsocketFailedClearPendingError& data) {
  StringifyTo(sink, "WebSocket failed to clear pending queues: ", data.exception());
}

void HorusStringify(const ErasedSink& sink, const WebsocketFailedToStopError& data) {
  StringifyTo(sink, "WebSocket failed to stop: ", data.exception());
}

void HorusStringify(const ErasedSink& sink, const WebsocketFailedToCreateError& data) {
  StringifyTo(sink, "Failed to create RPC server ws://", data.host(), ":", data.port(), "/: ", data.exception());
}

void HorusStringify(const ErasedSink& sink, const UnexpectedRpcError& data) {
  StringifyTo(sink, "Unexpected RPC error: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const LicensePollFailed& data) {
  StringifyTo(sink, "Failed to poll license server: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const LicenseExpiredWarning& data) {
  StringifyTo(sink, "License expired on ", data.expiration_time(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicenseUsageExceededWarning& data) {
  StringifyTo(sink, "License count of ", data.lidar_count(), " exceeds allowed usage of ", data.max_lidar_count());
}

void HorusStringify(const ErasedSink& sink, const StaticThreadPoolSlowTaskWarning& data) {
  StringifyTo(sink, "ThreadPool task took ", data.time_elapsed(), " to complete task enqueued in ", data.enqueue_location());
}

void HorusStringify(const ErasedSink& sink, const RpcUnsupportedServiceWarning& data) {
  StringifyTo(sink, "Received request for unsupported service: ", data.service_id());
}

void HorusStringify(const ErasedSink& sink, const WebsocketHandlerProblem& data) {
  StringifyTo(sink, "WebSocket RPC handler encountered an error: ", data.what());
}

void HorusStringify(const ErasedSink& sink, const WebsocketDeserializeError& data) {
  StringifyTo(sink, "WebSocket RPC received message cannot be deserialized: ", data.what());
}

void HorusStringify(const ErasedSink& sink, const WebsocketExpiredRpcEndpointError& data) {
  StringifyTo(sink, "Attempting to access expired WebSocket RPC endpoint ", data.uri());
}

void HorusStringify(const ErasedSink& sink, const WebsocketQueueOverloadedWarning& data) {
  StringifyTo(sink, "Websocket message queue overloaded: ", data.current(), " / ", data.max());
}

void HorusStringify(const ErasedSink& sink, const RpcFailedToNotifyWarning& data) {
  StringifyTo(sink, "Could not notify ", data.notification_name(), " to ", data.service(), " ", data.uri(), ": ", data.what());
}

void HorusStringify(const ErasedSink& sink, const ConfigSubscriptionFailedWarning& data) {
  StringifyTo(sink, "Failed to subscribe to config: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ThreadPoolClampedWorkersWarning& data) {
  StringifyTo(sink, "Clamped thread pool workers to ", data.clamped(), " instead of ", data.requested());
}

void HorusStringify(const ErasedSink& sink, const StoppingHorusBagRecorderAlreadyStopped& /*data*/) {
  StringifyTo(sink, "Trying to stop the Horus bag recorder which is already stopped.");
}

void HorusStringify(const ErasedSink& sink, const RecorderConfigUpdateWhileRunning& /*data*/) {
  StringifyTo(sink, "An update for the Horus bag recorder has been received while it was running. This update will not be taken into account until the next recording.");
}

void HorusStringify(const ErasedSink& sink, const ClampingDataWarning& data) {
  StringifyTo(sink, "Clamping point cloud: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const LidarIncompatibleValues& data) {
  StringifyTo(sink, "Lidars ", data.lidar_ip1(), " and ", data.lidar_ip2(), " have incompatible ", data.value_names(), ": ", data.value1(), " vs. ", data.value2(), "; ", data.resolution(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotDetermineContainerIdError& data) {
  StringifyTo(sink, "Cannot determine current Docker container ID; unknown ", data.container_id_file_path(), " format.");
}

void HorusStringify(const ErasedSink& sink, const StartedLidarDriver& data) {
  StringifyTo(sink, "Started lidar driver container ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotStartLidarDriver& data) {
  StringifyTo(sink, "Cannot start lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const StoppedLidarDriver& data) {
  StringifyTo(sink, "Stopped lidar driver container ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotStopLidarDriver& data) {
  StringifyTo(sink, "Cannot stop lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const RestartedLidarDriver& data) {
  StringifyTo(sink, "Restarted lidar driver container ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotRestartLidarDriver& data) {
  StringifyTo(sink, "Cannot restart lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const RemovedUnusedLidarDriver& data) {
  StringifyTo(sink, "Removed unused lidar driver container ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotRemoveUnusedLidarDriver& data) {
  StringifyTo(sink, "Cannot remove unused lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const LidarDriverGcFailure& data) {
  StringifyTo(sink, "Error encountered while removing unused lidar driver containers: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const IdSpaceExhausted& data) {
  StringifyTo(sink, "Cannot allocate new identifier with prefix \"", data.prefix(), "\".");
}

void HorusStringify(const ErasedSink& sink, const PreprocessingToPointAggregatorPointsSkipped& data) {
  StringifyTo(sink, "The point cloud publishing to the point aggregator service has been skipped ", data.num_skipped_points(), " time(s) in the last ", data.check_interval(), ".");
}

void HorusStringify(const ErasedSink& sink, const MinMsgIntervalLessThanThreshold& data) {
  StringifyTo(sink, "Discarding lidar points from ", data.lidar_id(), " since the time interval between two point-cloud messages is too close (<", data.threshold(), "). Adjust the Min-Message Interval parameter to change this behavior.");
}

void HorusStringify(const ErasedSink& sink, const FailedToCleanupRosWarning& data) {
  StringifyTo(sink, "Failed to clean up ROS nodes and processes: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const RpcDisconnectedWarning& data) {
  StringifyTo(sink, "RPC request ", data.request_name(), " to ", data.endpoint(), " failed since endpoint is disconnected.");
}

void HorusStringify(const ErasedSink& sink, const RpcUnhandledError& data) {
  StringifyTo(sink, "RPC request handler for ", data.request_name(), " failed due to an unhandled internal error: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const TimeDiffOutOfRangeWarning& data) {
  StringifyTo(sink, "Time difference is out of range: ", data.diff(), ". Previous timestamp: ", data.prev_time(), ", Current timestamp: ", data.curr_time(), ".");
}

void HorusStringify(const ErasedSink& sink, const TensorrtLog& data) {
  StringifyTo(sink, "[TensorRT] ", data.log());
}

void HorusStringify(const ErasedSink& sink, const BuildingTensorrtEngineInfo& /*data*/) {
  StringifyTo(sink, "Building TensorRT engine since the model has changed or the engine is not available. This may take a while.");
}

void HorusStringify(const ErasedSink& sink, const LoadingTensorrtEngineInfo& /*data*/) {
  StringifyTo(sink, "Loading TensorRT engine. This may take a while.");
}

void HorusStringify(const ErasedSink& sink, const CalibrationMapNotFound& data) {
  StringifyTo(sink, "The calibration map ", data.path(), " was not found.");
}

void HorusStringify(const ErasedSink& sink, const CalibrationMapNotValid& data) {
  StringifyTo(sink, "The calibration map ", data.path(), " is not valid.");
}

void HorusStringify(const ErasedSink& sink, const CalibrationMapPathAlreadyExists& data) {
  StringifyTo(sink, "The calibration map path ", data.path(), " already exists.");
}

void HorusStringify(const ErasedSink& sink, const FailedToSaveCalibrationMap& data) {
  StringifyTo(sink, "Failed to save the calibration map to ", data.path(), ".");
}

void HorusStringify(const ErasedSink& sink, const FailedToRemoveCalibrationMap& data) {
  StringifyTo(sink, "Failed to remove the calibration map at ", data.path(), ".");
}

void HorusStringify(const ErasedSink& sink, const FailedToIterateInDirectory& data) {
  StringifyTo(sink, "Failed to iterate in the ", data.directory_name(), " directory ", data.directory_path(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const MapBasedCalibrationWithoutMapLoading& /*data*/) {
  StringifyTo(sink, "The calibration map must be running before triggering the map-based calibration.");
}

void HorusStringify(const ErasedSink& sink, const MapBasedCalibrationAlreadyRunning& /*data*/) {
  StringifyTo(sink, "The map-based calibration is already running.");
}

void HorusStringify(const ErasedSink& sink, const CancelMapBasedCalibrationNotRunning& /*data*/) {
  StringifyTo(sink, "A cancel map-based calibration request has been received while the calibration is not running.");
}

void HorusStringify(const ErasedSink& sink, const BagStreamNotFound& data) {
  StringifyTo(sink, "The stream ", data.stream_id(), " was not found from ", data.bag_path(), ".");
}

void HorusStringify(const ErasedSink& sink, const EvaluationBagStartedInfo& data) {
  StringifyTo(sink, "Evaluation bag started for ", data.bag_path());
}

void HorusStringify(const ErasedSink& sink, const EvaluationBagFinishedInfo& data) {
  StringifyTo(sink, "Evaluation bag finished for ", data.bag_path());
}

void HorusStringify(const ErasedSink& sink, const BagNotFound& data) {
  StringifyTo(sink, "The bag ", data.bag_path(), " was not found.");
}

void HorusStringify(const ErasedSink& sink, const BuildingPipelineInfo& data) {
  StringifyTo(sink, "Building the ", data.pipeline_name(), " pipeline.");
}

void HorusStringify(const ErasedSink& sink, const BagIsNotEvaluation& data) {
  StringifyTo(sink, "The bag ", data.bag_path(), " is not an evaluation bag.");
}

void HorusStringify(const ErasedSink& sink, const HorusBagRunning& /*data*/) {
  StringifyTo(sink, "A Horus bag is currently running.");
}

void HorusStringify(const ErasedSink& sink, const AutoGroundCalibrationWarning& data) {
  StringifyTo(sink, "Automatic ground calibration failed for the following lidars: ", data.failed_lidar_ids(), ".");
}

void HorusStringify(const ErasedSink& sink, const AutoGroundCalibrationError& /*data*/) {
  StringifyTo(sink, "Automatic ground calibration failed for all lidars.");
}

void HorusStringify(const ErasedSink& sink, const ObjectDetectorNotLoadedWarning& /*data*/) {
  StringifyTo(sink, "The object detector is not loaded yet. Temporarily using rule based detection which may result in degraded detection results.");
}

void HorusStringify(const ErasedSink& sink, const CalibrationIsRunningError& /*data*/) {
  StringifyTo(sink, "A calibration process is already running.");
}

void HorusStringify(const ErasedSink& sink, const ModelInitInProgressInfo& data) {
  StringifyTo(sink, data.component(), " will start processing data once all models have finished initializing.");
}

void HorusStringify(const ErasedSink& sink, const RpcTimeoutWithResolutionWarning& data) {
  StringifyTo(sink, "RPC request \"", data.request_name(), "\" to \"", data.endpoint(), "\" timed out after ", data.elapsed(), ". Resolution: ", data.resolution());
}

void HorusStringify(const ErasedSink& sink, const CalibrationWasCancelledInfo& data) {
  StringifyTo(sink, "The ", data.calibration_process_name(), " process was cancelled.");
}

void HorusStringify(const ErasedSink& sink, const CalibrationMapRecordingFailedToStart& data) {
  StringifyTo(sink, "Failed to start recording the calibration map: ", data.path());
}

void HorusStringify(const ErasedSink& sink, const DetectionPipelineRequestedResetInfo& data) {
  StringifyTo(sink, "The detection pipeline has been requested to be reset because ", data.reason(), ".");
}

void HorusStringify(const ErasedSink& sink, const PreprocessingServicePipelineUnavailable& data) {
  StringifyTo(sink, "Preprocessing service pipeline is not available yet due to ", data.reason(), ".");
}

void HorusStringify(const ErasedSink& sink, const CircularRecordingDisabledWarning& data) {
  StringifyTo(sink, "The timeframe snapshot is disabled: ", data.reason());
}

void HorusStringify(const ErasedSink& sink, const SnapshotAlreadyRunningWarning& /*data*/) {
  StringifyTo(sink, "A snapshot is already running.");
}

void HorusStringify(const ErasedSink& sink, const ActiveProjectChangedInfo& /*data*/) {
  StringifyTo(sink, "The active project has been changed.");
}

void HorusStringify(const ErasedSink& sink, const ProjectConfigUpdatedInfo& /*data*/) {
  StringifyTo(sink, "The project configuration has been updated.");
}

void HorusStringify(const ErasedSink& sink, const InvalidLidarTimestamp& data) {
  StringifyTo(sink, "Invalid timestamp ", data.timestamp(), " sent by lidar ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const CalibrationAccumulatingPointsInfo& data) {
  StringifyTo(sink, "Calibration is accumulating points for ", data.time());
}

void HorusStringify(const ErasedSink& sink, const SparseNoiseFilterUsageNonRotationalLidars& /*data*/) {
  StringifyTo(sink, "The sparse noise filter cannot be used with non-rotational lidars.");
}

void HorusStringify(const ErasedSink& sink, const FileWriteWarning& data) {
  StringifyTo(sink, "Failed to write to file \"", data.filename(), "\": \"", data.details(), "\".");
}

void HorusStringify(const ErasedSink& sink, const LicenseForbiddenFeature& data) {
  StringifyTo(sink, data.feature_name(), " is not allowed by the current license.");
}

void HorusStringify(const ErasedSink& sink, const FailedToUpdateConfiguration& data) {
  StringifyTo(sink, "Failed to update the configuration: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const ObstructionDetectorBadReferenceWarning& /*data*/) {
  StringifyTo(sink, "The obstruction detector reference is not valid since it contains zero points.");
}

void HorusStringify(const ErasedSink& sink, const ProjectFileInvalidPermissionsError& data) {
  StringifyTo(sink, "Project file \"", data.filename(), "\" has invalid permissions. Please restart Horus to fix the issue.");
}

void HorusStringify(const ErasedSink& sink, const PipelineSchedulerError& data) {
  StringifyTo(sink, "Pipeline scheduler error: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const MultiLidarCalibrationWarning& data) {
  StringifyTo(sink, "Multi lidar calibration failed for the following lidars: ", data.failed_lidar_ids(), ".");
}

void HorusStringify(const ErasedSink& sink, const MultiLidarCalibrationError& /*data*/) {
  StringifyTo(sink, "Multi lidar calibration failed for all lidars.");
}

void HorusStringify(const ErasedSink& sink, const OldBagWarning& data) {
  StringifyTo(sink, "The bag file ", data.bag_path(), " with version ", data.version(), " is old. Please consider updating it with the API, refer to API documentation.");
}

void HorusStringify(const ErasedSink& sink, const UpgradingBagInfo& data) {
  StringifyTo(sink, "Upgrading bag file ", data.bag_path(), " to latest version.");
}

void HorusStringify(const ErasedSink& sink, const BagCalibrationSaveFailed& data) {
  StringifyTo(sink, "Failed to save the bag calibration: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const BagUpgradeFailed& data) {
  StringifyTo(sink, "Failed to upgrade the bag file ", data.bag_path(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const UnknownLidarError& data) {
  StringifyTo(sink, "Received points from unknown lidar ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const InvalidPointCloudWarning& data) {
  StringifyTo(sink, "Invalid point cloud received from lidar ", data.lidar_id(), ": ", data.reason());
}

void HorusStringify(const ErasedSink& sink, const LidarIsDroppingPackets& data) {
  StringifyTo(sink, "The lidar ", data.lidar_id(), " is dropping packets (dropped ", data.num_total_dropped_packets(), " packets out of ", data.num_total_expected_packets(), ").");
}

void HorusStringify(const ErasedSink& sink, const RemovedInvalidLidarsFromConfigWarning& data) {
  StringifyTo(sink, "Some deprecated lidars have been found in the loaded project. These invalid lidars were removed from the active project but a backup config file has been saved in ", data.backup_path(), ".");
}

void HorusStringify(const ErasedSink& sink, const CalibrationMapRecordingFailed& data) {
  StringifyTo(sink, "Failed recording the calibration map: ", data.message());
}

void HorusStringify(const ErasedSink& sink, const DetectionNodeNotFoundError& data) {
  StringifyTo(sink, "Detection node ", data.node_name(), " not found.");
}

void HorusStringify(const ErasedSink& sink, const CreatedVersionBackupInfo& data) {
  StringifyTo(sink, "Horus upgraded from ", data.old_horus_version(), " to ", data.new_horus_version(), ". The project was backed up to ", data.backup_path(), " before being upgraded.");
}

void HorusStringify(const ErasedSink& sink, const PlyFileLoadFailedError& data) {
  StringifyTo(sink, "Failed to load PLY file ", data.file_path(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const HesaiDriverLifecycle& data) {
  StringifyTo(sink, "Hesai XT32 driver ", data.action(), " for LiDAR ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const HesaiDriverError& data) {
  StringifyTo(sink, "Hesai XT32 driver error: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const HesaiPacketProcessingFailed& data) {
  StringifyTo(sink, "Failed to process packet for Hesai LiDAR ", data.lidar_id(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const HesaiCorrectionFileError& data) {
  StringifyTo(sink, "Failed to load Hesai XT32 correction file ", data.file_type(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const HesaiPacketStatistics& data) {
  StringifyTo(sink, "Hesai packet statistics - Received: ", data.packets_received(), ", Published: ", data.packets_published(), ", Dropped: ", data.packets_dropped(), ", Decode Failed: ", data.packets_decode_failed(), ", Success Rate: ", data.success_rate(), " %");
}

void HorusStringify(const ErasedSink& sink, const PlyFileWriteFailedError& data) {
  StringifyTo(sink, "Failed to write PLY file ", data.file_path(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ProjectSaveError& data) {
  StringifyTo(sink, "Failed to save project: ", data.error_message(), ".");
}

void HorusStringify(const ErasedSink& sink, const SaveStaticEnvironmentSuccess& data) {
  StringifyTo(sink, "Saved static environment to ", data.path());
}

void HorusStringify(const ErasedSink& sink, const SaveStaticEnvironmentFailed& data) {
  StringifyTo(sink, "Failed to serialize static environment to ", data.path(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const LoadStaticEnvironmentSuccess& data) {
  StringifyTo(sink, "Static environment loaded from ", data.path());
}

void HorusStringify(const ErasedSink& sink, const LoadStaticEnvironmentFailed& data) {
  StringifyTo(sink, "Failed to load static environment from ", data.path(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const AttemptToInjectInvalidLidarIdWarning& data) {
  StringifyTo(sink, "Attempt to inject invalid lidar ID ", data.lidar_id(), " into the system.");
}

void HorusStringify(const ErasedSink& sink, const ResetBundledPacketDueToUnexpectedPacket& data) {
  StringifyTo(sink, "Resetting bundled packet due to unexpected packet from lidar ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const PacketBundlerDroppedPacketsWarning& data) {
  StringifyTo(sink, "Lidar ", data.lidar_id(), " dropped ", data.num_dropped(), " packets over ", data.duration());
}

void HorusStringify(const ErasedSink& sink, const PacketBundlerFrameJumpWarning& data) {
  StringifyTo(sink, "Frame sequence jump detected on lidar ", data.lidar_id(), ": from ", data.frame_id(), " to ", data.next_frame_id());
}

void HorusStringify(const ErasedSink& sink, const LidarCorrectionLoadingSuccess& data) {
  StringifyTo(sink, "Successfully loaded ", data.correction_type(), " corrections from the lidar");
}

void HorusStringify(const ErasedSink& sink, const LidarCorrectionLoadingFailure& data) {
  StringifyTo(sink, "Failed to load ", data.correction_type(), " corrections from the lidar (", data.details(), "); using default correction values");
}

void HorusStringify(const ErasedSink& sink, const HesaiPacketStatisticsLidar& data) {
  StringifyTo(sink, "[", data.lidar_id(), "] Hesai Packet Statistics - Received: ", data.packets_received(), ", Published: ", data.packets_published(), ", Dropped: ", data.packets_dropped(), ", Decode Failed: ", data.packets_decode_failed(), ", Success Rate: ", data.success_rate(), " %");
}

}  // namespace logs
}  // namespace pb
}  // namespace sdk
}  // namespace horus
