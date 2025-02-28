#include "horus/logs/format.h"

#include <chrono>
#include <cstdint>

#include "horus/pb/logs/logs_pb.h"
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
  case LogData::DataOneof::kFileWriteError: {
    logs::HorusStringify(sink, log_data.file_write_error());
    break;
  }
  case LogData::DataOneof::kLicenseForbiddenFeature: {
    logs::HorusStringify(sink, log_data.license_forbidden_feature());
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
  StrAppendToSink(sink, data.message());
}

void HorusStringify(const ErasedSink& sink, const Oom& /*data*/) {
  StrAppendToSink(sink, "Out of memory");
}

void HorusStringify(const ErasedSink& sink, const RpcConnectionError& data) {
  StrAppendToSink(sink, "Cannot initiate connection to ", data.target_service(), " at ", data.target_uri(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const InvalidProjectName& data) {
  StrAppendToSink(sink, "Project name `", data.name(), "` is invalid: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ProjectNotFound& data) {
  StrAppendToSink(sink, "Project `", data.name(), "` not found");
}

void HorusStringify(const ErasedSink& sink, const ProjectAlreadyExists& data) {
  StrAppendToSink(sink, "Project `", data.name(), "` already exists");
}

void HorusStringify(const ErasedSink& sink, const InvalidConfiguration& /*data*/) {
  StrAppendToSink(sink, "Configuration is invalid");
}

void HorusStringify(const ErasedSink& sink, const EntityNotFound& data) {
  StrAppendToSink(sink, data.entity_type(), " `", data.id(), "` not found");
}

void HorusStringify(const ErasedSink& sink, const ActiveProjectCannotBeDeleted& /*data*/) {
  StrAppendToSink(sink, "Active project cannot be deleted");
}

void HorusStringify(const ErasedSink& sink, const RpcDisconnectionError& data) {
  StrAppendToSink(sink, "Cannot terminate connection to ", data.target_service(), " at ", data.target_uri(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const DroppedLogs& data) {
  StrAppendToSink(sink, "Dropped ", data.n(), " log messages");
}

void HorusStringify(const ErasedSink& sink, const OpenedProject& data) {
  StrAppendToSink(sink, "Opened project ", data.project_name());
}

void HorusStringify(const ErasedSink& sink, const CreatedProject& data) {
  StrAppendToSink(sink, "Created project ", data.project_name(), " from ", data.source_name());
}

void HorusStringify(const ErasedSink& sink, const ConfigUnavailable& /*data*/) {
  StrAppendToSink(sink, "Configuration is unavailable; request cannot be processed.");
}

void HorusStringify(const ErasedSink& sink, const InvalidRequest& data) {
  StrAppendToSink(sink, "Invalid request received: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const SanityCheckError& data) {
  StrAppendToSink(sink, data.sanity_check_name(), " failed: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagFailedToOpen& data) {
  StrAppendToSink(sink, "The bag file (", data.horus_bag_path(), ") could not be opened: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagFailedToClose& data) {
  StrAppendToSink(sink, "Bag file could not be closed: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagConversionFailed& data) {
  StrAppendToSink(sink, "Bag file conversion failed: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagFailedToWrite& data) {
  StrAppendToSink(sink, "Bag file writing failed ", data.name(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const CalibrationError& data) {
  StrAppendToSink(sink, "Calibration failed: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ProjectManagerFailedToStartRecording& data) {
  StrAppendToSink(sink, "Failed to start recording: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ProjectManagerFailedToStopRecording& data) {
  StrAppendToSink(sink, "Failed to stop recording: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ServiceConnectionTimedOut& data) {
  StrAppendToSink(sink, "The connection with the requested ", data.node_type(), " node with ID ", data.node_id(), " timed out");
}

void HorusStringify(const ErasedSink& sink, const BagRecorderAlreadyRunning& data) {
  StrAppendToSink(sink, "Bag recorder is already running for ", data.path());
}

void HorusStringify(const ErasedSink& sink, const LicenseServerConnectionError& data) {
  StrAppendToSink(sink, "Could not connect to license server: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicenseError& data) {
  StrAppendToSink(sink, "Could not read license: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicenseNotFoundError& /*data*/) {
  StrAppendToSink(sink, "Found no valid license.");
}

void HorusStringify(const ErasedSink& sink, const LicenseExpiredError& data) {
  StrAppendToSink(sink, "License expired on ", data.expiration_time(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicenseExceededError& data) {
  StrAppendToSink(sink, "Project exceeds number of LiDARs allowed by license (", data.lidar_count(), " > ", data.max_lidar_count(), ").");
}

void HorusStringify(const ErasedSink& sink, const LicenseHostMachineError& data) {
  StrAppendToSink(sink, "Could not validate license with current hardware - ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicensePrivilegeError& data) {
  StrAppendToSink(sink, "Endpoint requires \"", data.missing_privilege(), "\" privilege: ", data.level_error(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicenseActiveInfo& /*data*/) {
  StrAppendToSink(sink, "License is active.");
}

void HorusStringify(const ErasedSink& sink, const MultipleLicensesWarning& data) {
  StrAppendToSink(sink, "Found ", data.non_expired_licenses_count(), " non-expired concurrent licenses. Only \"", data.active_license_filename(), "\" is used.");
}

void HorusStringify(const ErasedSink& sink, const LicenseCurrentLicenseStatusInfo& data) {
  StrAppendToSink(sink, "Current license accepts up to ", data.max_lidar_count(), " and expires on ", data.expiration_time(), ".");
}

void HorusStringify(const ErasedSink& sink, const BagRecordingStoppedInfo& data) {
  StrAppendToSink(sink, "Bag recording stopped for ", data.path(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagRecordingFailedToStart& data) {
  StrAppendToSink(sink, "Bag recording failed to start: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const BagRecordingStartedInfo& data) {
  StrAppendToSink(sink, "Bag recording started for ", data.path());
}

void HorusStringify(const ErasedSink& sink, const ReplayRestartedInfo& /*data*/) {
  StrAppendToSink(sink, "Replay Restarted");
}

void HorusStringify(const ErasedSink& sink, const InputSourceChangeRequestedInfo& data) {
  StrAppendToSink(sink, "Input source ", data.source(), " requested");
}

void HorusStringify(const ErasedSink& sink, const InputSourceSwitchedInfo& data) {
  StrAppendToSink(sink, "Input source changed to ", data.source());
}

void HorusStringify(const ErasedSink& sink, const RpcTimeoutWarning& data) {
  StrAppendToSink(sink, "RPC request ", data.request_name(), " to ", data.endpoint(), " timed out after ", data.elapsed(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotWriteLogFile& data) {
  StrAppendToSink(sink, "Notification service cannot write log in ", data.path(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const PointCloudParsingFailureWarning& data) {
  StrAppendToSink(sink, "Point cloud parsing failed: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const LidarIsDead& data) {
  StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is considered dead. No data has been received for a while.");
}

void HorusStringify(const ErasedSink& sink, const LidarIsNotDeadAnymore& data) {
  StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is not considered dead anymore. A point cloud has been received again after some time.");
}

void HorusStringify(const ErasedSink& sink, const LidarIsObstructed& data) {
  StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is obstructed.");
}

void HorusStringify(const ErasedSink& sink, const LidarIsNotObstructedAnymore& data) {
  StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is not obstructed anymore.");
}

void HorusStringify(const ErasedSink& sink, const LidarIsTilted& data) {
  StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is tilted.");
}

void HorusStringify(const ErasedSink& sink, const LidarIsNotTiltedAnymore& data) {
  StrAppendToSink(sink, "The lidar ", data.lidar_id(), " is not tilted anymore.");
}

void HorusStringify(const ErasedSink& sink, const LidarHasBeenAutomaticallyRecalibrated& data) {
  StrAppendToSink(sink, "The lidar ", data.lidar_id(), " has been automatically re-calibrated.");
}

void HorusStringify(const ErasedSink& sink, const ReceivedFirstDataForLidar& data) {
  StrAppendToSink(sink, "Data has been received for the first time for the lidar ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const TerminationFailureError& data) {
  StrAppendToSink(sink, "Failed to destruct ", data.component(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const FrameProcessingError& data) {
  StrAppendToSink(sink, "The Frame Task Scheduler failed to process frame task: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ThreadPoolUnavailableError& data) {
  StrAppendToSink(sink, "Thread pool is not available: ", data.consequence());
}

void HorusStringify(const ErasedSink& sink, const InvalidArgument& data) {
  StrAppendToSink(sink, "Invalid argument: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ComponentInitializationFailureFatal& data) {
  StrAppendToSink(sink, "Failed to initialize ", data.component(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const UnhandledEnumCaseError& data) {
  StrAppendToSink(sink, "Unhandled enum case ", data.case_name(), " at ", data.location());
}

void HorusStringify(const ErasedSink& sink, const BagEmptyError& /*data*/) {
  StrAppendToSink(sink, "The replay bag file is empty.");
}

void HorusStringify(const ErasedSink& sink, const DiscardingDataError& data) {
  StrAppendToSink(sink, "Discarding data in ", data.component(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const DiscardingDataWarning& data) {
  StrAppendToSink(sink, "Discarding data in ", data.component(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const NothingToProcess& data) {
  StrAppendToSink(sink, "Nothing to process: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const InvalidComponentConfiguration& data) {
  StrAppendToSink(sink, "Invalid configuration for ", data.component(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ChannelReceiverNotFoundWarning& data) {
  StrAppendToSink(sink, "Channel receiver ", data.key(), " not found");
}

void HorusStringify(const ErasedSink& sink, const ModelLoadFailure& data) {
  StrAppendToSink(sink, "Model ", data.model(), " failed to load: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ModelExecutionFailureError& data) {
  StrAppendToSink(sink, "Model ", data.model(), " failed to execute: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ServiceReadyInfo& data) {
  StrAppendToSink(sink, data.service(), " service is ready");
}

void HorusStringify(const ErasedSink& sink, const ModelPreparingInfo& data) {
  StrAppendToSink(sink, "Preparing model ", data.model(), ", it may take several seconds to stabilize.");
}

void HorusStringify(const ErasedSink& sink, const ModelInitializedInfo& data) {
  StrAppendToSink(sink, data.name(), " model has been initialized and is ready for use.");
}

void HorusStringify(const ErasedSink& sink, const ModelInitializationFailureWarning& data) {
  StrAppendToSink(sink, "Failed to initialize model ", data.model(), ": ", data.details());
}

void HorusStringify(const ErasedSink& sink, const RosSpinnerStoppedWarning& data) {
  StrAppendToSink(sink, "ROS spinner stopped: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ActorSystemUnavailableError& /*data*/) {
  StrAppendToSink(sink, "Actor system is not available. Please reinstantiate the pipeline.");
}

void HorusStringify(const ErasedSink& sink, const ConfigNodeNotFoundError& data) {
  StrAppendToSink(sink, "Configuration for node ", data.node(), " is unavailable");
}

void HorusStringify(const ErasedSink& sink, const BagTimestampOutOfOrderError& data) {
  StrAppendToSink(sink, "Timestamps are out of order: ", data.prior_timestamp(), " > ", data.current_timestamp());
}

void HorusStringify(const ErasedSink& sink, const BagReplayUnexpectedTimestampError& data) {
  StrAppendToSink(sink, "Unexpected timestamp: expected ", data.expected_timestamp(), " but received ", data.received_timestamp());
}

void HorusStringify(const ErasedSink& sink, const WebsocketClosedInfo& data) {
  StrAppendToSink(sink, "WebSocket RPC connection to ", data.uri(), " closed");
}

void HorusStringify(const ErasedSink& sink, const WebsocketOpenedInfo& data) {
  StrAppendToSink(sink, "WebSocket RPC connection to ", data.uri(), " opened (as ", data.endpoint_is_server_client(), ")");
}

void HorusStringify(const ErasedSink& sink, const SubscriberDisconnectedInfo& data) {
  StrAppendToSink(sink, "Subscriber ", data.name(), " disconnected (", data.uri(), ")");
}

void HorusStringify(const ErasedSink& sink, const ThreadPoolSlowingDownWarning& data) {
  StrAppendToSink(sink, "Thread pool is slowing down for ", data.number(), " times!");
}

void HorusStringify(const ErasedSink& sink, const ThreadPoolNotRespondingWarning& data) {
  StrAppendToSink(sink, "Thread pool is not responding for ", data.not_responding_for(), ".");
}

void HorusStringify(const ErasedSink& sink, const ThreadPoolBrokenPromiseWarning& /*data*/) {
  StrAppendToSink(sink, "Thread pool task future is no longer available. The thread pool might have been restarted during events such as input source change.");
}

void HorusStringify(const ErasedSink& sink, const BoxFittingPointsBelowBaseWarning& /*data*/) {
  StrAppendToSink(sink, "Some points are below the base Z-coordinate when fitting the box. This might affect the detection results.");
}

void HorusStringify(const ErasedSink& sink, const FailedToRemoveStalePointsWarning& data) {
  StrAppendToSink(sink, "Failed to remove stale points: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const IrregularBroadcastingPeriodWarning& data) {
  StrAppendToSink(sink, data.num_irregular(), " irregular broadcast(s) detected in the last ", data.duration(), " (mean deviation: ", data.mean_deviation(), ").");
}

void HorusStringify(const ErasedSink& sink, const ClustererPointsOutOfRangeWarning& data) {
  StrAppendToSink(sink, "There are ", data.num_points(), " points out of range in the clusterer. Some points: ", data.sample_points(), ".");
}

void HorusStringify(const ErasedSink& sink, const InternalError& data) {
  StrAppendToSink(sink, "An internal error occurred: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const InternalFatal& data) {
  StrAppendToSink(sink, "An internal fatal error occurred: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ServiceStartingInfo& data) {
  StrAppendToSink(sink, data.service(), " is starting");
}

void HorusStringify(const ErasedSink& sink, const ConfigNodeNotFoundFatal& data) {
  StrAppendToSink(sink, "Configuration for node ", data.node(), " is unavailable");
}

void HorusStringify(const ErasedSink& sink, const ServiceSetupError& data) {
  StrAppendToSink(sink, "Failed to setup: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const FilesystemError& data) {
  StrAppendToSink(sink, "Filesystem error: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const InvalidPresetWarning& data) {
  StrAppendToSink(sink, "Tried to apply invalid preset ", data.preset(), " due to ", data.reason(), ", falling back to ", data.fallback_name(), ".");
}

void HorusStringify(const ErasedSink& sink, const WebsocketFailedClearPendingError& data) {
  StrAppendToSink(sink, "WebSocket failed to clear pending queues: ", data.exception());
}

void HorusStringify(const ErasedSink& sink, const WebsocketFailedToStopError& data) {
  StrAppendToSink(sink, "WebSocket failed to stop: ", data.exception());
}

void HorusStringify(const ErasedSink& sink, const WebsocketFailedToCreateError& data) {
  StrAppendToSink(sink, "Failed to create RPC server ws://", data.host(), ":", data.port(), "/: ", data.exception());
}

void HorusStringify(const ErasedSink& sink, const UnexpectedRpcError& data) {
  StrAppendToSink(sink, "Unexpected RPC error: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const LicensePollFailed& data) {
  StrAppendToSink(sink, "Failed to poll license server: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const LicenseExpiredWarning& data) {
  StrAppendToSink(sink, "License expired on ", data.expiration_time(), ".");
}

void HorusStringify(const ErasedSink& sink, const LicenseUsageExceededWarning& data) {
  StrAppendToSink(sink, "License count of ", data.lidar_count(), " exceeds allowed usage of ", data.max_lidar_count());
}

void HorusStringify(const ErasedSink& sink, const StaticThreadPoolSlowTaskWarning& data) {
  StrAppendToSink(sink, "ThreadPool task took ", data.time_elapsed(), " to complete task enqueued in ", data.enqueue_location());
}

void HorusStringify(const ErasedSink& sink, const RpcUnsupportedServiceWarning& data) {
  StrAppendToSink(sink, "Received request for unsupported service: ", data.service_id());
}

void HorusStringify(const ErasedSink& sink, const WebsocketHandlerProblem& data) {
  StrAppendToSink(sink, "WebSocket RPC handler encountered an error: ", data.what());
}

void HorusStringify(const ErasedSink& sink, const WebsocketDeserializeError& data) {
  StrAppendToSink(sink, "WebSocket RPC received message cannot be deserialized: ", data.what());
}

void HorusStringify(const ErasedSink& sink, const WebsocketExpiredRpcEndpointError& data) {
  StrAppendToSink(sink, "Attempting to access expired WebSocket RPC endpoint ", data.uri());
}

void HorusStringify(const ErasedSink& sink, const WebsocketQueueOverloadedWarning& data) {
  StrAppendToSink(sink, "Websocket message queue overloaded: ", data.current(), " / ", data.max());
}

void HorusStringify(const ErasedSink& sink, const RpcFailedToNotifyWarning& data) {
  StrAppendToSink(sink, "Could not notify ", data.notification_name(), " to ", data.service(), " ", data.uri(), ": ", data.what());
}

void HorusStringify(const ErasedSink& sink, const ConfigSubscriptionFailedWarning& data) {
  StrAppendToSink(sink, "Failed to subscribe to config: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const ThreadPoolClampedWorkersWarning& data) {
  StrAppendToSink(sink, "Clamped thread pool workers to ", data.clamped(), " instead of ", data.requested());
}

void HorusStringify(const ErasedSink& sink, const StoppingHorusBagRecorderAlreadyStopped& /*data*/) {
  StrAppendToSink(sink, "Trying to stop the Horus bag recorder which is already stopped.");
}

void HorusStringify(const ErasedSink& sink, const RecorderConfigUpdateWhileRunning& /*data*/) {
  StrAppendToSink(sink, "An update for the Horus bag recorder has been received while it was running. This update will not be taken into account until the next recording.");
}

void HorusStringify(const ErasedSink& sink, const ClampingDataWarning& data) {
  StrAppendToSink(sink, "Clamping point cloud: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const LidarIncompatibleValues& data) {
  StrAppendToSink(sink, "Lidars ", data.lidar_ip1(), " and ", data.lidar_ip2(), " have incompatible ", data.value_names(), ": ", data.value1(), " vs. ", data.value2(), "; ", data.resolution(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotDetermineContainerIdError& data) {
  StrAppendToSink(sink, "Cannot determine current Docker container ID; unknown ", data.container_id_file_path(), " format.");
}

void HorusStringify(const ErasedSink& sink, const StartedLidarDriver& data) {
  StrAppendToSink(sink, "Started lidar driver container ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotStartLidarDriver& data) {
  StrAppendToSink(sink, "Cannot start lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const StoppedLidarDriver& data) {
  StrAppendToSink(sink, "Stopped lidar driver container ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotStopLidarDriver& data) {
  StrAppendToSink(sink, "Cannot stop lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const RestartedLidarDriver& data) {
  StrAppendToSink(sink, "Restarted lidar driver container ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotRestartLidarDriver& data) {
  StrAppendToSink(sink, "Cannot restart lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const RemovedUnusedLidarDriver& data) {
  StrAppendToSink(sink, "Removed unused lidar driver container ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const CannotRemoveUnusedLidarDriver& data) {
  StrAppendToSink(sink, "Cannot remove unused lidar driver container ", data.lidar_id(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const LidarDriverGcFailure& data) {
  StrAppendToSink(sink, "Error encountered while removing unused lidar driver containers: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const IdSpaceExhausted& data) {
  StrAppendToSink(sink, "Cannot allocate new identifier with prefix \"", data.prefix(), "\".");
}

void HorusStringify(const ErasedSink& sink, const PreprocessingToPointAggregatorPointsSkipped& data) {
  StrAppendToSink(sink, "The point cloud publishing to the point aggregator service has been skipped ", data.num_skipped_points(), " time(s) in the last ", data.check_interval(), ".");
}

void HorusStringify(const ErasedSink& sink, const MinMsgIntervalLessThanThreshold& data) {
  StrAppendToSink(sink, "Discarding lidar points from ", data.lidar_id(), " since the time interval between two point-cloud messages is too close (<", data.threshold(), "). Adjust the Min-Message Interval parameter to change this behavior.");
}

void HorusStringify(const ErasedSink& sink, const FailedToCleanupRosWarning& data) {
  StrAppendToSink(sink, "Failed to clean up ROS nodes and processes: ", data.details());
}

void HorusStringify(const ErasedSink& sink, const RpcDisconnectedWarning& data) {
  StrAppendToSink(sink, "RPC request ", data.request_name(), " to ", data.endpoint(), " failed since endpoint is disconnected.");
}

void HorusStringify(const ErasedSink& sink, const RpcUnhandledError& data) {
  StrAppendToSink(sink, "RPC request handler for ", data.request_name(), " failed due to an unhandled internal error: ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const TimeDiffOutOfRangeWarning& data) {
  StrAppendToSink(sink, "Time difference is out of range: ", data.diff(), ". Previous timestamp: ", data.prev_time(), ", Current timestamp: ", data.curr_time(), ".");
}

void HorusStringify(const ErasedSink& sink, const TensorrtLog& data) {
  StrAppendToSink(sink, "[TensorRT] ", data.log());
}

void HorusStringify(const ErasedSink& sink, const BuildingTensorrtEngineInfo& /*data*/) {
  StrAppendToSink(sink, "Building TensorRT engine since the model has changed or the engine is not available. This may take a while.");
}

void HorusStringify(const ErasedSink& sink, const LoadingTensorrtEngineInfo& /*data*/) {
  StrAppendToSink(sink, "Loading TensorRT engine. This may take a while.");
}

void HorusStringify(const ErasedSink& sink, const CalibrationMapNotFound& data) {
  StrAppendToSink(sink, "The calibration map ", data.path(), " was not found.");
}

void HorusStringify(const ErasedSink& sink, const CalibrationMapNotValid& data) {
  StrAppendToSink(sink, "The calibration map ", data.path(), " is not valid.");
}

void HorusStringify(const ErasedSink& sink, const CalibrationMapPathAlreadyExists& data) {
  StrAppendToSink(sink, "The calibration map path ", data.path(), " already exists.");
}

void HorusStringify(const ErasedSink& sink, const FailedToSaveCalibrationMap& data) {
  StrAppendToSink(sink, "Failed to save the calibration map to ", data.path(), ".");
}

void HorusStringify(const ErasedSink& sink, const FailedToRemoveCalibrationMap& data) {
  StrAppendToSink(sink, "Failed to remove the calibration map at ", data.path(), ".");
}

void HorusStringify(const ErasedSink& sink, const FailedToIterateInDirectory& data) {
  StrAppendToSink(sink, "Failed to iterate in the ", data.directory_name(), " directory ", data.directory_path(), ": ", data.details(), ".");
}

void HorusStringify(const ErasedSink& sink, const MapBasedCalibrationWithoutMapLoading& /*data*/) {
  StrAppendToSink(sink, "The calibration map must be running before triggering the map-based calibration.");
}

void HorusStringify(const ErasedSink& sink, const MapBasedCalibrationAlreadyRunning& /*data*/) {
  StrAppendToSink(sink, "The map-based calibration is already running.");
}

void HorusStringify(const ErasedSink& sink, const CancelMapBasedCalibrationNotRunning& /*data*/) {
  StrAppendToSink(sink, "A cancel map-based calibration request has been received while the calibration is not running.");
}

void HorusStringify(const ErasedSink& sink, const BagStreamNotFound& data) {
  StrAppendToSink(sink, "The stream ", data.stream_id(), " was not found from ", data.bag_path(), ".");
}

void HorusStringify(const ErasedSink& sink, const EvaluationBagStartedInfo& data) {
  StrAppendToSink(sink, "Evaluation bag started for ", data.bag_path());
}

void HorusStringify(const ErasedSink& sink, const EvaluationBagFinishedInfo& data) {
  StrAppendToSink(sink, "Evaluation bag finished for ", data.bag_path());
}

void HorusStringify(const ErasedSink& sink, const BagNotFound& data) {
  StrAppendToSink(sink, "The bag ", data.bag_path(), " was not found.");
}

void HorusStringify(const ErasedSink& sink, const BuildingPipelineInfo& data) {
  StrAppendToSink(sink, "Building the ", data.pipeline_name(), " pipeline.");
}

void HorusStringify(const ErasedSink& sink, const BagIsNotEvaluation& data) {
  StrAppendToSink(sink, "The bag ", data.bag_path(), " is not an evaluation bag.");
}

void HorusStringify(const ErasedSink& sink, const HorusBagRunning& /*data*/) {
  StrAppendToSink(sink, "A Horus bag is currently running.");
}

void HorusStringify(const ErasedSink& sink, const AutoGroundCalibrationWarning& data) {
  StrAppendToSink(sink, "Automatic ground calibration failed for the following lidars: ", data.failed_lidar_ids(), ".");
}

void HorusStringify(const ErasedSink& sink, const AutoGroundCalibrationError& /*data*/) {
  StrAppendToSink(sink, "Automatic ground calibration failed for all lidars.");
}

void HorusStringify(const ErasedSink& sink, const ObjectDetectorNotLoadedWarning& /*data*/) {
  StrAppendToSink(sink, "The object detector is not loaded yet. Temporarily using rule based detection which may result in degraded detection results.");
}

void HorusStringify(const ErasedSink& sink, const CalibrationIsRunningError& /*data*/) {
  StrAppendToSink(sink, "A calibration process is already running.");
}

void HorusStringify(const ErasedSink& sink, const ModelInitInProgressInfo& data) {
  StrAppendToSink(sink, data.component(), " will start processing data once all models have finished initializing.");
}

void HorusStringify(const ErasedSink& sink, const RpcTimeoutWithResolutionWarning& data) {
  StrAppendToSink(sink, "RPC request \"", data.request_name(), "\" to \"", data.endpoint(), "\" timed out after ", data.elapsed(), ". Resolution: ", data.resolution());
}

void HorusStringify(const ErasedSink& sink, const CalibrationWasCancelledInfo& data) {
  StrAppendToSink(sink, "The ", data.calibration_process_name(), " process was cancelled.");
}

void HorusStringify(const ErasedSink& sink, const CalibrationMapRecordingFailedToStart& data) {
  StrAppendToSink(sink, "Failed to start recording the calibration map: ", data.path());
}

void HorusStringify(const ErasedSink& sink, const DetectionPipelineRequestedResetInfo& data) {
  StrAppendToSink(sink, "The detection pipeline has been requested to be reset because ", data.reason(), ".");
}

void HorusStringify(const ErasedSink& sink, const PreprocessingServicePipelineUnavailable& data) {
  StrAppendToSink(sink, "Preprocessing service pipeline is not available yet due to ", data.reason(), ".");
}

void HorusStringify(const ErasedSink& sink, const CircularRecordingDisabledWarning& data) {
  StrAppendToSink(sink, "The timeframe snapshot is disabled: ", data.reason());
}

void HorusStringify(const ErasedSink& sink, const SnapshotAlreadyRunningWarning& /*data*/) {
  StrAppendToSink(sink, "A snapshot is already running.");
}

void HorusStringify(const ErasedSink& sink, const ActiveProjectChangedInfo& /*data*/) {
  StrAppendToSink(sink, "The active project has been changed.");
}

void HorusStringify(const ErasedSink& sink, const ProjectConfigUpdatedInfo& /*data*/) {
  StrAppendToSink(sink, "The project configuration has been updated.");
}

void HorusStringify(const ErasedSink& sink, const InvalidLidarTimestamp& data) {
  StrAppendToSink(sink, "Invalid timestamp ", data.timestamp(), " sent by lidar ", data.lidar_id(), ".");
}

void HorusStringify(const ErasedSink& sink, const CalibrationAccumulatingPointsInfo& data) {
  StrAppendToSink(sink, "Calibration is accumulating points for ", data.time());
}

void HorusStringify(const ErasedSink& sink, const SparseNoiseFilterUsageNonRotationalLidars& /*data*/) {
  StrAppendToSink(sink, "The sparse noise filter cannot be used with non-rotational lidars.");
}

void HorusStringify(const ErasedSink& sink, const FileWriteError& data) {
  StrAppendToSink(sink, "Failed to write to file \"", data.filename(), "\": \"", data.details(), "\".");
}

void HorusStringify(const ErasedSink& sink, const LicenseForbiddenFeature& data) {
  StrAppendToSink(sink, data.feature_name(), " is not allowed by the current license.");
}

}  // namespace logs
}  // namespace pb
}  // namespace sdk
}  // namespace horus
