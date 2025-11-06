#include "horus/pb/logs/message_pb.h"

#include "horus/pb/serialize.h"

namespace horus {
namespace sdk {
namespace pb {

LogData::LogData(const LogData& other) noexcept(false)
    : generic_{other.generic_}
    , oom_{other.oom_}
    , rpc_connection_error_{other.rpc_connection_error_}
    , invalid_project_name_{other.invalid_project_name_}
    , project_not_found_{other.project_not_found_}
    , project_already_exists_{other.project_already_exists_}
    , invalid_configuration_{other.invalid_configuration_}
    , entity_not_found_{other.entity_not_found_}
    , active_project_cannot_be_deleted_{other.active_project_cannot_be_deleted_}
    , rpc_disconnection_error_{other.rpc_disconnection_error_}
    , dropped_logs_{other.dropped_logs_}
    , opened_project_{other.opened_project_}
    , created_project_{other.created_project_}
    , config_unavailable_{other.config_unavailable_}
    , invalid_request_{other.invalid_request_}
    , sanity_check_error_{other.sanity_check_error_}
    , bag_failed_to_open_{other.bag_failed_to_open_}
    , bag_failed_to_close_{other.bag_failed_to_close_}
    , bag_conversion_failed_{other.bag_conversion_failed_}
    , bag_failed_to_write_{other.bag_failed_to_write_}
    , calibration_error_{other.calibration_error_}
    , project_manager_failed_to_start_recording_{other.project_manager_failed_to_start_recording_}
    , project_manager_failed_to_stop_recording_{other.project_manager_failed_to_stop_recording_}
    , service_connection_timed_out_{other.service_connection_timed_out_}
    , bag_recorder_already_running_{other.bag_recorder_already_running_}
    , license_server_connection_error_{other.license_server_connection_error_}
    , license_error_{other.license_error_}
    , license_not_found_error_{other.license_not_found_error_}
    , license_expired_error_{other.license_expired_error_}
    , license_exceeded_error_{other.license_exceeded_error_}
    , license_host_machine_error_{other.license_host_machine_error_}
    , license_privilege_error_{other.license_privilege_error_}
    , license_active_info_{other.license_active_info_}
    , multiple_licenses_warning_{other.multiple_licenses_warning_}
    , license_current_license_status_info_{other.license_current_license_status_info_}
    , bag_recording_stopped_info_{other.bag_recording_stopped_info_}
    , bag_recording_failed_to_start_{other.bag_recording_failed_to_start_}
    , bag_recording_started_info_{other.bag_recording_started_info_}
    , replay_restarted_info_{other.replay_restarted_info_}
    , input_source_change_requested_info_{other.input_source_change_requested_info_}
    , input_source_switched_info_{other.input_source_switched_info_}
    , rpc_timeout_warning_{other.rpc_timeout_warning_}
    , cannot_write_log_file_{other.cannot_write_log_file_}
    , point_cloud_parsing_failure_warning_{other.point_cloud_parsing_failure_warning_}
    , lidar_is_dead_{other.lidar_is_dead_}
    , lidar_is_not_dead_anymore_{other.lidar_is_not_dead_anymore_}
    , lidar_is_obstructed_{other.lidar_is_obstructed_}
    , lidar_is_not_obstructed_anymore_{other.lidar_is_not_obstructed_anymore_}
    , lidar_is_tilted_{other.lidar_is_tilted_}
    , lidar_is_not_tilted_anymore_{other.lidar_is_not_tilted_anymore_}
    , lidar_has_been_automatically_recalibrated_{other.lidar_has_been_automatically_recalibrated_}
    , received_first_data_for_lidar_{other.received_first_data_for_lidar_}
    , termination_failure_error_{other.termination_failure_error_}
    , frame_processing_error_{other.frame_processing_error_}
    , thread_pool_unavailable_error_{other.thread_pool_unavailable_error_}
    , invalid_argument_{other.invalid_argument_}
    , component_initialization_failure_fatal_{other.component_initialization_failure_fatal_}
    , unhandled_enum_case_error_{other.unhandled_enum_case_error_}
    , bag_empty_error_{other.bag_empty_error_}
    , discarding_data_error_{other.discarding_data_error_}
    , discarding_data_warning_{other.discarding_data_warning_}
    , nothing_to_process_{other.nothing_to_process_}
    , invalid_component_configuration_{other.invalid_component_configuration_}
    , channel_receiver_not_found_warning_{other.channel_receiver_not_found_warning_}
    , model_load_failure_{other.model_load_failure_}
    , model_execution_failure_error_{other.model_execution_failure_error_}
    , service_ready_info_{other.service_ready_info_}
    , model_preparing_info_{other.model_preparing_info_}
    , model_initialized_info_{other.model_initialized_info_}
    , model_initialization_failure_warning_{other.model_initialization_failure_warning_}
    , ros_spinner_stopped_warning_{other.ros_spinner_stopped_warning_}
    , actor_system_unavailable_error_{other.actor_system_unavailable_error_}
    , config_node_not_found_error_{other.config_node_not_found_error_}
    , bag_timestamp_out_of_order_error_{other.bag_timestamp_out_of_order_error_}
    , bag_replay_unexpected_timestamp_error_{other.bag_replay_unexpected_timestamp_error_}
    , websocket_closed_info_{other.websocket_closed_info_}
    , websocket_opened_info_{other.websocket_opened_info_}
    , subscriber_disconnected_info_{other.subscriber_disconnected_info_}
    , thread_pool_slowing_down_warning_{other.thread_pool_slowing_down_warning_}
    , thread_pool_not_responding_warning_{other.thread_pool_not_responding_warning_}
    , thread_pool_broken_promise_warning_{other.thread_pool_broken_promise_warning_}
    , box_fitting_points_below_base_warning_{other.box_fitting_points_below_base_warning_}
    , failed_to_remove_stale_points_warning_{other.failed_to_remove_stale_points_warning_}
    , irregular_broadcasting_period_warning_{other.irregular_broadcasting_period_warning_}
    , clusterer_points_out_of_range_warning_{other.clusterer_points_out_of_range_warning_}
    , internal_error_{other.internal_error_}
    , internal_fatal_{other.internal_fatal_}
    , service_starting_info_{other.service_starting_info_}
    , config_node_not_found_fatal_{other.config_node_not_found_fatal_}
    , service_setup_error_{other.service_setup_error_}
    , filesystem_error_{other.filesystem_error_}
    , invalid_preset_warning_{other.invalid_preset_warning_}
    , websocket_failed_clear_pending_error_{other.websocket_failed_clear_pending_error_}
    , websocket_failed_to_stop_error_{other.websocket_failed_to_stop_error_}
    , websocket_failed_to_create_error_{other.websocket_failed_to_create_error_}
    , unexpected_rpc_error_{other.unexpected_rpc_error_}
    , license_poll_failed_{other.license_poll_failed_}
    , license_expired_warning_{other.license_expired_warning_}
    , license_usage_exceeded_warning_{other.license_usage_exceeded_warning_}
    , static_thread_pool_slow_task_warning_{other.static_thread_pool_slow_task_warning_}
    , rpc_unsupported_service_warning_{other.rpc_unsupported_service_warning_}
    , websocket_handler_problem_{other.websocket_handler_problem_}
    , websocket_deserialize_error_{other.websocket_deserialize_error_}
    , websocket_expired_rpc_endpoint_error_{other.websocket_expired_rpc_endpoint_error_}
    , websocket_queue_overloaded_warning_{other.websocket_queue_overloaded_warning_}
    , rpc_failed_to_notify_warning_{other.rpc_failed_to_notify_warning_}
    , config_subscription_failed_warning_{other.config_subscription_failed_warning_}
    , thread_pool_clamped_workers_warning_{other.thread_pool_clamped_workers_warning_}
    , stopping_horus_bag_recorder_already_stopped_{other.stopping_horus_bag_recorder_already_stopped_}
    , recorder_config_update_while_running_{other.recorder_config_update_while_running_}
    , clamping_data_warning_{other.clamping_data_warning_}
    , lidar_incompatible_values_{other.lidar_incompatible_values_}
    , cannot_determine_container_id_error_{other.cannot_determine_container_id_error_}
    , started_lidar_driver_{other.started_lidar_driver_}
    , cannot_start_lidar_driver_{other.cannot_start_lidar_driver_}
    , stopped_lidar_driver_{other.stopped_lidar_driver_}
    , cannot_stop_lidar_driver_{other.cannot_stop_lidar_driver_}
    , restarted_lidar_driver_{other.restarted_lidar_driver_}
    , cannot_restart_lidar_driver_{other.cannot_restart_lidar_driver_}
    , removed_unused_lidar_driver_{other.removed_unused_lidar_driver_}
    , cannot_remove_unused_lidar_driver_{other.cannot_remove_unused_lidar_driver_}
    , lidar_driver_gc_failure_{other.lidar_driver_gc_failure_}
    , id_space_exhausted_{other.id_space_exhausted_}
    , preprocessing_to_point_aggregator_points_skipped_{other.preprocessing_to_point_aggregator_points_skipped_}
    , min_msg_interval_less_than_threshold_{other.min_msg_interval_less_than_threshold_}
    , failed_to_cleanup_ros_warning_{other.failed_to_cleanup_ros_warning_}
    , rpc_disconnected_warning_{other.rpc_disconnected_warning_}
    , rpc_unhandled_error_{other.rpc_unhandled_error_}
    , time_diff_out_of_range_warning_{other.time_diff_out_of_range_warning_}
    , tensorrt_log_{other.tensorrt_log_}
    , building_tensorrt_engine_info_{other.building_tensorrt_engine_info_}
    , loading_tensorrt_engine_info_{other.loading_tensorrt_engine_info_}
    , calibration_map_not_found_{other.calibration_map_not_found_}
    , calibration_map_not_valid_{other.calibration_map_not_valid_}
    , calibration_map_path_already_exists_{other.calibration_map_path_already_exists_}
    , failed_to_save_calibration_map_{other.failed_to_save_calibration_map_}
    , failed_to_remove_calibration_map_{other.failed_to_remove_calibration_map_}
    , failed_to_iterate_in_directory_{other.failed_to_iterate_in_directory_}
    , map_based_calibration_without_map_loading_{other.map_based_calibration_without_map_loading_}
    , map_based_calibration_already_running_{other.map_based_calibration_already_running_}
    , cancel_map_based_calibration_not_running_{other.cancel_map_based_calibration_not_running_}
    , bag_stream_not_found_{other.bag_stream_not_found_}
    , evaluation_bag_started_info_{other.evaluation_bag_started_info_}
    , evaluation_bag_finished_info_{other.evaluation_bag_finished_info_}
    , bag_not_found_{other.bag_not_found_}
    , building_pipeline_info_{other.building_pipeline_info_}
    , bag_is_not_evaluation_{other.bag_is_not_evaluation_}
    , horus_bag_running_{other.horus_bag_running_}
    , auto_ground_calibration_warning_{other.auto_ground_calibration_warning_}
    , auto_ground_calibration_error_{other.auto_ground_calibration_error_}
    , object_detector_not_loaded_warning_{other.object_detector_not_loaded_warning_}
    , calibration_is_running_error_{other.calibration_is_running_error_}
    , model_init_in_progress_info_{other.model_init_in_progress_info_}
    , rpc_timeout_with_resolution_warning_{other.rpc_timeout_with_resolution_warning_}
    , calibration_was_cancelled_info_{other.calibration_was_cancelled_info_}
    , calibration_map_recording_failed_to_start_{other.calibration_map_recording_failed_to_start_}
    , detection_pipeline_requested_reset_info_{other.detection_pipeline_requested_reset_info_}
    , preprocessing_service_pipeline_unavailable_{other.preprocessing_service_pipeline_unavailable_}
    , circular_recording_disabled_warning_{other.circular_recording_disabled_warning_}
    , snapshot_already_running_warning_{other.snapshot_already_running_warning_}
    , active_project_changed_info_{other.active_project_changed_info_}
    , project_config_updated_info_{other.project_config_updated_info_}
    , invalid_lidar_timestamp_{other.invalid_lidar_timestamp_}
    , calibration_accumulating_points_info_{other.calibration_accumulating_points_info_}
    , sparse_noise_filter_usage_non_rotational_lidars_{other.sparse_noise_filter_usage_non_rotational_lidars_}
    , file_write_warning_{other.file_write_warning_}
    , license_forbidden_feature_{other.license_forbidden_feature_}
    , failed_to_update_configuration_{other.failed_to_update_configuration_}
    , obstruction_detector_bad_reference_warning_{other.obstruction_detector_bad_reference_warning_}
    , project_file_invalid_permissions_error_{other.project_file_invalid_permissions_error_}
    , pipeline_scheduler_error_{other.pipeline_scheduler_error_}
    , multi_lidar_calibration_warning_{other.multi_lidar_calibration_warning_}
    , multi_lidar_calibration_error_{other.multi_lidar_calibration_error_}
    , old_bag_warning_{other.old_bag_warning_}
    , upgrading_bag_info_{other.upgrading_bag_info_}
    , bag_calibration_save_failed_{other.bag_calibration_save_failed_}
    , bag_upgrade_failed_{other.bag_upgrade_failed_}
    , unknown_lidar_error_{other.unknown_lidar_error_}
    , invalid_point_cloud_warning_{other.invalid_point_cloud_warning_}
    , lidar_is_dropping_packets_{other.lidar_is_dropping_packets_}
    , removed_invalid_lidars_from_config_warning_{other.removed_invalid_lidars_from_config_warning_}
    , calibration_map_recording_failed_{other.calibration_map_recording_failed_}
    , detection_node_not_found_error_{other.detection_node_not_found_error_}
    , created_version_backup_info_{other.created_version_backup_info_}
    , ply_file_load_failed_error_{other.ply_file_load_failed_error_}
    , hesai_driver_lifecycle_{other.hesai_driver_lifecycle_}
    , hesai_driver_error_{other.hesai_driver_error_}
    , hesai_packet_processing_failed_{other.hesai_packet_processing_failed_}
    , hesai_correction_file_error_{other.hesai_correction_file_error_}
    , hesai_packet_statistics_{other.hesai_packet_statistics_}
    , ply_file_write_failed_error_{other.ply_file_write_failed_error_}
    , project_save_error_{other.project_save_error_}
    , save_static_environment_success_{other.save_static_environment_success_}
    , save_static_environment_failed_{other.save_static_environment_failed_}
    , load_static_environment_success_{other.load_static_environment_success_}
    , load_static_environment_failed_{other.load_static_environment_failed_}
    , attempt_to_inject_invalid_lidar_id_warning_{other.attempt_to_inject_invalid_lidar_id_warning_}
    , reset_bundled_packet_due_to_unexpected_packet_{other.reset_bundled_packet_due_to_unexpected_packet_}
    , packet_bundler_dropped_packets_warning_{other.packet_bundler_dropped_packets_warning_}
    , packet_bundler_frame_jump_warning_{other.packet_bundler_frame_jump_warning_}
    , lidar_correction_loading_success_{other.lidar_correction_loading_success_}
    , lidar_correction_loading_failure_{other.lidar_correction_loading_failure_}
    , hesai_packet_statistics_lidar_{other.hesai_packet_statistics_lidar_}
    , lidar_tilt_detection_aligned_to_calibration_map_info_{other.lidar_tilt_detection_aligned_to_calibration_map_info_}
    , lidar_tilt_detection_misaligned_to_calibration_map_warning_{other.lidar_tilt_detection_misaligned_to_calibration_map_warning_}
    , data_{other.data_}
    , set_fields_{other.set_fields_} {}

void LogData::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<logs::Generic>(writer, /*tag=*/ 1, generic_);
  }
  if (set_fields_[1]) {
    SerializeField<logs::Oom>(writer, /*tag=*/ 2, oom_);
  }
  if (set_fields_[2]) {
    SerializeField<logs::RpcConnectionError>(writer, /*tag=*/ 3, rpc_connection_error_);
  }
  if (set_fields_[3]) {
    SerializeField<logs::InvalidProjectName>(writer, /*tag=*/ 4, invalid_project_name_);
  }
  if (set_fields_[4]) {
    SerializeField<logs::ProjectNotFound>(writer, /*tag=*/ 5, project_not_found_);
  }
  if (set_fields_[5]) {
    SerializeField<logs::ProjectAlreadyExists>(writer, /*tag=*/ 6, project_already_exists_);
  }
  if (set_fields_[6]) {
    SerializeField<logs::InvalidConfiguration>(writer, /*tag=*/ 7, invalid_configuration_);
  }
  if (set_fields_[7]) {
    SerializeField<logs::EntityNotFound>(writer, /*tag=*/ 8, entity_not_found_);
  }
  if (set_fields_[8]) {
    SerializeField<logs::ActiveProjectCannotBeDeleted>(writer, /*tag=*/ 9, active_project_cannot_be_deleted_);
  }
  if (set_fields_[9]) {
    SerializeField<logs::RpcDisconnectionError>(writer, /*tag=*/ 10, rpc_disconnection_error_);
  }
  if (set_fields_[10]) {
    SerializeField<logs::DroppedLogs>(writer, /*tag=*/ 11, dropped_logs_);
  }
  if (set_fields_[11]) {
    SerializeField<logs::OpenedProject>(writer, /*tag=*/ 12, opened_project_);
  }
  if (set_fields_[12]) {
    SerializeField<logs::CreatedProject>(writer, /*tag=*/ 13, created_project_);
  }
  if (set_fields_[13]) {
    SerializeField<logs::ConfigUnavailable>(writer, /*tag=*/ 14, config_unavailable_);
  }
  if (set_fields_[14]) {
    SerializeField<logs::InvalidRequest>(writer, /*tag=*/ 15, invalid_request_);
  }
  if (set_fields_[15]) {
    SerializeField<logs::SanityCheckError>(writer, /*tag=*/ 16, sanity_check_error_);
  }
  if (set_fields_[16]) {
    SerializeField<logs::BagFailedToOpen>(writer, /*tag=*/ 17, bag_failed_to_open_);
  }
  if (set_fields_[17]) {
    SerializeField<logs::BagFailedToClose>(writer, /*tag=*/ 18, bag_failed_to_close_);
  }
  if (set_fields_[18]) {
    SerializeField<logs::BagConversionFailed>(writer, /*tag=*/ 19, bag_conversion_failed_);
  }
  if (set_fields_[19]) {
    SerializeField<logs::BagFailedToWrite>(writer, /*tag=*/ 20, bag_failed_to_write_);
  }
  if (set_fields_[20]) {
    SerializeField<logs::CalibrationError>(writer, /*tag=*/ 21, calibration_error_);
  }
  if (set_fields_[21]) {
    SerializeField<logs::ProjectManagerFailedToStartRecording>(writer, /*tag=*/ 22, project_manager_failed_to_start_recording_);
  }
  if (set_fields_[22]) {
    SerializeField<logs::ProjectManagerFailedToStopRecording>(writer, /*tag=*/ 23, project_manager_failed_to_stop_recording_);
  }
  if (set_fields_[23]) {
    SerializeField<logs::ServiceConnectionTimedOut>(writer, /*tag=*/ 24, service_connection_timed_out_);
  }
  if (set_fields_[24]) {
    SerializeField<logs::BagRecorderAlreadyRunning>(writer, /*tag=*/ 25, bag_recorder_already_running_);
  }
  if (set_fields_[25]) {
    SerializeField<logs::LicenseServerConnectionError>(writer, /*tag=*/ 26, license_server_connection_error_);
  }
  if (set_fields_[26]) {
    SerializeField<logs::LicenseError>(writer, /*tag=*/ 27, license_error_);
  }
  if (set_fields_[27]) {
    SerializeField<logs::LicenseNotFoundError>(writer, /*tag=*/ 145, license_not_found_error_);
  }
  if (set_fields_[28]) {
    SerializeField<logs::LicenseExpiredError>(writer, /*tag=*/ 146, license_expired_error_);
  }
  if (set_fields_[29]) {
    SerializeField<logs::LicenseExceededError>(writer, /*tag=*/ 147, license_exceeded_error_);
  }
  if (set_fields_[30]) {
    SerializeField<logs::LicenseHostMachineError>(writer, /*tag=*/ 28, license_host_machine_error_);
  }
  if (set_fields_[31]) {
    SerializeField<logs::LicensePrivilegeError>(writer, /*tag=*/ 148, license_privilege_error_);
  }
  if (set_fields_[32]) {
    SerializeField<logs::LicenseActiveInfo>(writer, /*tag=*/ 149, license_active_info_);
  }
  if (set_fields_[33]) {
    SerializeField<logs::MultipleLicensesWarning>(writer, /*tag=*/ 150, multiple_licenses_warning_);
  }
  if (set_fields_[34]) {
    SerializeField<logs::LicenseCurrentLicenseStatusInfo>(writer, /*tag=*/ 29, license_current_license_status_info_);
  }
  if (set_fields_[35]) {
    SerializeField<logs::BagRecordingStoppedInfo>(writer, /*tag=*/ 30, bag_recording_stopped_info_);
  }
  if (set_fields_[36]) {
    SerializeField<logs::BagRecordingFailedToStart>(writer, /*tag=*/ 31, bag_recording_failed_to_start_);
  }
  if (set_fields_[37]) {
    SerializeField<logs::BagRecordingStartedInfo>(writer, /*tag=*/ 32, bag_recording_started_info_);
  }
  if (set_fields_[38]) {
    SerializeField<logs::ReplayRestartedInfo>(writer, /*tag=*/ 33, replay_restarted_info_);
  }
  if (set_fields_[39]) {
    SerializeField<logs::InputSourceChangeRequestedInfo>(writer, /*tag=*/ 34, input_source_change_requested_info_);
  }
  if (set_fields_[40]) {
    SerializeField<logs::InputSourceSwitchedInfo>(writer, /*tag=*/ 35, input_source_switched_info_);
  }
  if (set_fields_[41]) {
    SerializeField<logs::RpcTimeoutWarning>(writer, /*tag=*/ 36, rpc_timeout_warning_);
  }
  if (set_fields_[42]) {
    SerializeField<logs::CannotWriteLogFile>(writer, /*tag=*/ 37, cannot_write_log_file_);
  }
  if (set_fields_[43]) {
    SerializeField<logs::PointCloudParsingFailureWarning>(writer, /*tag=*/ 38, point_cloud_parsing_failure_warning_);
  }
  if (set_fields_[44]) {
    SerializeField<logs::LidarIsDead>(writer, /*tag=*/ 39, lidar_is_dead_);
  }
  if (set_fields_[45]) {
    SerializeField<logs::LidarIsNotDeadAnymore>(writer, /*tag=*/ 40, lidar_is_not_dead_anymore_);
  }
  if (set_fields_[46]) {
    SerializeField<logs::LidarIsObstructed>(writer, /*tag=*/ 41, lidar_is_obstructed_);
  }
  if (set_fields_[47]) {
    SerializeField<logs::LidarIsNotObstructedAnymore>(writer, /*tag=*/ 42, lidar_is_not_obstructed_anymore_);
  }
  if (set_fields_[48]) {
    SerializeField<logs::LidarIsTilted>(writer, /*tag=*/ 43, lidar_is_tilted_);
  }
  if (set_fields_[49]) {
    SerializeField<logs::LidarIsNotTiltedAnymore>(writer, /*tag=*/ 44, lidar_is_not_tilted_anymore_);
  }
  if (set_fields_[50]) {
    SerializeField<logs::LidarHasBeenAutomaticallyRecalibrated>(writer, /*tag=*/ 45, lidar_has_been_automatically_recalibrated_);
  }
  if (set_fields_[51]) {
    SerializeField<logs::ReceivedFirstDataForLidar>(writer, /*tag=*/ 46, received_first_data_for_lidar_);
  }
  if (set_fields_[52]) {
    SerializeField<logs::TerminationFailureError>(writer, /*tag=*/ 47, termination_failure_error_);
  }
  if (set_fields_[53]) {
    SerializeField<logs::FrameProcessingError>(writer, /*tag=*/ 48, frame_processing_error_);
  }
  if (set_fields_[54]) {
    SerializeField<logs::ThreadPoolUnavailableError>(writer, /*tag=*/ 49, thread_pool_unavailable_error_);
  }
  if (set_fields_[55]) {
    SerializeField<logs::InvalidArgument>(writer, /*tag=*/ 50, invalid_argument_);
  }
  if (set_fields_[56]) {
    SerializeField<logs::ComponentInitializationFailureFatal>(writer, /*tag=*/ 51, component_initialization_failure_fatal_);
  }
  if (set_fields_[57]) {
    SerializeField<logs::UnhandledEnumCaseError>(writer, /*tag=*/ 52, unhandled_enum_case_error_);
  }
  if (set_fields_[58]) {
    SerializeField<logs::BagEmptyError>(writer, /*tag=*/ 53, bag_empty_error_);
  }
  if (set_fields_[59]) {
    SerializeField<logs::DiscardingDataError>(writer, /*tag=*/ 54, discarding_data_error_);
  }
  if (set_fields_[60]) {
    SerializeField<logs::DiscardingDataWarning>(writer, /*tag=*/ 55, discarding_data_warning_);
  }
  if (set_fields_[61]) {
    SerializeField<logs::NothingToProcess>(writer, /*tag=*/ 56, nothing_to_process_);
  }
  if (set_fields_[62]) {
    SerializeField<logs::InvalidComponentConfiguration>(writer, /*tag=*/ 57, invalid_component_configuration_);
  }
  if (set_fields_[63]) {
    SerializeField<logs::ChannelReceiverNotFoundWarning>(writer, /*tag=*/ 58, channel_receiver_not_found_warning_);
  }
  if (set_fields_[64]) {
    SerializeField<logs::ModelLoadFailure>(writer, /*tag=*/ 59, model_load_failure_);
  }
  if (set_fields_[65]) {
    SerializeField<logs::ModelExecutionFailureError>(writer, /*tag=*/ 60, model_execution_failure_error_);
  }
  if (set_fields_[66]) {
    SerializeField<logs::ServiceReadyInfo>(writer, /*tag=*/ 61, service_ready_info_);
  }
  if (set_fields_[67]) {
    SerializeField<logs::ModelPreparingInfo>(writer, /*tag=*/ 62, model_preparing_info_);
  }
  if (set_fields_[68]) {
    SerializeField<logs::ModelInitializedInfo>(writer, /*tag=*/ 63, model_initialized_info_);
  }
  if (set_fields_[69]) {
    SerializeField<logs::ModelInitializationFailureWarning>(writer, /*tag=*/ 64, model_initialization_failure_warning_);
  }
  if (set_fields_[70]) {
    SerializeField<logs::RosSpinnerStoppedWarning>(writer, /*tag=*/ 65, ros_spinner_stopped_warning_);
  }
  if (set_fields_[71]) {
    SerializeField<logs::ActorSystemUnavailableError>(writer, /*tag=*/ 66, actor_system_unavailable_error_);
  }
  if (set_fields_[72]) {
    SerializeField<logs::ConfigNodeNotFoundError>(writer, /*tag=*/ 67, config_node_not_found_error_);
  }
  if (set_fields_[73]) {
    SerializeField<logs::BagTimestampOutOfOrderError>(writer, /*tag=*/ 68, bag_timestamp_out_of_order_error_);
  }
  if (set_fields_[74]) {
    SerializeField<logs::BagReplayUnexpectedTimestampError>(writer, /*tag=*/ 69, bag_replay_unexpected_timestamp_error_);
  }
  if (set_fields_[75]) {
    SerializeField<logs::WebsocketClosedInfo>(writer, /*tag=*/ 70, websocket_closed_info_);
  }
  if (set_fields_[76]) {
    SerializeField<logs::WebsocketOpenedInfo>(writer, /*tag=*/ 71, websocket_opened_info_);
  }
  if (set_fields_[77]) {
    SerializeField<logs::SubscriberDisconnectedInfo>(writer, /*tag=*/ 72, subscriber_disconnected_info_);
  }
  if (set_fields_[78]) {
    SerializeField<logs::ThreadPoolSlowingDownWarning>(writer, /*tag=*/ 73, thread_pool_slowing_down_warning_);
  }
  if (set_fields_[79]) {
    SerializeField<logs::ThreadPoolNotRespondingWarning>(writer, /*tag=*/ 74, thread_pool_not_responding_warning_);
  }
  if (set_fields_[80]) {
    SerializeField<logs::ThreadPoolBrokenPromiseWarning>(writer, /*tag=*/ 75, thread_pool_broken_promise_warning_);
  }
  if (set_fields_[81]) {
    SerializeField<logs::BoxFittingPointsBelowBaseWarning>(writer, /*tag=*/ 76, box_fitting_points_below_base_warning_);
  }
  if (set_fields_[82]) {
    SerializeField<logs::FailedToRemoveStalePointsWarning>(writer, /*tag=*/ 77, failed_to_remove_stale_points_warning_);
  }
  if (set_fields_[83]) {
    SerializeField<logs::IrregularBroadcastingPeriodWarning>(writer, /*tag=*/ 78, irregular_broadcasting_period_warning_);
  }
  if (set_fields_[84]) {
    SerializeField<logs::ClustererPointsOutOfRangeWarning>(writer, /*tag=*/ 79, clusterer_points_out_of_range_warning_);
  }
  if (set_fields_[85]) {
    SerializeField<logs::InternalError>(writer, /*tag=*/ 80, internal_error_);
  }
  if (set_fields_[86]) {
    SerializeField<logs::InternalFatal>(writer, /*tag=*/ 81, internal_fatal_);
  }
  if (set_fields_[87]) {
    SerializeField<logs::ServiceStartingInfo>(writer, /*tag=*/ 82, service_starting_info_);
  }
  if (set_fields_[88]) {
    SerializeField<logs::ConfigNodeNotFoundFatal>(writer, /*tag=*/ 83, config_node_not_found_fatal_);
  }
  if (set_fields_[89]) {
    SerializeField<logs::ServiceSetupError>(writer, /*tag=*/ 84, service_setup_error_);
  }
  if (set_fields_[90]) {
    SerializeField<logs::FilesystemError>(writer, /*tag=*/ 85, filesystem_error_);
  }
  if (set_fields_[91]) {
    SerializeField<logs::InvalidPresetWarning>(writer, /*tag=*/ 86, invalid_preset_warning_);
  }
  if (set_fields_[92]) {
    SerializeField<logs::WebsocketFailedClearPendingError>(writer, /*tag=*/ 87, websocket_failed_clear_pending_error_);
  }
  if (set_fields_[93]) {
    SerializeField<logs::WebsocketFailedToStopError>(writer, /*tag=*/ 88, websocket_failed_to_stop_error_);
  }
  if (set_fields_[94]) {
    SerializeField<logs::WebsocketFailedToCreateError>(writer, /*tag=*/ 89, websocket_failed_to_create_error_);
  }
  if (set_fields_[95]) {
    SerializeField<logs::UnexpectedRpcError>(writer, /*tag=*/ 90, unexpected_rpc_error_);
  }
  if (set_fields_[96]) {
    SerializeField<logs::LicensePollFailed>(writer, /*tag=*/ 91, license_poll_failed_);
  }
  if (set_fields_[97]) {
    SerializeField<logs::LicenseExpiredWarning>(writer, /*tag=*/ 92, license_expired_warning_);
  }
  if (set_fields_[98]) {
    SerializeField<logs::LicenseUsageExceededWarning>(writer, /*tag=*/ 93, license_usage_exceeded_warning_);
  }
  if (set_fields_[99]) {
    SerializeField<logs::StaticThreadPoolSlowTaskWarning>(writer, /*tag=*/ 94, static_thread_pool_slow_task_warning_);
  }
  if (set_fields_[100]) {
    SerializeField<logs::RpcUnsupportedServiceWarning>(writer, /*tag=*/ 95, rpc_unsupported_service_warning_);
  }
  if (set_fields_[101]) {
    SerializeField<logs::WebsocketHandlerProblem>(writer, /*tag=*/ 96, websocket_handler_problem_);
  }
  if (set_fields_[102]) {
    SerializeField<logs::WebsocketDeserializeError>(writer, /*tag=*/ 97, websocket_deserialize_error_);
  }
  if (set_fields_[103]) {
    SerializeField<logs::WebsocketExpiredRpcEndpointError>(writer, /*tag=*/ 98, websocket_expired_rpc_endpoint_error_);
  }
  if (set_fields_[104]) {
    SerializeField<logs::WebsocketQueueOverloadedWarning>(writer, /*tag=*/ 99, websocket_queue_overloaded_warning_);
  }
  if (set_fields_[105]) {
    SerializeField<logs::RpcFailedToNotifyWarning>(writer, /*tag=*/ 100, rpc_failed_to_notify_warning_);
  }
  if (set_fields_[106]) {
    SerializeField<logs::ConfigSubscriptionFailedWarning>(writer, /*tag=*/ 101, config_subscription_failed_warning_);
  }
  if (set_fields_[107]) {
    SerializeField<logs::ThreadPoolClampedWorkersWarning>(writer, /*tag=*/ 102, thread_pool_clamped_workers_warning_);
  }
  if (set_fields_[108]) {
    SerializeField<logs::StoppingHorusBagRecorderAlreadyStopped>(writer, /*tag=*/ 103, stopping_horus_bag_recorder_already_stopped_);
  }
  if (set_fields_[109]) {
    SerializeField<logs::RecorderConfigUpdateWhileRunning>(writer, /*tag=*/ 104, recorder_config_update_while_running_);
  }
  if (set_fields_[110]) {
    SerializeField<logs::ClampingDataWarning>(writer, /*tag=*/ 105, clamping_data_warning_);
  }
  if (set_fields_[111]) {
    SerializeField<logs::LidarIncompatibleValues>(writer, /*tag=*/ 106, lidar_incompatible_values_);
  }
  if (set_fields_[112]) {
    SerializeField<logs::CannotDetermineContainerIdError>(writer, /*tag=*/ 107, cannot_determine_container_id_error_);
  }
  if (set_fields_[113]) {
    SerializeField<logs::StartedLidarDriver>(writer, /*tag=*/ 108, started_lidar_driver_);
  }
  if (set_fields_[114]) {
    SerializeField<logs::CannotStartLidarDriver>(writer, /*tag=*/ 109, cannot_start_lidar_driver_);
  }
  if (set_fields_[115]) {
    SerializeField<logs::StoppedLidarDriver>(writer, /*tag=*/ 110, stopped_lidar_driver_);
  }
  if (set_fields_[116]) {
    SerializeField<logs::CannotStopLidarDriver>(writer, /*tag=*/ 111, cannot_stop_lidar_driver_);
  }
  if (set_fields_[117]) {
    SerializeField<logs::RestartedLidarDriver>(writer, /*tag=*/ 112, restarted_lidar_driver_);
  }
  if (set_fields_[118]) {
    SerializeField<logs::CannotRestartLidarDriver>(writer, /*tag=*/ 113, cannot_restart_lidar_driver_);
  }
  if (set_fields_[119]) {
    SerializeField<logs::RemovedUnusedLidarDriver>(writer, /*tag=*/ 114, removed_unused_lidar_driver_);
  }
  if (set_fields_[120]) {
    SerializeField<logs::CannotRemoveUnusedLidarDriver>(writer, /*tag=*/ 115, cannot_remove_unused_lidar_driver_);
  }
  if (set_fields_[121]) {
    SerializeField<logs::LidarDriverGcFailure>(writer, /*tag=*/ 116, lidar_driver_gc_failure_);
  }
  if (set_fields_[122]) {
    SerializeField<logs::IdSpaceExhausted>(writer, /*tag=*/ 117, id_space_exhausted_);
  }
  if (set_fields_[123]) {
    SerializeField<logs::PreprocessingToPointAggregatorPointsSkipped>(writer, /*tag=*/ 118, preprocessing_to_point_aggregator_points_skipped_);
  }
  if (set_fields_[124]) {
    SerializeField<logs::MinMsgIntervalLessThanThreshold>(writer, /*tag=*/ 119, min_msg_interval_less_than_threshold_);
  }
  if (set_fields_[125]) {
    SerializeField<logs::FailedToCleanupRosWarning>(writer, /*tag=*/ 120, failed_to_cleanup_ros_warning_);
  }
  if (set_fields_[126]) {
    SerializeField<logs::RpcDisconnectedWarning>(writer, /*tag=*/ 121, rpc_disconnected_warning_);
  }
  if (set_fields_[127]) {
    SerializeField<logs::RpcUnhandledError>(writer, /*tag=*/ 122, rpc_unhandled_error_);
  }
  if (set_fields_[128]) {
    SerializeField<logs::TimeDiffOutOfRangeWarning>(writer, /*tag=*/ 123, time_diff_out_of_range_warning_);
  }
  if (set_fields_[129]) {
    SerializeField<logs::TensorrtLog>(writer, /*tag=*/ 124, tensorrt_log_);
  }
  if (set_fields_[130]) {
    SerializeField<logs::BuildingTensorrtEngineInfo>(writer, /*tag=*/ 125, building_tensorrt_engine_info_);
  }
  if (set_fields_[131]) {
    SerializeField<logs::LoadingTensorrtEngineInfo>(writer, /*tag=*/ 126, loading_tensorrt_engine_info_);
  }
  if (set_fields_[132]) {
    SerializeField<logs::CalibrationMapNotFound>(writer, /*tag=*/ 127, calibration_map_not_found_);
  }
  if (set_fields_[133]) {
    SerializeField<logs::CalibrationMapNotValid>(writer, /*tag=*/ 128, calibration_map_not_valid_);
  }
  if (set_fields_[134]) {
    SerializeField<logs::CalibrationMapPathAlreadyExists>(writer, /*tag=*/ 129, calibration_map_path_already_exists_);
  }
  if (set_fields_[135]) {
    SerializeField<logs::FailedToSaveCalibrationMap>(writer, /*tag=*/ 130, failed_to_save_calibration_map_);
  }
  if (set_fields_[136]) {
    SerializeField<logs::FailedToRemoveCalibrationMap>(writer, /*tag=*/ 131, failed_to_remove_calibration_map_);
  }
  if (set_fields_[137]) {
    SerializeField<logs::FailedToIterateInDirectory>(writer, /*tag=*/ 132, failed_to_iterate_in_directory_);
  }
  if (set_fields_[138]) {
    SerializeField<logs::MapBasedCalibrationWithoutMapLoading>(writer, /*tag=*/ 133, map_based_calibration_without_map_loading_);
  }
  if (set_fields_[139]) {
    SerializeField<logs::MapBasedCalibrationAlreadyRunning>(writer, /*tag=*/ 134, map_based_calibration_already_running_);
  }
  if (set_fields_[140]) {
    SerializeField<logs::CancelMapBasedCalibrationNotRunning>(writer, /*tag=*/ 135, cancel_map_based_calibration_not_running_);
  }
  if (set_fields_[141]) {
    SerializeField<logs::BagStreamNotFound>(writer, /*tag=*/ 136, bag_stream_not_found_);
  }
  if (set_fields_[142]) {
    SerializeField<logs::EvaluationBagStartedInfo>(writer, /*tag=*/ 137, evaluation_bag_started_info_);
  }
  if (set_fields_[143]) {
    SerializeField<logs::EvaluationBagFinishedInfo>(writer, /*tag=*/ 138, evaluation_bag_finished_info_);
  }
  if (set_fields_[144]) {
    SerializeField<logs::BagNotFound>(writer, /*tag=*/ 139, bag_not_found_);
  }
  if (set_fields_[145]) {
    SerializeField<logs::BuildingPipelineInfo>(writer, /*tag=*/ 140, building_pipeline_info_);
  }
  if (set_fields_[146]) {
    SerializeField<logs::BagIsNotEvaluation>(writer, /*tag=*/ 141, bag_is_not_evaluation_);
  }
  if (set_fields_[147]) {
    SerializeField<logs::HorusBagRunning>(writer, /*tag=*/ 142, horus_bag_running_);
  }
  if (set_fields_[148]) {
    SerializeField<logs::AutoGroundCalibrationWarning>(writer, /*tag=*/ 143, auto_ground_calibration_warning_);
  }
  if (set_fields_[149]) {
    SerializeField<logs::AutoGroundCalibrationError>(writer, /*tag=*/ 144, auto_ground_calibration_error_);
  }
  if (set_fields_[150]) {
    SerializeField<logs::ObjectDetectorNotLoadedWarning>(writer, /*tag=*/ 151, object_detector_not_loaded_warning_);
  }
  if (set_fields_[151]) {
    SerializeField<logs::CalibrationIsRunningError>(writer, /*tag=*/ 152, calibration_is_running_error_);
  }
  if (set_fields_[152]) {
    SerializeField<logs::ModelInitInProgressInfo>(writer, /*tag=*/ 153, model_init_in_progress_info_);
  }
  if (set_fields_[153]) {
    SerializeField<logs::RpcTimeoutWithResolutionWarning>(writer, /*tag=*/ 154, rpc_timeout_with_resolution_warning_);
  }
  if (set_fields_[154]) {
    SerializeField<logs::CalibrationWasCancelledInfo>(writer, /*tag=*/ 155, calibration_was_cancelled_info_);
  }
  if (set_fields_[155]) {
    SerializeField<logs::CalibrationMapRecordingFailedToStart>(writer, /*tag=*/ 156, calibration_map_recording_failed_to_start_);
  }
  if (set_fields_[156]) {
    SerializeField<logs::DetectionPipelineRequestedResetInfo>(writer, /*tag=*/ 157, detection_pipeline_requested_reset_info_);
  }
  if (set_fields_[157]) {
    SerializeField<logs::PreprocessingServicePipelineUnavailable>(writer, /*tag=*/ 158, preprocessing_service_pipeline_unavailable_);
  }
  if (set_fields_[158]) {
    SerializeField<logs::CircularRecordingDisabledWarning>(writer, /*tag=*/ 159, circular_recording_disabled_warning_);
  }
  if (set_fields_[159]) {
    SerializeField<logs::SnapshotAlreadyRunningWarning>(writer, /*tag=*/ 160, snapshot_already_running_warning_);
  }
  if (set_fields_[160]) {
    SerializeField<logs::ActiveProjectChangedInfo>(writer, /*tag=*/ 161, active_project_changed_info_);
  }
  if (set_fields_[161]) {
    SerializeField<logs::ProjectConfigUpdatedInfo>(writer, /*tag=*/ 162, project_config_updated_info_);
  }
  if (set_fields_[162]) {
    SerializeField<logs::InvalidLidarTimestamp>(writer, /*tag=*/ 163, invalid_lidar_timestamp_);
  }
  if (set_fields_[163]) {
    SerializeField<logs::CalibrationAccumulatingPointsInfo>(writer, /*tag=*/ 164, calibration_accumulating_points_info_);
  }
  if (set_fields_[164]) {
    SerializeField<logs::SparseNoiseFilterUsageNonRotationalLidars>(writer, /*tag=*/ 165, sparse_noise_filter_usage_non_rotational_lidars_);
  }
  if (set_fields_[165]) {
    SerializeField<logs::FileWriteWarning>(writer, /*tag=*/ 166, file_write_warning_);
  }
  if (set_fields_[166]) {
    SerializeField<logs::LicenseForbiddenFeature>(writer, /*tag=*/ 167, license_forbidden_feature_);
  }
  if (set_fields_[167]) {
    SerializeField<logs::FailedToUpdateConfiguration>(writer, /*tag=*/ 168, failed_to_update_configuration_);
  }
  if (set_fields_[168]) {
    SerializeField<logs::ObstructionDetectorBadReferenceWarning>(writer, /*tag=*/ 169, obstruction_detector_bad_reference_warning_);
  }
  if (set_fields_[169]) {
    SerializeField<logs::ProjectFileInvalidPermissionsError>(writer, /*tag=*/ 170, project_file_invalid_permissions_error_);
  }
  if (set_fields_[170]) {
    SerializeField<logs::PipelineSchedulerError>(writer, /*tag=*/ 171, pipeline_scheduler_error_);
  }
  if (set_fields_[171]) {
    SerializeField<logs::MultiLidarCalibrationWarning>(writer, /*tag=*/ 172, multi_lidar_calibration_warning_);
  }
  if (set_fields_[172]) {
    SerializeField<logs::MultiLidarCalibrationError>(writer, /*tag=*/ 173, multi_lidar_calibration_error_);
  }
  if (set_fields_[173]) {
    SerializeField<logs::OldBagWarning>(writer, /*tag=*/ 174, old_bag_warning_);
  }
  if (set_fields_[174]) {
    SerializeField<logs::UpgradingBagInfo>(writer, /*tag=*/ 175, upgrading_bag_info_);
  }
  if (set_fields_[175]) {
    SerializeField<logs::BagCalibrationSaveFailed>(writer, /*tag=*/ 176, bag_calibration_save_failed_);
  }
  if (set_fields_[176]) {
    SerializeField<logs::BagUpgradeFailed>(writer, /*tag=*/ 177, bag_upgrade_failed_);
  }
  if (set_fields_[177]) {
    SerializeField<logs::UnknownLidarError>(writer, /*tag=*/ 178, unknown_lidar_error_);
  }
  if (set_fields_[178]) {
    SerializeField<logs::InvalidPointCloudWarning>(writer, /*tag=*/ 179, invalid_point_cloud_warning_);
  }
  if (set_fields_[179]) {
    SerializeField<logs::LidarIsDroppingPackets>(writer, /*tag=*/ 180, lidar_is_dropping_packets_);
  }
  if (set_fields_[180]) {
    SerializeField<logs::RemovedInvalidLidarsFromConfigWarning>(writer, /*tag=*/ 181, removed_invalid_lidars_from_config_warning_);
  }
  if (set_fields_[181]) {
    SerializeField<logs::CalibrationMapRecordingFailed>(writer, /*tag=*/ 182, calibration_map_recording_failed_);
  }
  if (set_fields_[182]) {
    SerializeField<logs::DetectionNodeNotFoundError>(writer, /*tag=*/ 183, detection_node_not_found_error_);
  }
  if (set_fields_[183]) {
    SerializeField<logs::CreatedVersionBackupInfo>(writer, /*tag=*/ 184, created_version_backup_info_);
  }
  if (set_fields_[184]) {
    SerializeField<logs::PlyFileLoadFailedError>(writer, /*tag=*/ 185, ply_file_load_failed_error_);
  }
  if (set_fields_[185]) {
    SerializeField<logs::HesaiDriverLifecycle>(writer, /*tag=*/ 186, hesai_driver_lifecycle_);
  }
  if (set_fields_[186]) {
    SerializeField<logs::HesaiDriverError>(writer, /*tag=*/ 187, hesai_driver_error_);
  }
  if (set_fields_[187]) {
    SerializeField<logs::HesaiPacketProcessingFailed>(writer, /*tag=*/ 188, hesai_packet_processing_failed_);
  }
  if (set_fields_[188]) {
    SerializeField<logs::HesaiCorrectionFileError>(writer, /*tag=*/ 189, hesai_correction_file_error_);
  }
  if (set_fields_[189]) {
    SerializeField<logs::HesaiPacketStatistics>(writer, /*tag=*/ 190, hesai_packet_statistics_);
  }
  if (set_fields_[190]) {
    SerializeField<logs::PlyFileWriteFailedError>(writer, /*tag=*/ 191, ply_file_write_failed_error_);
  }
  if (set_fields_[191]) {
    SerializeField<logs::ProjectSaveError>(writer, /*tag=*/ 192, project_save_error_);
  }
  if (set_fields_[192]) {
    SerializeField<logs::SaveStaticEnvironmentSuccess>(writer, /*tag=*/ 193, save_static_environment_success_);
  }
  if (set_fields_[193]) {
    SerializeField<logs::SaveStaticEnvironmentFailed>(writer, /*tag=*/ 194, save_static_environment_failed_);
  }
  if (set_fields_[194]) {
    SerializeField<logs::LoadStaticEnvironmentSuccess>(writer, /*tag=*/ 195, load_static_environment_success_);
  }
  if (set_fields_[195]) {
    SerializeField<logs::LoadStaticEnvironmentFailed>(writer, /*tag=*/ 196, load_static_environment_failed_);
  }
  if (set_fields_[196]) {
    SerializeField<logs::AttemptToInjectInvalidLidarIdWarning>(writer, /*tag=*/ 197, attempt_to_inject_invalid_lidar_id_warning_);
  }
  if (set_fields_[197]) {
    SerializeField<logs::ResetBundledPacketDueToUnexpectedPacket>(writer, /*tag=*/ 198, reset_bundled_packet_due_to_unexpected_packet_);
  }
  if (set_fields_[198]) {
    SerializeField<logs::PacketBundlerDroppedPacketsWarning>(writer, /*tag=*/ 199, packet_bundler_dropped_packets_warning_);
  }
  if (set_fields_[199]) {
    SerializeField<logs::PacketBundlerFrameJumpWarning>(writer, /*tag=*/ 200, packet_bundler_frame_jump_warning_);
  }
  if (set_fields_[200]) {
    SerializeField<logs::LidarCorrectionLoadingSuccess>(writer, /*tag=*/ 201, lidar_correction_loading_success_);
  }
  if (set_fields_[201]) {
    SerializeField<logs::LidarCorrectionLoadingFailure>(writer, /*tag=*/ 202, lidar_correction_loading_failure_);
  }
  if (set_fields_[202]) {
    SerializeField<logs::HesaiPacketStatisticsLidar>(writer, /*tag=*/ 203, hesai_packet_statistics_lidar_);
  }
  if (set_fields_[203]) {
    SerializeField<logs::LidarTiltDetectionAlignedToCalibrationMapInfo>(writer, /*tag=*/ 204, lidar_tilt_detection_aligned_to_calibration_map_info_);
  }
  if (set_fields_[204]) {
    SerializeField<logs::LidarTiltDetectionMisalignedToCalibrationMapWarning>(writer, /*tag=*/ 205, lidar_tilt_detection_misaligned_to_calibration_map_warning_);
  }
}

void LogData::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        clear_data();
        data_ = DataOneof::kGeneric;
        DeserializeField<logs::Generic>(reader, generic_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        clear_data();
        data_ = DataOneof::kOom;
        DeserializeField<logs::Oom>(reader, oom_);
        set_fields_[1] = true;
        break;
      }
      case 3: {
        clear_data();
        data_ = DataOneof::kRpcConnectionError;
        DeserializeField<logs::RpcConnectionError>(reader, rpc_connection_error_);
        set_fields_[2] = true;
        break;
      }
      case 4: {
        clear_data();
        data_ = DataOneof::kInvalidProjectName;
        DeserializeField<logs::InvalidProjectName>(reader, invalid_project_name_);
        set_fields_[3] = true;
        break;
      }
      case 5: {
        clear_data();
        data_ = DataOneof::kProjectNotFound;
        DeserializeField<logs::ProjectNotFound>(reader, project_not_found_);
        set_fields_[4] = true;
        break;
      }
      case 6: {
        clear_data();
        data_ = DataOneof::kProjectAlreadyExists;
        DeserializeField<logs::ProjectAlreadyExists>(reader, project_already_exists_);
        set_fields_[5] = true;
        break;
      }
      case 7: {
        clear_data();
        data_ = DataOneof::kInvalidConfiguration;
        DeserializeField<logs::InvalidConfiguration>(reader, invalid_configuration_);
        set_fields_[6] = true;
        break;
      }
      case 8: {
        clear_data();
        data_ = DataOneof::kEntityNotFound;
        DeserializeField<logs::EntityNotFound>(reader, entity_not_found_);
        set_fields_[7] = true;
        break;
      }
      case 9: {
        clear_data();
        data_ = DataOneof::kActiveProjectCannotBeDeleted;
        DeserializeField<logs::ActiveProjectCannotBeDeleted>(reader, active_project_cannot_be_deleted_);
        set_fields_[8] = true;
        break;
      }
      case 10: {
        clear_data();
        data_ = DataOneof::kRpcDisconnectionError;
        DeserializeField<logs::RpcDisconnectionError>(reader, rpc_disconnection_error_);
        set_fields_[9] = true;
        break;
      }
      case 11: {
        clear_data();
        data_ = DataOneof::kDroppedLogs;
        DeserializeField<logs::DroppedLogs>(reader, dropped_logs_);
        set_fields_[10] = true;
        break;
      }
      case 12: {
        clear_data();
        data_ = DataOneof::kOpenedProject;
        DeserializeField<logs::OpenedProject>(reader, opened_project_);
        set_fields_[11] = true;
        break;
      }
      case 13: {
        clear_data();
        data_ = DataOneof::kCreatedProject;
        DeserializeField<logs::CreatedProject>(reader, created_project_);
        set_fields_[12] = true;
        break;
      }
      case 14: {
        clear_data();
        data_ = DataOneof::kConfigUnavailable;
        DeserializeField<logs::ConfigUnavailable>(reader, config_unavailable_);
        set_fields_[13] = true;
        break;
      }
      case 15: {
        clear_data();
        data_ = DataOneof::kInvalidRequest;
        DeserializeField<logs::InvalidRequest>(reader, invalid_request_);
        set_fields_[14] = true;
        break;
      }
      case 16: {
        clear_data();
        data_ = DataOneof::kSanityCheckError;
        DeserializeField<logs::SanityCheckError>(reader, sanity_check_error_);
        set_fields_[15] = true;
        break;
      }
      case 17: {
        clear_data();
        data_ = DataOneof::kBagFailedToOpen;
        DeserializeField<logs::BagFailedToOpen>(reader, bag_failed_to_open_);
        set_fields_[16] = true;
        break;
      }
      case 18: {
        clear_data();
        data_ = DataOneof::kBagFailedToClose;
        DeserializeField<logs::BagFailedToClose>(reader, bag_failed_to_close_);
        set_fields_[17] = true;
        break;
      }
      case 19: {
        clear_data();
        data_ = DataOneof::kBagConversionFailed;
        DeserializeField<logs::BagConversionFailed>(reader, bag_conversion_failed_);
        set_fields_[18] = true;
        break;
      }
      case 20: {
        clear_data();
        data_ = DataOneof::kBagFailedToWrite;
        DeserializeField<logs::BagFailedToWrite>(reader, bag_failed_to_write_);
        set_fields_[19] = true;
        break;
      }
      case 21: {
        clear_data();
        data_ = DataOneof::kCalibrationError;
        DeserializeField<logs::CalibrationError>(reader, calibration_error_);
        set_fields_[20] = true;
        break;
      }
      case 22: {
        clear_data();
        data_ = DataOneof::kProjectManagerFailedToStartRecording;
        DeserializeField<logs::ProjectManagerFailedToStartRecording>(reader, project_manager_failed_to_start_recording_);
        set_fields_[21] = true;
        break;
      }
      case 23: {
        clear_data();
        data_ = DataOneof::kProjectManagerFailedToStopRecording;
        DeserializeField<logs::ProjectManagerFailedToStopRecording>(reader, project_manager_failed_to_stop_recording_);
        set_fields_[22] = true;
        break;
      }
      case 24: {
        clear_data();
        data_ = DataOneof::kServiceConnectionTimedOut;
        DeserializeField<logs::ServiceConnectionTimedOut>(reader, service_connection_timed_out_);
        set_fields_[23] = true;
        break;
      }
      case 25: {
        clear_data();
        data_ = DataOneof::kBagRecorderAlreadyRunning;
        DeserializeField<logs::BagRecorderAlreadyRunning>(reader, bag_recorder_already_running_);
        set_fields_[24] = true;
        break;
      }
      case 26: {
        clear_data();
        data_ = DataOneof::kLicenseServerConnectionError;
        DeserializeField<logs::LicenseServerConnectionError>(reader, license_server_connection_error_);
        set_fields_[25] = true;
        break;
      }
      case 27: {
        clear_data();
        data_ = DataOneof::kLicenseError;
        DeserializeField<logs::LicenseError>(reader, license_error_);
        set_fields_[26] = true;
        break;
      }
      case 145: {
        clear_data();
        data_ = DataOneof::kLicenseNotFoundError;
        DeserializeField<logs::LicenseNotFoundError>(reader, license_not_found_error_);
        set_fields_[27] = true;
        break;
      }
      case 146: {
        clear_data();
        data_ = DataOneof::kLicenseExpiredError;
        DeserializeField<logs::LicenseExpiredError>(reader, license_expired_error_);
        set_fields_[28] = true;
        break;
      }
      case 147: {
        clear_data();
        data_ = DataOneof::kLicenseExceededError;
        DeserializeField<logs::LicenseExceededError>(reader, license_exceeded_error_);
        set_fields_[29] = true;
        break;
      }
      case 28: {
        clear_data();
        data_ = DataOneof::kLicenseHostMachineError;
        DeserializeField<logs::LicenseHostMachineError>(reader, license_host_machine_error_);
        set_fields_[30] = true;
        break;
      }
      case 148: {
        clear_data();
        data_ = DataOneof::kLicensePrivilegeError;
        DeserializeField<logs::LicensePrivilegeError>(reader, license_privilege_error_);
        set_fields_[31] = true;
        break;
      }
      case 149: {
        clear_data();
        data_ = DataOneof::kLicenseActiveInfo;
        DeserializeField<logs::LicenseActiveInfo>(reader, license_active_info_);
        set_fields_[32] = true;
        break;
      }
      case 150: {
        clear_data();
        data_ = DataOneof::kMultipleLicensesWarning;
        DeserializeField<logs::MultipleLicensesWarning>(reader, multiple_licenses_warning_);
        set_fields_[33] = true;
        break;
      }
      case 29: {
        clear_data();
        data_ = DataOneof::kLicenseCurrentLicenseStatusInfo;
        DeserializeField<logs::LicenseCurrentLicenseStatusInfo>(reader, license_current_license_status_info_);
        set_fields_[34] = true;
        break;
      }
      case 30: {
        clear_data();
        data_ = DataOneof::kBagRecordingStoppedInfo;
        DeserializeField<logs::BagRecordingStoppedInfo>(reader, bag_recording_stopped_info_);
        set_fields_[35] = true;
        break;
      }
      case 31: {
        clear_data();
        data_ = DataOneof::kBagRecordingFailedToStart;
        DeserializeField<logs::BagRecordingFailedToStart>(reader, bag_recording_failed_to_start_);
        set_fields_[36] = true;
        break;
      }
      case 32: {
        clear_data();
        data_ = DataOneof::kBagRecordingStartedInfo;
        DeserializeField<logs::BagRecordingStartedInfo>(reader, bag_recording_started_info_);
        set_fields_[37] = true;
        break;
      }
      case 33: {
        clear_data();
        data_ = DataOneof::kReplayRestartedInfo;
        DeserializeField<logs::ReplayRestartedInfo>(reader, replay_restarted_info_);
        set_fields_[38] = true;
        break;
      }
      case 34: {
        clear_data();
        data_ = DataOneof::kInputSourceChangeRequestedInfo;
        DeserializeField<logs::InputSourceChangeRequestedInfo>(reader, input_source_change_requested_info_);
        set_fields_[39] = true;
        break;
      }
      case 35: {
        clear_data();
        data_ = DataOneof::kInputSourceSwitchedInfo;
        DeserializeField<logs::InputSourceSwitchedInfo>(reader, input_source_switched_info_);
        set_fields_[40] = true;
        break;
      }
      case 36: {
        clear_data();
        data_ = DataOneof::kRpcTimeoutWarning;
        DeserializeField<logs::RpcTimeoutWarning>(reader, rpc_timeout_warning_);
        set_fields_[41] = true;
        break;
      }
      case 37: {
        clear_data();
        data_ = DataOneof::kCannotWriteLogFile;
        DeserializeField<logs::CannotWriteLogFile>(reader, cannot_write_log_file_);
        set_fields_[42] = true;
        break;
      }
      case 38: {
        clear_data();
        data_ = DataOneof::kPointCloudParsingFailureWarning;
        DeserializeField<logs::PointCloudParsingFailureWarning>(reader, point_cloud_parsing_failure_warning_);
        set_fields_[43] = true;
        break;
      }
      case 39: {
        clear_data();
        data_ = DataOneof::kLidarIsDead;
        DeserializeField<logs::LidarIsDead>(reader, lidar_is_dead_);
        set_fields_[44] = true;
        break;
      }
      case 40: {
        clear_data();
        data_ = DataOneof::kLidarIsNotDeadAnymore;
        DeserializeField<logs::LidarIsNotDeadAnymore>(reader, lidar_is_not_dead_anymore_);
        set_fields_[45] = true;
        break;
      }
      case 41: {
        clear_data();
        data_ = DataOneof::kLidarIsObstructed;
        DeserializeField<logs::LidarIsObstructed>(reader, lidar_is_obstructed_);
        set_fields_[46] = true;
        break;
      }
      case 42: {
        clear_data();
        data_ = DataOneof::kLidarIsNotObstructedAnymore;
        DeserializeField<logs::LidarIsNotObstructedAnymore>(reader, lidar_is_not_obstructed_anymore_);
        set_fields_[47] = true;
        break;
      }
      case 43: {
        clear_data();
        data_ = DataOneof::kLidarIsTilted;
        DeserializeField<logs::LidarIsTilted>(reader, lidar_is_tilted_);
        set_fields_[48] = true;
        break;
      }
      case 44: {
        clear_data();
        data_ = DataOneof::kLidarIsNotTiltedAnymore;
        DeserializeField<logs::LidarIsNotTiltedAnymore>(reader, lidar_is_not_tilted_anymore_);
        set_fields_[49] = true;
        break;
      }
      case 45: {
        clear_data();
        data_ = DataOneof::kLidarHasBeenAutomaticallyRecalibrated;
        DeserializeField<logs::LidarHasBeenAutomaticallyRecalibrated>(reader, lidar_has_been_automatically_recalibrated_);
        set_fields_[50] = true;
        break;
      }
      case 46: {
        clear_data();
        data_ = DataOneof::kReceivedFirstDataForLidar;
        DeserializeField<logs::ReceivedFirstDataForLidar>(reader, received_first_data_for_lidar_);
        set_fields_[51] = true;
        break;
      }
      case 47: {
        clear_data();
        data_ = DataOneof::kTerminationFailureError;
        DeserializeField<logs::TerminationFailureError>(reader, termination_failure_error_);
        set_fields_[52] = true;
        break;
      }
      case 48: {
        clear_data();
        data_ = DataOneof::kFrameProcessingError;
        DeserializeField<logs::FrameProcessingError>(reader, frame_processing_error_);
        set_fields_[53] = true;
        break;
      }
      case 49: {
        clear_data();
        data_ = DataOneof::kThreadPoolUnavailableError;
        DeserializeField<logs::ThreadPoolUnavailableError>(reader, thread_pool_unavailable_error_);
        set_fields_[54] = true;
        break;
      }
      case 50: {
        clear_data();
        data_ = DataOneof::kInvalidArgument;
        DeserializeField<logs::InvalidArgument>(reader, invalid_argument_);
        set_fields_[55] = true;
        break;
      }
      case 51: {
        clear_data();
        data_ = DataOneof::kComponentInitializationFailureFatal;
        DeserializeField<logs::ComponentInitializationFailureFatal>(reader, component_initialization_failure_fatal_);
        set_fields_[56] = true;
        break;
      }
      case 52: {
        clear_data();
        data_ = DataOneof::kUnhandledEnumCaseError;
        DeserializeField<logs::UnhandledEnumCaseError>(reader, unhandled_enum_case_error_);
        set_fields_[57] = true;
        break;
      }
      case 53: {
        clear_data();
        data_ = DataOneof::kBagEmptyError;
        DeserializeField<logs::BagEmptyError>(reader, bag_empty_error_);
        set_fields_[58] = true;
        break;
      }
      case 54: {
        clear_data();
        data_ = DataOneof::kDiscardingDataError;
        DeserializeField<logs::DiscardingDataError>(reader, discarding_data_error_);
        set_fields_[59] = true;
        break;
      }
      case 55: {
        clear_data();
        data_ = DataOneof::kDiscardingDataWarning;
        DeserializeField<logs::DiscardingDataWarning>(reader, discarding_data_warning_);
        set_fields_[60] = true;
        break;
      }
      case 56: {
        clear_data();
        data_ = DataOneof::kNothingToProcess;
        DeserializeField<logs::NothingToProcess>(reader, nothing_to_process_);
        set_fields_[61] = true;
        break;
      }
      case 57: {
        clear_data();
        data_ = DataOneof::kInvalidComponentConfiguration;
        DeserializeField<logs::InvalidComponentConfiguration>(reader, invalid_component_configuration_);
        set_fields_[62] = true;
        break;
      }
      case 58: {
        clear_data();
        data_ = DataOneof::kChannelReceiverNotFoundWarning;
        DeserializeField<logs::ChannelReceiverNotFoundWarning>(reader, channel_receiver_not_found_warning_);
        set_fields_[63] = true;
        break;
      }
      case 59: {
        clear_data();
        data_ = DataOneof::kModelLoadFailure;
        DeserializeField<logs::ModelLoadFailure>(reader, model_load_failure_);
        set_fields_[64] = true;
        break;
      }
      case 60: {
        clear_data();
        data_ = DataOneof::kModelExecutionFailureError;
        DeserializeField<logs::ModelExecutionFailureError>(reader, model_execution_failure_error_);
        set_fields_[65] = true;
        break;
      }
      case 61: {
        clear_data();
        data_ = DataOneof::kServiceReadyInfo;
        DeserializeField<logs::ServiceReadyInfo>(reader, service_ready_info_);
        set_fields_[66] = true;
        break;
      }
      case 62: {
        clear_data();
        data_ = DataOneof::kModelPreparingInfo;
        DeserializeField<logs::ModelPreparingInfo>(reader, model_preparing_info_);
        set_fields_[67] = true;
        break;
      }
      case 63: {
        clear_data();
        data_ = DataOneof::kModelInitializedInfo;
        DeserializeField<logs::ModelInitializedInfo>(reader, model_initialized_info_);
        set_fields_[68] = true;
        break;
      }
      case 64: {
        clear_data();
        data_ = DataOneof::kModelInitializationFailureWarning;
        DeserializeField<logs::ModelInitializationFailureWarning>(reader, model_initialization_failure_warning_);
        set_fields_[69] = true;
        break;
      }
      case 65: {
        clear_data();
        data_ = DataOneof::kRosSpinnerStoppedWarning;
        DeserializeField<logs::RosSpinnerStoppedWarning>(reader, ros_spinner_stopped_warning_);
        set_fields_[70] = true;
        break;
      }
      case 66: {
        clear_data();
        data_ = DataOneof::kActorSystemUnavailableError;
        DeserializeField<logs::ActorSystemUnavailableError>(reader, actor_system_unavailable_error_);
        set_fields_[71] = true;
        break;
      }
      case 67: {
        clear_data();
        data_ = DataOneof::kConfigNodeNotFoundError;
        DeserializeField<logs::ConfigNodeNotFoundError>(reader, config_node_not_found_error_);
        set_fields_[72] = true;
        break;
      }
      case 68: {
        clear_data();
        data_ = DataOneof::kBagTimestampOutOfOrderError;
        DeserializeField<logs::BagTimestampOutOfOrderError>(reader, bag_timestamp_out_of_order_error_);
        set_fields_[73] = true;
        break;
      }
      case 69: {
        clear_data();
        data_ = DataOneof::kBagReplayUnexpectedTimestampError;
        DeserializeField<logs::BagReplayUnexpectedTimestampError>(reader, bag_replay_unexpected_timestamp_error_);
        set_fields_[74] = true;
        break;
      }
      case 70: {
        clear_data();
        data_ = DataOneof::kWebsocketClosedInfo;
        DeserializeField<logs::WebsocketClosedInfo>(reader, websocket_closed_info_);
        set_fields_[75] = true;
        break;
      }
      case 71: {
        clear_data();
        data_ = DataOneof::kWebsocketOpenedInfo;
        DeserializeField<logs::WebsocketOpenedInfo>(reader, websocket_opened_info_);
        set_fields_[76] = true;
        break;
      }
      case 72: {
        clear_data();
        data_ = DataOneof::kSubscriberDisconnectedInfo;
        DeserializeField<logs::SubscriberDisconnectedInfo>(reader, subscriber_disconnected_info_);
        set_fields_[77] = true;
        break;
      }
      case 73: {
        clear_data();
        data_ = DataOneof::kThreadPoolSlowingDownWarning;
        DeserializeField<logs::ThreadPoolSlowingDownWarning>(reader, thread_pool_slowing_down_warning_);
        set_fields_[78] = true;
        break;
      }
      case 74: {
        clear_data();
        data_ = DataOneof::kThreadPoolNotRespondingWarning;
        DeserializeField<logs::ThreadPoolNotRespondingWarning>(reader, thread_pool_not_responding_warning_);
        set_fields_[79] = true;
        break;
      }
      case 75: {
        clear_data();
        data_ = DataOneof::kThreadPoolBrokenPromiseWarning;
        DeserializeField<logs::ThreadPoolBrokenPromiseWarning>(reader, thread_pool_broken_promise_warning_);
        set_fields_[80] = true;
        break;
      }
      case 76: {
        clear_data();
        data_ = DataOneof::kBoxFittingPointsBelowBaseWarning;
        DeserializeField<logs::BoxFittingPointsBelowBaseWarning>(reader, box_fitting_points_below_base_warning_);
        set_fields_[81] = true;
        break;
      }
      case 77: {
        clear_data();
        data_ = DataOneof::kFailedToRemoveStalePointsWarning;
        DeserializeField<logs::FailedToRemoveStalePointsWarning>(reader, failed_to_remove_stale_points_warning_);
        set_fields_[82] = true;
        break;
      }
      case 78: {
        clear_data();
        data_ = DataOneof::kIrregularBroadcastingPeriodWarning;
        DeserializeField<logs::IrregularBroadcastingPeriodWarning>(reader, irregular_broadcasting_period_warning_);
        set_fields_[83] = true;
        break;
      }
      case 79: {
        clear_data();
        data_ = DataOneof::kClustererPointsOutOfRangeWarning;
        DeserializeField<logs::ClustererPointsOutOfRangeWarning>(reader, clusterer_points_out_of_range_warning_);
        set_fields_[84] = true;
        break;
      }
      case 80: {
        clear_data();
        data_ = DataOneof::kInternalError;
        DeserializeField<logs::InternalError>(reader, internal_error_);
        set_fields_[85] = true;
        break;
      }
      case 81: {
        clear_data();
        data_ = DataOneof::kInternalFatal;
        DeserializeField<logs::InternalFatal>(reader, internal_fatal_);
        set_fields_[86] = true;
        break;
      }
      case 82: {
        clear_data();
        data_ = DataOneof::kServiceStartingInfo;
        DeserializeField<logs::ServiceStartingInfo>(reader, service_starting_info_);
        set_fields_[87] = true;
        break;
      }
      case 83: {
        clear_data();
        data_ = DataOneof::kConfigNodeNotFoundFatal;
        DeserializeField<logs::ConfigNodeNotFoundFatal>(reader, config_node_not_found_fatal_);
        set_fields_[88] = true;
        break;
      }
      case 84: {
        clear_data();
        data_ = DataOneof::kServiceSetupError;
        DeserializeField<logs::ServiceSetupError>(reader, service_setup_error_);
        set_fields_[89] = true;
        break;
      }
      case 85: {
        clear_data();
        data_ = DataOneof::kFilesystemError;
        DeserializeField<logs::FilesystemError>(reader, filesystem_error_);
        set_fields_[90] = true;
        break;
      }
      case 86: {
        clear_data();
        data_ = DataOneof::kInvalidPresetWarning;
        DeserializeField<logs::InvalidPresetWarning>(reader, invalid_preset_warning_);
        set_fields_[91] = true;
        break;
      }
      case 87: {
        clear_data();
        data_ = DataOneof::kWebsocketFailedClearPendingError;
        DeserializeField<logs::WebsocketFailedClearPendingError>(reader, websocket_failed_clear_pending_error_);
        set_fields_[92] = true;
        break;
      }
      case 88: {
        clear_data();
        data_ = DataOneof::kWebsocketFailedToStopError;
        DeserializeField<logs::WebsocketFailedToStopError>(reader, websocket_failed_to_stop_error_);
        set_fields_[93] = true;
        break;
      }
      case 89: {
        clear_data();
        data_ = DataOneof::kWebsocketFailedToCreateError;
        DeserializeField<logs::WebsocketFailedToCreateError>(reader, websocket_failed_to_create_error_);
        set_fields_[94] = true;
        break;
      }
      case 90: {
        clear_data();
        data_ = DataOneof::kUnexpectedRpcError;
        DeserializeField<logs::UnexpectedRpcError>(reader, unexpected_rpc_error_);
        set_fields_[95] = true;
        break;
      }
      case 91: {
        clear_data();
        data_ = DataOneof::kLicensePollFailed;
        DeserializeField<logs::LicensePollFailed>(reader, license_poll_failed_);
        set_fields_[96] = true;
        break;
      }
      case 92: {
        clear_data();
        data_ = DataOneof::kLicenseExpiredWarning;
        DeserializeField<logs::LicenseExpiredWarning>(reader, license_expired_warning_);
        set_fields_[97] = true;
        break;
      }
      case 93: {
        clear_data();
        data_ = DataOneof::kLicenseUsageExceededWarning;
        DeserializeField<logs::LicenseUsageExceededWarning>(reader, license_usage_exceeded_warning_);
        set_fields_[98] = true;
        break;
      }
      case 94: {
        clear_data();
        data_ = DataOneof::kStaticThreadPoolSlowTaskWarning;
        DeserializeField<logs::StaticThreadPoolSlowTaskWarning>(reader, static_thread_pool_slow_task_warning_);
        set_fields_[99] = true;
        break;
      }
      case 95: {
        clear_data();
        data_ = DataOneof::kRpcUnsupportedServiceWarning;
        DeserializeField<logs::RpcUnsupportedServiceWarning>(reader, rpc_unsupported_service_warning_);
        set_fields_[100] = true;
        break;
      }
      case 96: {
        clear_data();
        data_ = DataOneof::kWebsocketHandlerProblem;
        DeserializeField<logs::WebsocketHandlerProblem>(reader, websocket_handler_problem_);
        set_fields_[101] = true;
        break;
      }
      case 97: {
        clear_data();
        data_ = DataOneof::kWebsocketDeserializeError;
        DeserializeField<logs::WebsocketDeserializeError>(reader, websocket_deserialize_error_);
        set_fields_[102] = true;
        break;
      }
      case 98: {
        clear_data();
        data_ = DataOneof::kWebsocketExpiredRpcEndpointError;
        DeserializeField<logs::WebsocketExpiredRpcEndpointError>(reader, websocket_expired_rpc_endpoint_error_);
        set_fields_[103] = true;
        break;
      }
      case 99: {
        clear_data();
        data_ = DataOneof::kWebsocketQueueOverloadedWarning;
        DeserializeField<logs::WebsocketQueueOverloadedWarning>(reader, websocket_queue_overloaded_warning_);
        set_fields_[104] = true;
        break;
      }
      case 100: {
        clear_data();
        data_ = DataOneof::kRpcFailedToNotifyWarning;
        DeserializeField<logs::RpcFailedToNotifyWarning>(reader, rpc_failed_to_notify_warning_);
        set_fields_[105] = true;
        break;
      }
      case 101: {
        clear_data();
        data_ = DataOneof::kConfigSubscriptionFailedWarning;
        DeserializeField<logs::ConfigSubscriptionFailedWarning>(reader, config_subscription_failed_warning_);
        set_fields_[106] = true;
        break;
      }
      case 102: {
        clear_data();
        data_ = DataOneof::kThreadPoolClampedWorkersWarning;
        DeserializeField<logs::ThreadPoolClampedWorkersWarning>(reader, thread_pool_clamped_workers_warning_);
        set_fields_[107] = true;
        break;
      }
      case 103: {
        clear_data();
        data_ = DataOneof::kStoppingHorusBagRecorderAlreadyStopped;
        DeserializeField<logs::StoppingHorusBagRecorderAlreadyStopped>(reader, stopping_horus_bag_recorder_already_stopped_);
        set_fields_[108] = true;
        break;
      }
      case 104: {
        clear_data();
        data_ = DataOneof::kRecorderConfigUpdateWhileRunning;
        DeserializeField<logs::RecorderConfigUpdateWhileRunning>(reader, recorder_config_update_while_running_);
        set_fields_[109] = true;
        break;
      }
      case 105: {
        clear_data();
        data_ = DataOneof::kClampingDataWarning;
        DeserializeField<logs::ClampingDataWarning>(reader, clamping_data_warning_);
        set_fields_[110] = true;
        break;
      }
      case 106: {
        clear_data();
        data_ = DataOneof::kLidarIncompatibleValues;
        DeserializeField<logs::LidarIncompatibleValues>(reader, lidar_incompatible_values_);
        set_fields_[111] = true;
        break;
      }
      case 107: {
        clear_data();
        data_ = DataOneof::kCannotDetermineContainerIdError;
        DeserializeField<logs::CannotDetermineContainerIdError>(reader, cannot_determine_container_id_error_);
        set_fields_[112] = true;
        break;
      }
      case 108: {
        clear_data();
        data_ = DataOneof::kStartedLidarDriver;
        DeserializeField<logs::StartedLidarDriver>(reader, started_lidar_driver_);
        set_fields_[113] = true;
        break;
      }
      case 109: {
        clear_data();
        data_ = DataOneof::kCannotStartLidarDriver;
        DeserializeField<logs::CannotStartLidarDriver>(reader, cannot_start_lidar_driver_);
        set_fields_[114] = true;
        break;
      }
      case 110: {
        clear_data();
        data_ = DataOneof::kStoppedLidarDriver;
        DeserializeField<logs::StoppedLidarDriver>(reader, stopped_lidar_driver_);
        set_fields_[115] = true;
        break;
      }
      case 111: {
        clear_data();
        data_ = DataOneof::kCannotStopLidarDriver;
        DeserializeField<logs::CannotStopLidarDriver>(reader, cannot_stop_lidar_driver_);
        set_fields_[116] = true;
        break;
      }
      case 112: {
        clear_data();
        data_ = DataOneof::kRestartedLidarDriver;
        DeserializeField<logs::RestartedLidarDriver>(reader, restarted_lidar_driver_);
        set_fields_[117] = true;
        break;
      }
      case 113: {
        clear_data();
        data_ = DataOneof::kCannotRestartLidarDriver;
        DeserializeField<logs::CannotRestartLidarDriver>(reader, cannot_restart_lidar_driver_);
        set_fields_[118] = true;
        break;
      }
      case 114: {
        clear_data();
        data_ = DataOneof::kRemovedUnusedLidarDriver;
        DeserializeField<logs::RemovedUnusedLidarDriver>(reader, removed_unused_lidar_driver_);
        set_fields_[119] = true;
        break;
      }
      case 115: {
        clear_data();
        data_ = DataOneof::kCannotRemoveUnusedLidarDriver;
        DeserializeField<logs::CannotRemoveUnusedLidarDriver>(reader, cannot_remove_unused_lidar_driver_);
        set_fields_[120] = true;
        break;
      }
      case 116: {
        clear_data();
        data_ = DataOneof::kLidarDriverGcFailure;
        DeserializeField<logs::LidarDriverGcFailure>(reader, lidar_driver_gc_failure_);
        set_fields_[121] = true;
        break;
      }
      case 117: {
        clear_data();
        data_ = DataOneof::kIdSpaceExhausted;
        DeserializeField<logs::IdSpaceExhausted>(reader, id_space_exhausted_);
        set_fields_[122] = true;
        break;
      }
      case 118: {
        clear_data();
        data_ = DataOneof::kPreprocessingToPointAggregatorPointsSkipped;
        DeserializeField<logs::PreprocessingToPointAggregatorPointsSkipped>(reader, preprocessing_to_point_aggregator_points_skipped_);
        set_fields_[123] = true;
        break;
      }
      case 119: {
        clear_data();
        data_ = DataOneof::kMinMsgIntervalLessThanThreshold;
        DeserializeField<logs::MinMsgIntervalLessThanThreshold>(reader, min_msg_interval_less_than_threshold_);
        set_fields_[124] = true;
        break;
      }
      case 120: {
        clear_data();
        data_ = DataOneof::kFailedToCleanupRosWarning;
        DeserializeField<logs::FailedToCleanupRosWarning>(reader, failed_to_cleanup_ros_warning_);
        set_fields_[125] = true;
        break;
      }
      case 121: {
        clear_data();
        data_ = DataOneof::kRpcDisconnectedWarning;
        DeserializeField<logs::RpcDisconnectedWarning>(reader, rpc_disconnected_warning_);
        set_fields_[126] = true;
        break;
      }
      case 122: {
        clear_data();
        data_ = DataOneof::kRpcUnhandledError;
        DeserializeField<logs::RpcUnhandledError>(reader, rpc_unhandled_error_);
        set_fields_[127] = true;
        break;
      }
      case 123: {
        clear_data();
        data_ = DataOneof::kTimeDiffOutOfRangeWarning;
        DeserializeField<logs::TimeDiffOutOfRangeWarning>(reader, time_diff_out_of_range_warning_);
        set_fields_[128] = true;
        break;
      }
      case 124: {
        clear_data();
        data_ = DataOneof::kTensorrtLog;
        DeserializeField<logs::TensorrtLog>(reader, tensorrt_log_);
        set_fields_[129] = true;
        break;
      }
      case 125: {
        clear_data();
        data_ = DataOneof::kBuildingTensorrtEngineInfo;
        DeserializeField<logs::BuildingTensorrtEngineInfo>(reader, building_tensorrt_engine_info_);
        set_fields_[130] = true;
        break;
      }
      case 126: {
        clear_data();
        data_ = DataOneof::kLoadingTensorrtEngineInfo;
        DeserializeField<logs::LoadingTensorrtEngineInfo>(reader, loading_tensorrt_engine_info_);
        set_fields_[131] = true;
        break;
      }
      case 127: {
        clear_data();
        data_ = DataOneof::kCalibrationMapNotFound;
        DeserializeField<logs::CalibrationMapNotFound>(reader, calibration_map_not_found_);
        set_fields_[132] = true;
        break;
      }
      case 128: {
        clear_data();
        data_ = DataOneof::kCalibrationMapNotValid;
        DeserializeField<logs::CalibrationMapNotValid>(reader, calibration_map_not_valid_);
        set_fields_[133] = true;
        break;
      }
      case 129: {
        clear_data();
        data_ = DataOneof::kCalibrationMapPathAlreadyExists;
        DeserializeField<logs::CalibrationMapPathAlreadyExists>(reader, calibration_map_path_already_exists_);
        set_fields_[134] = true;
        break;
      }
      case 130: {
        clear_data();
        data_ = DataOneof::kFailedToSaveCalibrationMap;
        DeserializeField<logs::FailedToSaveCalibrationMap>(reader, failed_to_save_calibration_map_);
        set_fields_[135] = true;
        break;
      }
      case 131: {
        clear_data();
        data_ = DataOneof::kFailedToRemoveCalibrationMap;
        DeserializeField<logs::FailedToRemoveCalibrationMap>(reader, failed_to_remove_calibration_map_);
        set_fields_[136] = true;
        break;
      }
      case 132: {
        clear_data();
        data_ = DataOneof::kFailedToIterateInDirectory;
        DeserializeField<logs::FailedToIterateInDirectory>(reader, failed_to_iterate_in_directory_);
        set_fields_[137] = true;
        break;
      }
      case 133: {
        clear_data();
        data_ = DataOneof::kMapBasedCalibrationWithoutMapLoading;
        DeserializeField<logs::MapBasedCalibrationWithoutMapLoading>(reader, map_based_calibration_without_map_loading_);
        set_fields_[138] = true;
        break;
      }
      case 134: {
        clear_data();
        data_ = DataOneof::kMapBasedCalibrationAlreadyRunning;
        DeserializeField<logs::MapBasedCalibrationAlreadyRunning>(reader, map_based_calibration_already_running_);
        set_fields_[139] = true;
        break;
      }
      case 135: {
        clear_data();
        data_ = DataOneof::kCancelMapBasedCalibrationNotRunning;
        DeserializeField<logs::CancelMapBasedCalibrationNotRunning>(reader, cancel_map_based_calibration_not_running_);
        set_fields_[140] = true;
        break;
      }
      case 136: {
        clear_data();
        data_ = DataOneof::kBagStreamNotFound;
        DeserializeField<logs::BagStreamNotFound>(reader, bag_stream_not_found_);
        set_fields_[141] = true;
        break;
      }
      case 137: {
        clear_data();
        data_ = DataOneof::kEvaluationBagStartedInfo;
        DeserializeField<logs::EvaluationBagStartedInfo>(reader, evaluation_bag_started_info_);
        set_fields_[142] = true;
        break;
      }
      case 138: {
        clear_data();
        data_ = DataOneof::kEvaluationBagFinishedInfo;
        DeserializeField<logs::EvaluationBagFinishedInfo>(reader, evaluation_bag_finished_info_);
        set_fields_[143] = true;
        break;
      }
      case 139: {
        clear_data();
        data_ = DataOneof::kBagNotFound;
        DeserializeField<logs::BagNotFound>(reader, bag_not_found_);
        set_fields_[144] = true;
        break;
      }
      case 140: {
        clear_data();
        data_ = DataOneof::kBuildingPipelineInfo;
        DeserializeField<logs::BuildingPipelineInfo>(reader, building_pipeline_info_);
        set_fields_[145] = true;
        break;
      }
      case 141: {
        clear_data();
        data_ = DataOneof::kBagIsNotEvaluation;
        DeserializeField<logs::BagIsNotEvaluation>(reader, bag_is_not_evaluation_);
        set_fields_[146] = true;
        break;
      }
      case 142: {
        clear_data();
        data_ = DataOneof::kHorusBagRunning;
        DeserializeField<logs::HorusBagRunning>(reader, horus_bag_running_);
        set_fields_[147] = true;
        break;
      }
      case 143: {
        clear_data();
        data_ = DataOneof::kAutoGroundCalibrationWarning;
        DeserializeField<logs::AutoGroundCalibrationWarning>(reader, auto_ground_calibration_warning_);
        set_fields_[148] = true;
        break;
      }
      case 144: {
        clear_data();
        data_ = DataOneof::kAutoGroundCalibrationError;
        DeserializeField<logs::AutoGroundCalibrationError>(reader, auto_ground_calibration_error_);
        set_fields_[149] = true;
        break;
      }
      case 151: {
        clear_data();
        data_ = DataOneof::kObjectDetectorNotLoadedWarning;
        DeserializeField<logs::ObjectDetectorNotLoadedWarning>(reader, object_detector_not_loaded_warning_);
        set_fields_[150] = true;
        break;
      }
      case 152: {
        clear_data();
        data_ = DataOneof::kCalibrationIsRunningError;
        DeserializeField<logs::CalibrationIsRunningError>(reader, calibration_is_running_error_);
        set_fields_[151] = true;
        break;
      }
      case 153: {
        clear_data();
        data_ = DataOneof::kModelInitInProgressInfo;
        DeserializeField<logs::ModelInitInProgressInfo>(reader, model_init_in_progress_info_);
        set_fields_[152] = true;
        break;
      }
      case 154: {
        clear_data();
        data_ = DataOneof::kRpcTimeoutWithResolutionWarning;
        DeserializeField<logs::RpcTimeoutWithResolutionWarning>(reader, rpc_timeout_with_resolution_warning_);
        set_fields_[153] = true;
        break;
      }
      case 155: {
        clear_data();
        data_ = DataOneof::kCalibrationWasCancelledInfo;
        DeserializeField<logs::CalibrationWasCancelledInfo>(reader, calibration_was_cancelled_info_);
        set_fields_[154] = true;
        break;
      }
      case 156: {
        clear_data();
        data_ = DataOneof::kCalibrationMapRecordingFailedToStart;
        DeserializeField<logs::CalibrationMapRecordingFailedToStart>(reader, calibration_map_recording_failed_to_start_);
        set_fields_[155] = true;
        break;
      }
      case 157: {
        clear_data();
        data_ = DataOneof::kDetectionPipelineRequestedResetInfo;
        DeserializeField<logs::DetectionPipelineRequestedResetInfo>(reader, detection_pipeline_requested_reset_info_);
        set_fields_[156] = true;
        break;
      }
      case 158: {
        clear_data();
        data_ = DataOneof::kPreprocessingServicePipelineUnavailable;
        DeserializeField<logs::PreprocessingServicePipelineUnavailable>(reader, preprocessing_service_pipeline_unavailable_);
        set_fields_[157] = true;
        break;
      }
      case 159: {
        clear_data();
        data_ = DataOneof::kCircularRecordingDisabledWarning;
        DeserializeField<logs::CircularRecordingDisabledWarning>(reader, circular_recording_disabled_warning_);
        set_fields_[158] = true;
        break;
      }
      case 160: {
        clear_data();
        data_ = DataOneof::kSnapshotAlreadyRunningWarning;
        DeserializeField<logs::SnapshotAlreadyRunningWarning>(reader, snapshot_already_running_warning_);
        set_fields_[159] = true;
        break;
      }
      case 161: {
        clear_data();
        data_ = DataOneof::kActiveProjectChangedInfo;
        DeserializeField<logs::ActiveProjectChangedInfo>(reader, active_project_changed_info_);
        set_fields_[160] = true;
        break;
      }
      case 162: {
        clear_data();
        data_ = DataOneof::kProjectConfigUpdatedInfo;
        DeserializeField<logs::ProjectConfigUpdatedInfo>(reader, project_config_updated_info_);
        set_fields_[161] = true;
        break;
      }
      case 163: {
        clear_data();
        data_ = DataOneof::kInvalidLidarTimestamp;
        DeserializeField<logs::InvalidLidarTimestamp>(reader, invalid_lidar_timestamp_);
        set_fields_[162] = true;
        break;
      }
      case 164: {
        clear_data();
        data_ = DataOneof::kCalibrationAccumulatingPointsInfo;
        DeserializeField<logs::CalibrationAccumulatingPointsInfo>(reader, calibration_accumulating_points_info_);
        set_fields_[163] = true;
        break;
      }
      case 165: {
        clear_data();
        data_ = DataOneof::kSparseNoiseFilterUsageNonRotationalLidars;
        DeserializeField<logs::SparseNoiseFilterUsageNonRotationalLidars>(reader, sparse_noise_filter_usage_non_rotational_lidars_);
        set_fields_[164] = true;
        break;
      }
      case 166: {
        clear_data();
        data_ = DataOneof::kFileWriteWarning;
        DeserializeField<logs::FileWriteWarning>(reader, file_write_warning_);
        set_fields_[165] = true;
        break;
      }
      case 167: {
        clear_data();
        data_ = DataOneof::kLicenseForbiddenFeature;
        DeserializeField<logs::LicenseForbiddenFeature>(reader, license_forbidden_feature_);
        set_fields_[166] = true;
        break;
      }
      case 168: {
        clear_data();
        data_ = DataOneof::kFailedToUpdateConfiguration;
        DeserializeField<logs::FailedToUpdateConfiguration>(reader, failed_to_update_configuration_);
        set_fields_[167] = true;
        break;
      }
      case 169: {
        clear_data();
        data_ = DataOneof::kObstructionDetectorBadReferenceWarning;
        DeserializeField<logs::ObstructionDetectorBadReferenceWarning>(reader, obstruction_detector_bad_reference_warning_);
        set_fields_[168] = true;
        break;
      }
      case 170: {
        clear_data();
        data_ = DataOneof::kProjectFileInvalidPermissionsError;
        DeserializeField<logs::ProjectFileInvalidPermissionsError>(reader, project_file_invalid_permissions_error_);
        set_fields_[169] = true;
        break;
      }
      case 171: {
        clear_data();
        data_ = DataOneof::kPipelineSchedulerError;
        DeserializeField<logs::PipelineSchedulerError>(reader, pipeline_scheduler_error_);
        set_fields_[170] = true;
        break;
      }
      case 172: {
        clear_data();
        data_ = DataOneof::kMultiLidarCalibrationWarning;
        DeserializeField<logs::MultiLidarCalibrationWarning>(reader, multi_lidar_calibration_warning_);
        set_fields_[171] = true;
        break;
      }
      case 173: {
        clear_data();
        data_ = DataOneof::kMultiLidarCalibrationError;
        DeserializeField<logs::MultiLidarCalibrationError>(reader, multi_lidar_calibration_error_);
        set_fields_[172] = true;
        break;
      }
      case 174: {
        clear_data();
        data_ = DataOneof::kOldBagWarning;
        DeserializeField<logs::OldBagWarning>(reader, old_bag_warning_);
        set_fields_[173] = true;
        break;
      }
      case 175: {
        clear_data();
        data_ = DataOneof::kUpgradingBagInfo;
        DeserializeField<logs::UpgradingBagInfo>(reader, upgrading_bag_info_);
        set_fields_[174] = true;
        break;
      }
      case 176: {
        clear_data();
        data_ = DataOneof::kBagCalibrationSaveFailed;
        DeserializeField<logs::BagCalibrationSaveFailed>(reader, bag_calibration_save_failed_);
        set_fields_[175] = true;
        break;
      }
      case 177: {
        clear_data();
        data_ = DataOneof::kBagUpgradeFailed;
        DeserializeField<logs::BagUpgradeFailed>(reader, bag_upgrade_failed_);
        set_fields_[176] = true;
        break;
      }
      case 178: {
        clear_data();
        data_ = DataOneof::kUnknownLidarError;
        DeserializeField<logs::UnknownLidarError>(reader, unknown_lidar_error_);
        set_fields_[177] = true;
        break;
      }
      case 179: {
        clear_data();
        data_ = DataOneof::kInvalidPointCloudWarning;
        DeserializeField<logs::InvalidPointCloudWarning>(reader, invalid_point_cloud_warning_);
        set_fields_[178] = true;
        break;
      }
      case 180: {
        clear_data();
        data_ = DataOneof::kLidarIsDroppingPackets;
        DeserializeField<logs::LidarIsDroppingPackets>(reader, lidar_is_dropping_packets_);
        set_fields_[179] = true;
        break;
      }
      case 181: {
        clear_data();
        data_ = DataOneof::kRemovedInvalidLidarsFromConfigWarning;
        DeserializeField<logs::RemovedInvalidLidarsFromConfigWarning>(reader, removed_invalid_lidars_from_config_warning_);
        set_fields_[180] = true;
        break;
      }
      case 182: {
        clear_data();
        data_ = DataOneof::kCalibrationMapRecordingFailed;
        DeserializeField<logs::CalibrationMapRecordingFailed>(reader, calibration_map_recording_failed_);
        set_fields_[181] = true;
        break;
      }
      case 183: {
        clear_data();
        data_ = DataOneof::kDetectionNodeNotFoundError;
        DeserializeField<logs::DetectionNodeNotFoundError>(reader, detection_node_not_found_error_);
        set_fields_[182] = true;
        break;
      }
      case 184: {
        clear_data();
        data_ = DataOneof::kCreatedVersionBackupInfo;
        DeserializeField<logs::CreatedVersionBackupInfo>(reader, created_version_backup_info_);
        set_fields_[183] = true;
        break;
      }
      case 185: {
        clear_data();
        data_ = DataOneof::kPlyFileLoadFailedError;
        DeserializeField<logs::PlyFileLoadFailedError>(reader, ply_file_load_failed_error_);
        set_fields_[184] = true;
        break;
      }
      case 186: {
        clear_data();
        data_ = DataOneof::kHesaiDriverLifecycle;
        DeserializeField<logs::HesaiDriverLifecycle>(reader, hesai_driver_lifecycle_);
        set_fields_[185] = true;
        break;
      }
      case 187: {
        clear_data();
        data_ = DataOneof::kHesaiDriverError;
        DeserializeField<logs::HesaiDriverError>(reader, hesai_driver_error_);
        set_fields_[186] = true;
        break;
      }
      case 188: {
        clear_data();
        data_ = DataOneof::kHesaiPacketProcessingFailed;
        DeserializeField<logs::HesaiPacketProcessingFailed>(reader, hesai_packet_processing_failed_);
        set_fields_[187] = true;
        break;
      }
      case 189: {
        clear_data();
        data_ = DataOneof::kHesaiCorrectionFileError;
        DeserializeField<logs::HesaiCorrectionFileError>(reader, hesai_correction_file_error_);
        set_fields_[188] = true;
        break;
      }
      case 190: {
        clear_data();
        data_ = DataOneof::kHesaiPacketStatistics;
        DeserializeField<logs::HesaiPacketStatistics>(reader, hesai_packet_statistics_);
        set_fields_[189] = true;
        break;
      }
      case 191: {
        clear_data();
        data_ = DataOneof::kPlyFileWriteFailedError;
        DeserializeField<logs::PlyFileWriteFailedError>(reader, ply_file_write_failed_error_);
        set_fields_[190] = true;
        break;
      }
      case 192: {
        clear_data();
        data_ = DataOneof::kProjectSaveError;
        DeserializeField<logs::ProjectSaveError>(reader, project_save_error_);
        set_fields_[191] = true;
        break;
      }
      case 193: {
        clear_data();
        data_ = DataOneof::kSaveStaticEnvironmentSuccess;
        DeserializeField<logs::SaveStaticEnvironmentSuccess>(reader, save_static_environment_success_);
        set_fields_[192] = true;
        break;
      }
      case 194: {
        clear_data();
        data_ = DataOneof::kSaveStaticEnvironmentFailed;
        DeserializeField<logs::SaveStaticEnvironmentFailed>(reader, save_static_environment_failed_);
        set_fields_[193] = true;
        break;
      }
      case 195: {
        clear_data();
        data_ = DataOneof::kLoadStaticEnvironmentSuccess;
        DeserializeField<logs::LoadStaticEnvironmentSuccess>(reader, load_static_environment_success_);
        set_fields_[194] = true;
        break;
      }
      case 196: {
        clear_data();
        data_ = DataOneof::kLoadStaticEnvironmentFailed;
        DeserializeField<logs::LoadStaticEnvironmentFailed>(reader, load_static_environment_failed_);
        set_fields_[195] = true;
        break;
      }
      case 197: {
        clear_data();
        data_ = DataOneof::kAttemptToInjectInvalidLidarIdWarning;
        DeserializeField<logs::AttemptToInjectInvalidLidarIdWarning>(reader, attempt_to_inject_invalid_lidar_id_warning_);
        set_fields_[196] = true;
        break;
      }
      case 198: {
        clear_data();
        data_ = DataOneof::kResetBundledPacketDueToUnexpectedPacket;
        DeserializeField<logs::ResetBundledPacketDueToUnexpectedPacket>(reader, reset_bundled_packet_due_to_unexpected_packet_);
        set_fields_[197] = true;
        break;
      }
      case 199: {
        clear_data();
        data_ = DataOneof::kPacketBundlerDroppedPacketsWarning;
        DeserializeField<logs::PacketBundlerDroppedPacketsWarning>(reader, packet_bundler_dropped_packets_warning_);
        set_fields_[198] = true;
        break;
      }
      case 200: {
        clear_data();
        data_ = DataOneof::kPacketBundlerFrameJumpWarning;
        DeserializeField<logs::PacketBundlerFrameJumpWarning>(reader, packet_bundler_frame_jump_warning_);
        set_fields_[199] = true;
        break;
      }
      case 201: {
        clear_data();
        data_ = DataOneof::kLidarCorrectionLoadingSuccess;
        DeserializeField<logs::LidarCorrectionLoadingSuccess>(reader, lidar_correction_loading_success_);
        set_fields_[200] = true;
        break;
      }
      case 202: {
        clear_data();
        data_ = DataOneof::kLidarCorrectionLoadingFailure;
        DeserializeField<logs::LidarCorrectionLoadingFailure>(reader, lidar_correction_loading_failure_);
        set_fields_[201] = true;
        break;
      }
      case 203: {
        clear_data();
        data_ = DataOneof::kHesaiPacketStatisticsLidar;
        DeserializeField<logs::HesaiPacketStatisticsLidar>(reader, hesai_packet_statistics_lidar_);
        set_fields_[202] = true;
        break;
      }
      case 204: {
        clear_data();
        data_ = DataOneof::kLidarTiltDetectionAlignedToCalibrationMapInfo;
        DeserializeField<logs::LidarTiltDetectionAlignedToCalibrationMapInfo>(reader, lidar_tilt_detection_aligned_to_calibration_map_info_);
        set_fields_[203] = true;
        break;
      }
      case 205: {
        clear_data();
        data_ = DataOneof::kLidarTiltDetectionMisalignedToCalibrationMapWarning;
        DeserializeField<logs::LidarTiltDetectionMisalignedToCalibrationMapWarning>(reader, lidar_tilt_detection_misaligned_to_calibration_map_warning_);
        set_fields_[204] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

LogMessage::LogMessage(const LogMessage& other) noexcept(false)
    : metadata_{other.metadata_}
    , data_{other.data_}
    , set_fields_{other.set_fields_} {}

void LogMessage::SerializeTo(PbWriter& writer) const noexcept(false) {
  if (set_fields_[0]) {
    SerializeField<LogMetadata>(writer, /*tag=*/ 1, metadata_);
  }
  if (set_fields_[1]) {
    SerializeField<LogData>(writer, /*tag=*/ 2, data_);
  }
}

void LogMessage::DeserializeFrom(PbReader& reader) noexcept(false) {
  while (reader.Reader().next()) {
    switch (reader.Reader().tag()) {
      case 1: {
        DeserializeField<LogMetadata>(reader, metadata_);
        set_fields_[0] = true;
        break;
      }
      case 2: {
        DeserializeField<LogData>(reader, data_);
        set_fields_[1] = true;
        break;
      }
      default: {
        reader.Reader().skip();
        break;
      }
    }
  }
}

}  // namespace pb
}  // namespace sdk
}  // namespace horus
