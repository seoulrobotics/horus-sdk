import typing
from ..pb.logs.message_pb2 import LogData
from ..pb.logs import logs_pb2 as _logs_pb

_formatters: typing.Dict[int, typing.Callable[[LogData], str]] = {
    LogData.GENERIC_FIELD_NUMBER: lambda m: format_generic(m.generic),
    LogData.OOM_FIELD_NUMBER: lambda m: format_oom(m.oom),
    LogData.RPC_CONNECTION_ERROR_FIELD_NUMBER: lambda m: format_rpc_connection_error(m.rpc_connection_error),
    LogData.INVALID_PROJECT_NAME_FIELD_NUMBER: lambda m: format_invalid_project_name(m.invalid_project_name),
    LogData.PROJECT_NOT_FOUND_FIELD_NUMBER: lambda m: format_project_not_found(m.project_not_found),
    LogData.PROJECT_ALREADY_EXISTS_FIELD_NUMBER: lambda m: format_project_already_exists(m.project_already_exists),
    LogData.INVALID_CONFIGURATION_FIELD_NUMBER: lambda m: format_invalid_configuration(m.invalid_configuration),
    LogData.ENTITY_NOT_FOUND_FIELD_NUMBER: lambda m: format_entity_not_found(m.entity_not_found),
    LogData.ACTIVE_PROJECT_CANNOT_BE_DELETED_FIELD_NUMBER: lambda m: format_active_project_cannot_be_deleted(m.active_project_cannot_be_deleted),
    LogData.RPC_DISCONNECTION_ERROR_FIELD_NUMBER: lambda m: format_rpc_disconnection_error(m.rpc_disconnection_error),
    LogData.DROPPED_LOGS_FIELD_NUMBER: lambda m: format_dropped_logs(m.dropped_logs),
    LogData.OPENED_PROJECT_FIELD_NUMBER: lambda m: format_opened_project(m.opened_project),
    LogData.CREATED_PROJECT_FIELD_NUMBER: lambda m: format_created_project(m.created_project),
    LogData.CONFIG_UNAVAILABLE_FIELD_NUMBER: lambda m: format_config_unavailable(m.config_unavailable),
    LogData.INVALID_REQUEST_FIELD_NUMBER: lambda m: format_invalid_request(m.invalid_request),
    LogData.SANITY_CHECK_ERROR_FIELD_NUMBER: lambda m: format_sanity_check_error(m.sanity_check_error),
    LogData.BAG_FAILED_TO_OPEN_FIELD_NUMBER: lambda m: format_bag_failed_to_open(m.bag_failed_to_open),
    LogData.BAG_FAILED_TO_CLOSE_FIELD_NUMBER: lambda m: format_bag_failed_to_close(m.bag_failed_to_close),
    LogData.BAG_CONVERSION_FAILED_FIELD_NUMBER: lambda m: format_bag_conversion_failed(m.bag_conversion_failed),
    LogData.BAG_FAILED_TO_WRITE_FIELD_NUMBER: lambda m: format_bag_failed_to_write(m.bag_failed_to_write),
    LogData.CALIBRATION_ERROR_FIELD_NUMBER: lambda m: format_calibration_error(m.calibration_error),
    LogData.PROJECT_MANAGER_FAILED_TO_START_RECORDING_FIELD_NUMBER: lambda m: format_project_manager_failed_to_start_recording(m.project_manager_failed_to_start_recording),
    LogData.PROJECT_MANAGER_FAILED_TO_STOP_RECORDING_FIELD_NUMBER: lambda m: format_project_manager_failed_to_stop_recording(m.project_manager_failed_to_stop_recording),
    LogData.SERVICE_CONNECTION_TIMED_OUT_FIELD_NUMBER: lambda m: format_service_connection_timed_out(m.service_connection_timed_out),
    LogData.BAG_RECORDER_ALREADY_RUNNING_FIELD_NUMBER: lambda m: format_bag_recorder_already_running(m.bag_recorder_already_running),
    LogData.LICENSE_SERVER_CONNECTION_ERROR_FIELD_NUMBER: lambda m: format_license_server_connection_error(m.license_server_connection_error),
    LogData.LICENSE_ERROR_FIELD_NUMBER: lambda m: format_license_error(m.license_error),
    LogData.LICENSE_NOT_FOUND_ERROR_FIELD_NUMBER: lambda m: format_license_not_found_error(m.license_not_found_error),
    LogData.LICENSE_EXPIRED_ERROR_FIELD_NUMBER: lambda m: format_license_expired_error(m.license_expired_error),
    LogData.LICENSE_EXCEEDED_ERROR_FIELD_NUMBER: lambda m: format_license_exceeded_error(m.license_exceeded_error),
    LogData.LICENSE_HOST_MACHINE_ERROR_FIELD_NUMBER: lambda m: format_license_host_machine_error(m.license_host_machine_error),
    LogData.LICENSE_PRIVILEGE_ERROR_FIELD_NUMBER: lambda m: format_license_privilege_error(m.license_privilege_error),
    LogData.LICENSE_ACTIVE_INFO_FIELD_NUMBER: lambda m: format_license_active_info(m.license_active_info),
    LogData.MULTIPLE_LICENSES_WARNING_FIELD_NUMBER: lambda m: format_multiple_licenses_warning(m.multiple_licenses_warning),
    LogData.LICENSE_CURRENT_LICENSE_STATUS_INFO_FIELD_NUMBER: lambda m: format_license_current_license_status_info(m.license_current_license_status_info),
    LogData.BAG_RECORDING_STOPPED_INFO_FIELD_NUMBER: lambda m: format_bag_recording_stopped_info(m.bag_recording_stopped_info),
    LogData.BAG_RECORDING_FAILED_TO_START_FIELD_NUMBER: lambda m: format_bag_recording_failed_to_start(m.bag_recording_failed_to_start),
    LogData.BAG_RECORDING_STARTED_INFO_FIELD_NUMBER: lambda m: format_bag_recording_started_info(m.bag_recording_started_info),
    LogData.REPLAY_RESTARTED_INFO_FIELD_NUMBER: lambda m: format_replay_restarted_info(m.replay_restarted_info),
    LogData.INPUT_SOURCE_CHANGE_REQUESTED_INFO_FIELD_NUMBER: lambda m: format_input_source_change_requested_info(m.input_source_change_requested_info),
    LogData.INPUT_SOURCE_SWITCHED_INFO_FIELD_NUMBER: lambda m: format_input_source_switched_info(m.input_source_switched_info),
    LogData.RPC_TIMEOUT_WARNING_FIELD_NUMBER: lambda m: format_rpc_timeout_warning(m.rpc_timeout_warning),
    LogData.CANNOT_WRITE_LOG_FILE_FIELD_NUMBER: lambda m: format_cannot_write_log_file(m.cannot_write_log_file),
    LogData.POINT_CLOUD_PARSING_FAILURE_WARNING_FIELD_NUMBER: lambda m: format_point_cloud_parsing_failure_warning(m.point_cloud_parsing_failure_warning),
    LogData.LIDAR_IS_DEAD_FIELD_NUMBER: lambda m: format_lidar_is_dead(m.lidar_is_dead),
    LogData.LIDAR_IS_NOT_DEAD_ANYMORE_FIELD_NUMBER: lambda m: format_lidar_is_not_dead_anymore(m.lidar_is_not_dead_anymore),
    LogData.LIDAR_IS_OBSTRUCTED_FIELD_NUMBER: lambda m: format_lidar_is_obstructed(m.lidar_is_obstructed),
    LogData.LIDAR_IS_NOT_OBSTRUCTED_ANYMORE_FIELD_NUMBER: lambda m: format_lidar_is_not_obstructed_anymore(m.lidar_is_not_obstructed_anymore),
    LogData.LIDAR_IS_TILTED_FIELD_NUMBER: lambda m: format_lidar_is_tilted(m.lidar_is_tilted),
    LogData.LIDAR_IS_NOT_TILTED_ANYMORE_FIELD_NUMBER: lambda m: format_lidar_is_not_tilted_anymore(m.lidar_is_not_tilted_anymore),
    LogData.LIDAR_HAS_BEEN_AUTOMATICALLY_RECALIBRATED_FIELD_NUMBER: lambda m: format_lidar_has_been_automatically_recalibrated(m.lidar_has_been_automatically_recalibrated),
    LogData.RECEIVED_FIRST_DATA_FOR_LIDAR_FIELD_NUMBER: lambda m: format_received_first_data_for_lidar(m.received_first_data_for_lidar),
    LogData.TERMINATION_FAILURE_ERROR_FIELD_NUMBER: lambda m: format_termination_failure_error(m.termination_failure_error),
    LogData.FRAME_PROCESSING_ERROR_FIELD_NUMBER: lambda m: format_frame_processing_error(m.frame_processing_error),
    LogData.THREAD_POOL_UNAVAILABLE_ERROR_FIELD_NUMBER: lambda m: format_thread_pool_unavailable_error(m.thread_pool_unavailable_error),
    LogData.INVALID_ARGUMENT_FIELD_NUMBER: lambda m: format_invalid_argument(m.invalid_argument),
    LogData.COMPONENT_INITIALIZATION_FAILURE_FATAL_FIELD_NUMBER: lambda m: format_component_initialization_failure_fatal(m.component_initialization_failure_fatal),
    LogData.UNHANDLED_ENUM_CASE_ERROR_FIELD_NUMBER: lambda m: format_unhandled_enum_case_error(m.unhandled_enum_case_error),
    LogData.BAG_EMPTY_ERROR_FIELD_NUMBER: lambda m: format_bag_empty_error(m.bag_empty_error),
    LogData.DISCARDING_DATA_ERROR_FIELD_NUMBER: lambda m: format_discarding_data_error(m.discarding_data_error),
    LogData.DISCARDING_DATA_WARNING_FIELD_NUMBER: lambda m: format_discarding_data_warning(m.discarding_data_warning),
    LogData.NOTHING_TO_PROCESS_FIELD_NUMBER: lambda m: format_nothing_to_process(m.nothing_to_process),
    LogData.INVALID_COMPONENT_CONFIGURATION_FIELD_NUMBER: lambda m: format_invalid_component_configuration(m.invalid_component_configuration),
    LogData.CHANNEL_RECEIVER_NOT_FOUND_WARNING_FIELD_NUMBER: lambda m: format_channel_receiver_not_found_warning(m.channel_receiver_not_found_warning),
    LogData.MODEL_LOAD_FAILURE_FIELD_NUMBER: lambda m: format_model_load_failure(m.model_load_failure),
    LogData.MODEL_EXECUTION_FAILURE_ERROR_FIELD_NUMBER: lambda m: format_model_execution_failure_error(m.model_execution_failure_error),
    LogData.SERVICE_READY_INFO_FIELD_NUMBER: lambda m: format_service_ready_info(m.service_ready_info),
    LogData.MODEL_PREPARING_INFO_FIELD_NUMBER: lambda m: format_model_preparing_info(m.model_preparing_info),
    LogData.MODEL_INITIALIZED_INFO_FIELD_NUMBER: lambda m: format_model_initialized_info(m.model_initialized_info),
    LogData.MODEL_INITIALIZATION_FAILURE_WARNING_FIELD_NUMBER: lambda m: format_model_initialization_failure_warning(m.model_initialization_failure_warning),
    LogData.ROS_SPINNER_STOPPED_WARNING_FIELD_NUMBER: lambda m: format_ros_spinner_stopped_warning(m.ros_spinner_stopped_warning),
    LogData.ACTOR_SYSTEM_UNAVAILABLE_ERROR_FIELD_NUMBER: lambda m: format_actor_system_unavailable_error(m.actor_system_unavailable_error),
    LogData.CONFIG_NODE_NOT_FOUND_ERROR_FIELD_NUMBER: lambda m: format_config_node_not_found_error(m.config_node_not_found_error),
    LogData.BAG_TIMESTAMP_OUT_OF_ORDER_ERROR_FIELD_NUMBER: lambda m: format_bag_timestamp_out_of_order_error(m.bag_timestamp_out_of_order_error),
    LogData.BAG_REPLAY_UNEXPECTED_TIMESTAMP_ERROR_FIELD_NUMBER: lambda m: format_bag_replay_unexpected_timestamp_error(m.bag_replay_unexpected_timestamp_error),
    LogData.WEBSOCKET_CLOSED_INFO_FIELD_NUMBER: lambda m: format_websocket_closed_info(m.websocket_closed_info),
    LogData.WEBSOCKET_OPENED_INFO_FIELD_NUMBER: lambda m: format_websocket_opened_info(m.websocket_opened_info),
    LogData.SUBSCRIBER_DISCONNECTED_INFO_FIELD_NUMBER: lambda m: format_subscriber_disconnected_info(m.subscriber_disconnected_info),
    LogData.THREAD_POOL_SLOWING_DOWN_WARNING_FIELD_NUMBER: lambda m: format_thread_pool_slowing_down_warning(m.thread_pool_slowing_down_warning),
    LogData.THREAD_POOL_NOT_RESPONDING_WARNING_FIELD_NUMBER: lambda m: format_thread_pool_not_responding_warning(m.thread_pool_not_responding_warning),
    LogData.THREAD_POOL_BROKEN_PROMISE_WARNING_FIELD_NUMBER: lambda m: format_thread_pool_broken_promise_warning(m.thread_pool_broken_promise_warning),
    LogData.BOX_FITTING_POINTS_BELOW_BASE_WARNING_FIELD_NUMBER: lambda m: format_box_fitting_points_below_base_warning(m.box_fitting_points_below_base_warning),
    LogData.FAILED_TO_REMOVE_STALE_POINTS_WARNING_FIELD_NUMBER: lambda m: format_failed_to_remove_stale_points_warning(m.failed_to_remove_stale_points_warning),
    LogData.IRREGULAR_BROADCASTING_PERIOD_WARNING_FIELD_NUMBER: lambda m: format_irregular_broadcasting_period_warning(m.irregular_broadcasting_period_warning),
    LogData.CLUSTERER_POINTS_OUT_OF_RANGE_WARNING_FIELD_NUMBER: lambda m: format_clusterer_points_out_of_range_warning(m.clusterer_points_out_of_range_warning),
    LogData.INTERNAL_ERROR_FIELD_NUMBER: lambda m: format_internal_error(m.internal_error),
    LogData.INTERNAL_FATAL_FIELD_NUMBER: lambda m: format_internal_fatal(m.internal_fatal),
    LogData.SERVICE_STARTING_INFO_FIELD_NUMBER: lambda m: format_service_starting_info(m.service_starting_info),
    LogData.CONFIG_NODE_NOT_FOUND_FATAL_FIELD_NUMBER: lambda m: format_config_node_not_found_fatal(m.config_node_not_found_fatal),
    LogData.SERVICE_SETUP_ERROR_FIELD_NUMBER: lambda m: format_service_setup_error(m.service_setup_error),
    LogData.FILESYSTEM_ERROR_FIELD_NUMBER: lambda m: format_filesystem_error(m.filesystem_error),
    LogData.INVALID_PRESET_WARNING_FIELD_NUMBER: lambda m: format_invalid_preset_warning(m.invalid_preset_warning),
    LogData.WEBSOCKET_FAILED_CLEAR_PENDING_ERROR_FIELD_NUMBER: lambda m: format_websocket_failed_clear_pending_error(m.websocket_failed_clear_pending_error),
    LogData.WEBSOCKET_FAILED_TO_STOP_ERROR_FIELD_NUMBER: lambda m: format_websocket_failed_to_stop_error(m.websocket_failed_to_stop_error),
    LogData.WEBSOCKET_FAILED_TO_CREATE_ERROR_FIELD_NUMBER: lambda m: format_websocket_failed_to_create_error(m.websocket_failed_to_create_error),
    LogData.UNEXPECTED_RPC_ERROR_FIELD_NUMBER: lambda m: format_unexpected_rpc_error(m.unexpected_rpc_error),
    LogData.LICENSE_POLL_FAILED_FIELD_NUMBER: lambda m: format_license_poll_failed(m.license_poll_failed),
    LogData.LICENSE_EXPIRED_WARNING_FIELD_NUMBER: lambda m: format_license_expired_warning(m.license_expired_warning),
    LogData.LICENSE_USAGE_EXCEEDED_WARNING_FIELD_NUMBER: lambda m: format_license_usage_exceeded_warning(m.license_usage_exceeded_warning),
    LogData.STATIC_THREAD_POOL_SLOW_TASK_WARNING_FIELD_NUMBER: lambda m: format_static_thread_pool_slow_task_warning(m.static_thread_pool_slow_task_warning),
    LogData.RPC_UNSUPPORTED_SERVICE_WARNING_FIELD_NUMBER: lambda m: format_rpc_unsupported_service_warning(m.rpc_unsupported_service_warning),
    LogData.WEBSOCKET_HANDLER_PROBLEM_FIELD_NUMBER: lambda m: format_websocket_handler_problem(m.websocket_handler_problem),
    LogData.WEBSOCKET_DESERIALIZE_ERROR_FIELD_NUMBER: lambda m: format_websocket_deserialize_error(m.websocket_deserialize_error),
    LogData.WEBSOCKET_EXPIRED_RPC_ENDPOINT_ERROR_FIELD_NUMBER: lambda m: format_websocket_expired_rpc_endpoint_error(m.websocket_expired_rpc_endpoint_error),
    LogData.WEBSOCKET_QUEUE_OVERLOADED_WARNING_FIELD_NUMBER: lambda m: format_websocket_queue_overloaded_warning(m.websocket_queue_overloaded_warning),
    LogData.RPC_FAILED_TO_NOTIFY_WARNING_FIELD_NUMBER: lambda m: format_rpc_failed_to_notify_warning(m.rpc_failed_to_notify_warning),
    LogData.CONFIG_SUBSCRIPTION_FAILED_WARNING_FIELD_NUMBER: lambda m: format_config_subscription_failed_warning(m.config_subscription_failed_warning),
    LogData.THREAD_POOL_CLAMPED_WORKERS_WARNING_FIELD_NUMBER: lambda m: format_thread_pool_clamped_workers_warning(m.thread_pool_clamped_workers_warning),
    LogData.STOPPING_HORUS_BAG_RECORDER_ALREADY_STOPPED_FIELD_NUMBER: lambda m: format_stopping_horus_bag_recorder_already_stopped(m.stopping_horus_bag_recorder_already_stopped),
    LogData.RECORDER_CONFIG_UPDATE_WHILE_RUNNING_FIELD_NUMBER: lambda m: format_recorder_config_update_while_running(m.recorder_config_update_while_running),
    LogData.CLAMPING_DATA_WARNING_FIELD_NUMBER: lambda m: format_clamping_data_warning(m.clamping_data_warning),
    LogData.LIDAR_INCOMPATIBLE_VALUES_FIELD_NUMBER: lambda m: format_lidar_incompatible_values(m.lidar_incompatible_values),
    LogData.CANNOT_DETERMINE_CONTAINER_ID_ERROR_FIELD_NUMBER: lambda m: format_cannot_determine_container_id_error(m.cannot_determine_container_id_error),
    LogData.STARTED_LIDAR_DRIVER_FIELD_NUMBER: lambda m: format_started_lidar_driver(m.started_lidar_driver),
    LogData.CANNOT_START_LIDAR_DRIVER_FIELD_NUMBER: lambda m: format_cannot_start_lidar_driver(m.cannot_start_lidar_driver),
    LogData.STOPPED_LIDAR_DRIVER_FIELD_NUMBER: lambda m: format_stopped_lidar_driver(m.stopped_lidar_driver),
    LogData.CANNOT_STOP_LIDAR_DRIVER_FIELD_NUMBER: lambda m: format_cannot_stop_lidar_driver(m.cannot_stop_lidar_driver),
    LogData.RESTARTED_LIDAR_DRIVER_FIELD_NUMBER: lambda m: format_restarted_lidar_driver(m.restarted_lidar_driver),
    LogData.CANNOT_RESTART_LIDAR_DRIVER_FIELD_NUMBER: lambda m: format_cannot_restart_lidar_driver(m.cannot_restart_lidar_driver),
    LogData.REMOVED_UNUSED_LIDAR_DRIVER_FIELD_NUMBER: lambda m: format_removed_unused_lidar_driver(m.removed_unused_lidar_driver),
    LogData.CANNOT_REMOVE_UNUSED_LIDAR_DRIVER_FIELD_NUMBER: lambda m: format_cannot_remove_unused_lidar_driver(m.cannot_remove_unused_lidar_driver),
    LogData.LIDAR_DRIVER_GC_FAILURE_FIELD_NUMBER: lambda m: format_lidar_driver_gc_failure(m.lidar_driver_gc_failure),
    LogData.ID_SPACE_EXHAUSTED_FIELD_NUMBER: lambda m: format_id_space_exhausted(m.id_space_exhausted),
    LogData.PREPROCESSING_TO_POINT_AGGREGATOR_POINTS_SKIPPED_FIELD_NUMBER: lambda m: format_preprocessing_to_point_aggregator_points_skipped(m.preprocessing_to_point_aggregator_points_skipped),
    LogData.MIN_MSG_INTERVAL_LESS_THAN_THRESHOLD_FIELD_NUMBER: lambda m: format_min_msg_interval_less_than_threshold(m.min_msg_interval_less_than_threshold),
    LogData.FAILED_TO_CLEANUP_ROS_WARNING_FIELD_NUMBER: lambda m: format_failed_to_cleanup_ros_warning(m.failed_to_cleanup_ros_warning),
    LogData.RPC_DISCONNECTED_WARNING_FIELD_NUMBER: lambda m: format_rpc_disconnected_warning(m.rpc_disconnected_warning),
    LogData.RPC_UNHANDLED_ERROR_FIELD_NUMBER: lambda m: format_rpc_unhandled_error(m.rpc_unhandled_error),
    LogData.TIME_DIFF_OUT_OF_RANGE_WARNING_FIELD_NUMBER: lambda m: format_time_diff_out_of_range_warning(m.time_diff_out_of_range_warning),
    LogData.TENSORRT_LOG_FIELD_NUMBER: lambda m: format_tensorrt_log(m.tensorrt_log),
    LogData.BUILDING_TENSORRT_ENGINE_INFO_FIELD_NUMBER: lambda m: format_building_tensorrt_engine_info(m.building_tensorrt_engine_info),
    LogData.LOADING_TENSORRT_ENGINE_INFO_FIELD_NUMBER: lambda m: format_loading_tensorrt_engine_info(m.loading_tensorrt_engine_info),
    LogData.CALIBRATION_MAP_NOT_FOUND_FIELD_NUMBER: lambda m: format_calibration_map_not_found(m.calibration_map_not_found),
    LogData.CALIBRATION_MAP_NOT_VALID_FIELD_NUMBER: lambda m: format_calibration_map_not_valid(m.calibration_map_not_valid),
    LogData.CALIBRATION_MAP_PATH_ALREADY_EXISTS_FIELD_NUMBER: lambda m: format_calibration_map_path_already_exists(m.calibration_map_path_already_exists),
    LogData.FAILED_TO_SAVE_CALIBRATION_MAP_FIELD_NUMBER: lambda m: format_failed_to_save_calibration_map(m.failed_to_save_calibration_map),
    LogData.FAILED_TO_REMOVE_CALIBRATION_MAP_FIELD_NUMBER: lambda m: format_failed_to_remove_calibration_map(m.failed_to_remove_calibration_map),
    LogData.FAILED_TO_ITERATE_IN_DIRECTORY_FIELD_NUMBER: lambda m: format_failed_to_iterate_in_directory(m.failed_to_iterate_in_directory),
    LogData.MAP_BASED_CALIBRATION_WITHOUT_MAP_LOADING_FIELD_NUMBER: lambda m: format_map_based_calibration_without_map_loading(m.map_based_calibration_without_map_loading),
    LogData.MAP_BASED_CALIBRATION_ALREADY_RUNNING_FIELD_NUMBER: lambda m: format_map_based_calibration_already_running(m.map_based_calibration_already_running),
    LogData.CANCEL_MAP_BASED_CALIBRATION_NOT_RUNNING_FIELD_NUMBER: lambda m: format_cancel_map_based_calibration_not_running(m.cancel_map_based_calibration_not_running),
    LogData.BAG_STREAM_NOT_FOUND_FIELD_NUMBER: lambda m: format_bag_stream_not_found(m.bag_stream_not_found),
    LogData.EVALUATION_BAG_STARTED_INFO_FIELD_NUMBER: lambda m: format_evaluation_bag_started_info(m.evaluation_bag_started_info),
    LogData.EVALUATION_BAG_FINISHED_INFO_FIELD_NUMBER: lambda m: format_evaluation_bag_finished_info(m.evaluation_bag_finished_info),
    LogData.BAG_NOT_FOUND_FIELD_NUMBER: lambda m: format_bag_not_found(m.bag_not_found),
    LogData.BUILDING_PIPELINE_INFO_FIELD_NUMBER: lambda m: format_building_pipeline_info(m.building_pipeline_info),
    LogData.BAG_IS_NOT_EVALUATION_FIELD_NUMBER: lambda m: format_bag_is_not_evaluation(m.bag_is_not_evaluation),
    LogData.HORUS_BAG_RUNNING_FIELD_NUMBER: lambda m: format_horus_bag_running(m.horus_bag_running),
    LogData.AUTO_GROUND_CALIBRATION_WARNING_FIELD_NUMBER: lambda m: format_auto_ground_calibration_warning(m.auto_ground_calibration_warning),
    LogData.AUTO_GROUND_CALIBRATION_ERROR_FIELD_NUMBER: lambda m: format_auto_ground_calibration_error(m.auto_ground_calibration_error),
    LogData.OBJECT_DETECTOR_NOT_LOADED_WARNING_FIELD_NUMBER: lambda m: format_object_detector_not_loaded_warning(m.object_detector_not_loaded_warning),
    LogData.CALIBRATION_IS_RUNNING_ERROR_FIELD_NUMBER: lambda m: format_calibration_is_running_error(m.calibration_is_running_error),
    LogData.MODEL_INIT_IN_PROGRESS_INFO_FIELD_NUMBER: lambda m: format_model_init_in_progress_info(m.model_init_in_progress_info),
    LogData.RPC_TIMEOUT_WITH_RESOLUTION_WARNING_FIELD_NUMBER: lambda m: format_rpc_timeout_with_resolution_warning(m.rpc_timeout_with_resolution_warning),
    LogData.CALIBRATION_WAS_CANCELLED_INFO_FIELD_NUMBER: lambda m: format_calibration_was_cancelled_info(m.calibration_was_cancelled_info),
    LogData.CALIBRATION_MAP_RECORDING_FAILED_TO_START_FIELD_NUMBER: lambda m: format_calibration_map_recording_failed_to_start(m.calibration_map_recording_failed_to_start),
    LogData.DETECTION_PIPELINE_REQUESTED_RESET_INFO_FIELD_NUMBER: lambda m: format_detection_pipeline_requested_reset_info(m.detection_pipeline_requested_reset_info),
    LogData.PREPROCESSING_SERVICE_PIPELINE_UNAVAILABLE_FIELD_NUMBER: lambda m: format_preprocessing_service_pipeline_unavailable(m.preprocessing_service_pipeline_unavailable),
    LogData.CIRCULAR_RECORDING_DISABLED_WARNING_FIELD_NUMBER: lambda m: format_circular_recording_disabled_warning(m.circular_recording_disabled_warning),
    LogData.SNAPSHOT_ALREADY_RUNNING_WARNING_FIELD_NUMBER: lambda m: format_snapshot_already_running_warning(m.snapshot_already_running_warning),
    LogData.ACTIVE_PROJECT_CHANGED_INFO_FIELD_NUMBER: lambda m: format_active_project_changed_info(m.active_project_changed_info),
    LogData.PROJECT_CONFIG_UPDATED_INFO_FIELD_NUMBER: lambda m: format_project_config_updated_info(m.project_config_updated_info),
    LogData.INVALID_LIDAR_TIMESTAMP_FIELD_NUMBER: lambda m: format_invalid_lidar_timestamp(m.invalid_lidar_timestamp),
    LogData.CALIBRATION_ACCUMULATING_POINTS_INFO_FIELD_NUMBER: lambda m: format_calibration_accumulating_points_info(m.calibration_accumulating_points_info),
    LogData.SPARSE_NOISE_FILTER_USAGE_NON_ROTATIONAL_LIDARS_FIELD_NUMBER: lambda m: format_sparse_noise_filter_usage_non_rotational_lidars(m.sparse_noise_filter_usage_non_rotational_lidars),
    LogData.FILE_WRITE_WARNING_FIELD_NUMBER: lambda m: format_file_write_warning(m.file_write_warning),
    LogData.LICENSE_FORBIDDEN_FEATURE_FIELD_NUMBER: lambda m: format_license_forbidden_feature(m.license_forbidden_feature),
    LogData.FAILED_TO_UPDATE_CONFIGURATION_FIELD_NUMBER: lambda m: format_failed_to_update_configuration(m.failed_to_update_configuration),
    LogData.OBSTRUCTION_DETECTOR_BAD_REFERENCE_WARNING_FIELD_NUMBER: lambda m: format_obstruction_detector_bad_reference_warning(m.obstruction_detector_bad_reference_warning),
    LogData.PROJECT_FILE_INVALID_PERMISSIONS_ERROR_FIELD_NUMBER: lambda m: format_project_file_invalid_permissions_error(m.project_file_invalid_permissions_error),
    LogData.PIPELINE_SCHEDULER_ERROR_FIELD_NUMBER: lambda m: format_pipeline_scheduler_error(m.pipeline_scheduler_error),
    LogData.MULTI_LIDAR_CALIBRATION_WARNING_FIELD_NUMBER: lambda m: format_multi_lidar_calibration_warning(m.multi_lidar_calibration_warning),
    LogData.MULTI_LIDAR_CALIBRATION_ERROR_FIELD_NUMBER: lambda m: format_multi_lidar_calibration_error(m.multi_lidar_calibration_error),
    LogData.OLD_BAG_WARNING_FIELD_NUMBER: lambda m: format_old_bag_warning(m.old_bag_warning),
    LogData.UPGRADING_BAG_INFO_FIELD_NUMBER: lambda m: format_upgrading_bag_info(m.upgrading_bag_info),
    LogData.BAG_CALIBRATION_SAVE_FAILED_FIELD_NUMBER: lambda m: format_bag_calibration_save_failed(m.bag_calibration_save_failed),
    LogData.BAG_UPGRADE_FAILED_FIELD_NUMBER: lambda m: format_bag_upgrade_failed(m.bag_upgrade_failed),
    LogData.UNKNOWN_LIDAR_ERROR_FIELD_NUMBER: lambda m: format_unknown_lidar_error(m.unknown_lidar_error),
    LogData.INVALID_POINT_CLOUD_WARNING_FIELD_NUMBER: lambda m: format_invalid_point_cloud_warning(m.invalid_point_cloud_warning),
    LogData.LIDAR_IS_DROPPING_PACKETS_FIELD_NUMBER: lambda m: format_lidar_is_dropping_packets(m.lidar_is_dropping_packets),
    LogData.REMOVED_INVALID_LIDARS_FROM_CONFIG_WARNING_FIELD_NUMBER: lambda m: format_removed_invalid_lidars_from_config_warning(m.removed_invalid_lidars_from_config_warning),
    LogData.CALIBRATION_MAP_RECORDING_FAILED_FIELD_NUMBER: lambda m: format_calibration_map_recording_failed(m.calibration_map_recording_failed),
    LogData.DETECTION_NODE_NOT_FOUND_ERROR_FIELD_NUMBER: lambda m: format_detection_node_not_found_error(m.detection_node_not_found_error),
    LogData.CREATED_VERSION_BACKUP_INFO_FIELD_NUMBER: lambda m: format_created_version_backup_info(m.created_version_backup_info),
    LogData.PLY_FILE_LOAD_FAILED_ERROR_FIELD_NUMBER: lambda m: format_ply_file_load_failed_error(m.ply_file_load_failed_error),
    LogData.HESAI_DRIVER_LIFECYCLE_FIELD_NUMBER: lambda m: format_hesai_driver_lifecycle(m.hesai_driver_lifecycle),
    LogData.HESAI_DRIVER_ERROR_FIELD_NUMBER: lambda m: format_hesai_driver_error(m.hesai_driver_error),
    LogData.HESAI_PACKET_PROCESSING_FAILED_FIELD_NUMBER: lambda m: format_hesai_packet_processing_failed(m.hesai_packet_processing_failed),
    LogData.HESAI_CORRECTION_FILE_ERROR_FIELD_NUMBER: lambda m: format_hesai_correction_file_error(m.hesai_correction_file_error),
    LogData.HESAI_PACKET_STATISTICS_FIELD_NUMBER: lambda m: format_hesai_packet_statistics(m.hesai_packet_statistics),
    LogData.PLY_FILE_WRITE_FAILED_ERROR_FIELD_NUMBER: lambda m: format_ply_file_write_failed_error(m.ply_file_write_failed_error),
    LogData.PROJECT_SAVE_ERROR_FIELD_NUMBER: lambda m: format_project_save_error(m.project_save_error),
    LogData.SAVE_STATIC_ENVIRONMENT_SUCCESS_FIELD_NUMBER: lambda m: format_save_static_environment_success(m.save_static_environment_success),
    LogData.SAVE_STATIC_ENVIRONMENT_FAILED_FIELD_NUMBER: lambda m: format_save_static_environment_failed(m.save_static_environment_failed),
    LogData.LOAD_STATIC_ENVIRONMENT_SUCCESS_FIELD_NUMBER: lambda m: format_load_static_environment_success(m.load_static_environment_success),
    LogData.LOAD_STATIC_ENVIRONMENT_FAILED_FIELD_NUMBER: lambda m: format_load_static_environment_failed(m.load_static_environment_failed),
    LogData.ATTEMPT_TO_INJECT_INVALID_LIDAR_ID_WARNING_FIELD_NUMBER: lambda m: format_attempt_to_inject_invalid_lidar_id_warning(m.attempt_to_inject_invalid_lidar_id_warning),
    LogData.RESET_BUNDLED_PACKET_DUE_TO_UNEXPECTED_PACKET_FIELD_NUMBER: lambda m: format_reset_bundled_packet_due_to_unexpected_packet(m.reset_bundled_packet_due_to_unexpected_packet),
    LogData.PACKET_BUNDLER_DROPPED_PACKETS_WARNING_FIELD_NUMBER: lambda m: format_packet_bundler_dropped_packets_warning(m.packet_bundler_dropped_packets_warning),
    LogData.PACKET_BUNDLER_FRAME_JUMP_WARNING_FIELD_NUMBER: lambda m: format_packet_bundler_frame_jump_warning(m.packet_bundler_frame_jump_warning),
    LogData.LIDAR_CORRECTION_LOADING_SUCCESS_FIELD_NUMBER: lambda m: format_lidar_correction_loading_success(m.lidar_correction_loading_success),
    LogData.LIDAR_CORRECTION_LOADING_FAILURE_FIELD_NUMBER: lambda m: format_lidar_correction_loading_failure(m.lidar_correction_loading_failure),
    LogData.HESAI_PACKET_STATISTICS_LIDAR_FIELD_NUMBER: lambda m: format_hesai_packet_statistics_lidar(m.hesai_packet_statistics_lidar),
}

def _unknown_format(data: LogData) -> str:
    _ = data

    return "unknown log message"

def format_log_field_no(data: LogData) -> int:
    if fields := data.ListFields():
        return typing.cast(int, fields[0][0].number)
    return 0

def format_log_data(data: LogData) -> str:
    return _formatters.get(format_log_field_no(data), _unknown_format)(data)

# MARK: Specific log formatting

def format_generic(log: _logs_pb.Generic) -> str:
    """Formats log `Generic` to a string."""
    return f"{log.message}"

def format_oom(log: _logs_pb.Oom) -> str:
    """Formats log `Oom` to a string."""
    return f"Out of memory"

def format_rpc_connection_error(log: _logs_pb.RpcConnectionError) -> str:
    """Formats log `RpcConnectionError` to a string."""
    return f"Cannot initiate connection to {log.target_service} at {log.target_uri}: {log.details}"

def format_invalid_project_name(log: _logs_pb.InvalidProjectName) -> str:
    """Formats log `InvalidProjectName` to a string."""
    return f"Project name `{log.name}` is invalid: {log.details}"

def format_project_not_found(log: _logs_pb.ProjectNotFound) -> str:
    """Formats log `ProjectNotFound` to a string."""
    return f"Project `{log.name}` not found"

def format_project_already_exists(log: _logs_pb.ProjectAlreadyExists) -> str:
    """Formats log `ProjectAlreadyExists` to a string."""
    return f"Project `{log.name}` already exists"

def format_invalid_configuration(log: _logs_pb.InvalidConfiguration) -> str:
    """Formats log `InvalidConfiguration` to a string."""
    return f"Configuration is invalid"

def format_entity_not_found(log: _logs_pb.EntityNotFound) -> str:
    """Formats log `EntityNotFound` to a string."""
    return f"{log.entity_type} `{log.id}` not found"

def format_active_project_cannot_be_deleted(log: _logs_pb.ActiveProjectCannotBeDeleted) -> str:
    """Formats log `ActiveProjectCannotBeDeleted` to a string."""
    return f"Active project cannot be deleted"

def format_rpc_disconnection_error(log: _logs_pb.RpcDisconnectionError) -> str:
    """Formats log `RpcDisconnectionError` to a string."""
    return f"Cannot terminate connection to {log.target_service} at {log.target_uri}: {log.details}"

def format_dropped_logs(log: _logs_pb.DroppedLogs) -> str:
    """Formats log `DroppedLogs` to a string."""
    return f"Dropped {log.n} log messages"

def format_opened_project(log: _logs_pb.OpenedProject) -> str:
    """Formats log `OpenedProject` to a string."""
    return f"Opened project {log.project_name}"

def format_created_project(log: _logs_pb.CreatedProject) -> str:
    """Formats log `CreatedProject` to a string."""
    return f"Created project {log.project_name} from {log.source_name}"

def format_config_unavailable(log: _logs_pb.ConfigUnavailable) -> str:
    """Formats log `ConfigUnavailable` to a string."""
    return f"Configuration is unavailable; request cannot be processed."

def format_invalid_request(log: _logs_pb.InvalidRequest) -> str:
    """Formats log `InvalidRequest` to a string."""
    return f"Invalid request received: {log.details}"

def format_sanity_check_error(log: _logs_pb.SanityCheckError) -> str:
    """Formats log `SanityCheckError` to a string."""
    return f"{log.sanity_check_name} failed: {log.details}"

def format_bag_failed_to_open(log: _logs_pb.BagFailedToOpen) -> str:
    """Formats log `BagFailedToOpen` to a string."""
    return f"The bag file ({log.horus_bag_path}) could not be opened: {log.details}"

def format_bag_failed_to_close(log: _logs_pb.BagFailedToClose) -> str:
    """Formats log `BagFailedToClose` to a string."""
    return f"Bag file could not be closed: {log.details}"

def format_bag_conversion_failed(log: _logs_pb.BagConversionFailed) -> str:
    """Formats log `BagConversionFailed` to a string."""
    return f"Bag file conversion failed: {log.details}"

def format_bag_failed_to_write(log: _logs_pb.BagFailedToWrite) -> str:
    """Formats log `BagFailedToWrite` to a string."""
    return f"Bag file writing failed {log.name}: {log.details}"

def format_calibration_error(log: _logs_pb.CalibrationError) -> str:
    """Formats log `CalibrationError` to a string."""
    return f"Calibration failed: {log.details}"

def format_project_manager_failed_to_start_recording(log: _logs_pb.ProjectManagerFailedToStartRecording) -> str:
    """Formats log `ProjectManagerFailedToStartRecording` to a string."""
    return f"Failed to start recording: {log.details}"

def format_project_manager_failed_to_stop_recording(log: _logs_pb.ProjectManagerFailedToStopRecording) -> str:
    """Formats log `ProjectManagerFailedToStopRecording` to a string."""
    return f"Failed to stop recording: {log.details}"

def format_service_connection_timed_out(log: _logs_pb.ServiceConnectionTimedOut) -> str:
    """Formats log `ServiceConnectionTimedOut` to a string."""
    return f"The connection with the requested {log.node_type} node with ID {log.node_id} timed out"

def format_bag_recorder_already_running(log: _logs_pb.BagRecorderAlreadyRunning) -> str:
    """Formats log `BagRecorderAlreadyRunning` to a string."""
    return f"Bag recorder is already running for {log.path}"

def format_license_server_connection_error(log: _logs_pb.LicenseServerConnectionError) -> str:
    """Formats log `LicenseServerConnectionError` to a string."""
    return f"Could not connect to license server: {log.details}."

def format_license_error(log: _logs_pb.LicenseError) -> str:
    """Formats log `LicenseError` to a string."""
    return f"Could not read license: {log.details}."

def format_license_not_found_error(log: _logs_pb.LicenseNotFoundError) -> str:
    """Formats log `LicenseNotFoundError` to a string."""
    return f"Found no valid license."

def format_license_expired_error(log: _logs_pb.LicenseExpiredError) -> str:
    """Formats log `LicenseExpiredError` to a string."""
    return f"License expired on {log.expiration_time}."

def format_license_exceeded_error(log: _logs_pb.LicenseExceededError) -> str:
    """Formats log `LicenseExceededError` to a string."""
    return f"Project exceeds number of LiDARs allowed by license ({log.lidar_count} > {log.max_lidar_count})."

def format_license_host_machine_error(log: _logs_pb.LicenseHostMachineError) -> str:
    """Formats log `LicenseHostMachineError` to a string."""
    return f"Could not validate license with current hardware - {log.details}."

def format_license_privilege_error(log: _logs_pb.LicensePrivilegeError) -> str:
    """Formats log `LicensePrivilegeError` to a string."""
    return f"Endpoint requires \"{log.missing_privilege}\" privilege: {log.level_error}."

def format_license_active_info(log: _logs_pb.LicenseActiveInfo) -> str:
    """Formats log `LicenseActiveInfo` to a string."""
    return f"License is active."

def format_multiple_licenses_warning(log: _logs_pb.MultipleLicensesWarning) -> str:
    """Formats log `MultipleLicensesWarning` to a string."""
    return f"Found {log.non_expired_licenses_count} non-expired concurrent licenses. Only \"{log.active_license_filename}\" is used."

def format_license_current_license_status_info(log: _logs_pb.LicenseCurrentLicenseStatusInfo) -> str:
    """Formats log `LicenseCurrentLicenseStatusInfo` to a string."""
    return f"Current license accepts up to {log.max_lidar_count} and expires on {log.expiration_time}."

def format_bag_recording_stopped_info(log: _logs_pb.BagRecordingStoppedInfo) -> str:
    """Formats log `BagRecordingStoppedInfo` to a string."""
    return f"Bag recording stopped for {log.path}: {log.details}"

def format_bag_recording_failed_to_start(log: _logs_pb.BagRecordingFailedToStart) -> str:
    """Formats log `BagRecordingFailedToStart` to a string."""
    return f"Bag recording failed to start: {log.details}"

def format_bag_recording_started_info(log: _logs_pb.BagRecordingStartedInfo) -> str:
    """Formats log `BagRecordingStartedInfo` to a string."""
    return f"Bag recording started for {log.path}"

def format_replay_restarted_info(log: _logs_pb.ReplayRestartedInfo) -> str:
    """Formats log `ReplayRestartedInfo` to a string."""
    return f"Replay Restarted"

def format_input_source_change_requested_info(log: _logs_pb.InputSourceChangeRequestedInfo) -> str:
    """Formats log `InputSourceChangeRequestedInfo` to a string."""
    return f"Input source {log.source} requested"

def format_input_source_switched_info(log: _logs_pb.InputSourceSwitchedInfo) -> str:
    """Formats log `InputSourceSwitchedInfo` to a string."""
    return f"Input source changed to {log.source}"

def format_rpc_timeout_warning(log: _logs_pb.RpcTimeoutWarning) -> str:
    """Formats log `RpcTimeoutWarning` to a string."""
    return f"RPC request {log.request_name} to {log.endpoint} timed out after {log.elapsed}."

def format_cannot_write_log_file(log: _logs_pb.CannotWriteLogFile) -> str:
    """Formats log `CannotWriteLogFile` to a string."""
    return f"Notification service cannot write log in {log.path}: {log.details}"

def format_point_cloud_parsing_failure_warning(log: _logs_pb.PointCloudParsingFailureWarning) -> str:
    """Formats log `PointCloudParsingFailureWarning` to a string."""
    return f"Point cloud parsing failed: {log.details}"

def format_lidar_is_dead(log: _logs_pb.LidarIsDead) -> str:
    """Formats log `LidarIsDead` to a string."""
    return f"The lidar {log.lidar_id} is considered dead. No data has been received for a while."

def format_lidar_is_not_dead_anymore(log: _logs_pb.LidarIsNotDeadAnymore) -> str:
    """Formats log `LidarIsNotDeadAnymore` to a string."""
    return f"The lidar {log.lidar_id} is not considered dead anymore. A point cloud has been received again after some time."

def format_lidar_is_obstructed(log: _logs_pb.LidarIsObstructed) -> str:
    """Formats log `LidarIsObstructed` to a string."""
    return f"The lidar {log.lidar_id} is obstructed."

def format_lidar_is_not_obstructed_anymore(log: _logs_pb.LidarIsNotObstructedAnymore) -> str:
    """Formats log `LidarIsNotObstructedAnymore` to a string."""
    return f"The lidar {log.lidar_id} is not obstructed anymore."

def format_lidar_is_tilted(log: _logs_pb.LidarIsTilted) -> str:
    """Formats log `LidarIsTilted` to a string."""
    return f"The lidar {log.lidar_id} is tilted."

def format_lidar_is_not_tilted_anymore(log: _logs_pb.LidarIsNotTiltedAnymore) -> str:
    """Formats log `LidarIsNotTiltedAnymore` to a string."""
    return f"The lidar {log.lidar_id} is not tilted anymore."

def format_lidar_has_been_automatically_recalibrated(log: _logs_pb.LidarHasBeenAutomaticallyRecalibrated) -> str:
    """Formats log `LidarHasBeenAutomaticallyRecalibrated` to a string."""
    return f"The lidar {log.lidar_id} has been automatically re-calibrated."

def format_received_first_data_for_lidar(log: _logs_pb.ReceivedFirstDataForLidar) -> str:
    """Formats log `ReceivedFirstDataForLidar` to a string."""
    return f"Data has been received for the first time for the lidar {log.lidar_id}."

def format_termination_failure_error(log: _logs_pb.TerminationFailureError) -> str:
    """Formats log `TerminationFailureError` to a string."""
    return f"Failed to destruct {log.component}: {log.details}"

def format_frame_processing_error(log: _logs_pb.FrameProcessingError) -> str:
    """Formats log `FrameProcessingError` to a string."""
    return f"The Frame Task Scheduler failed to process frame task: {log.details}"

def format_thread_pool_unavailable_error(log: _logs_pb.ThreadPoolUnavailableError) -> str:
    """Formats log `ThreadPoolUnavailableError` to a string."""
    return f"Thread pool is not available: {log.consequence}"

def format_invalid_argument(log: _logs_pb.InvalidArgument) -> str:
    """Formats log `InvalidArgument` to a string."""
    return f"Invalid argument: {log.details}"

def format_component_initialization_failure_fatal(log: _logs_pb.ComponentInitializationFailureFatal) -> str:
    """Formats log `ComponentInitializationFailureFatal` to a string."""
    return f"Failed to initialize {log.component}: {log.details}"

def format_unhandled_enum_case_error(log: _logs_pb.UnhandledEnumCaseError) -> str:
    """Formats log `UnhandledEnumCaseError` to a string."""
    return f"Unhandled enum case {log.case_name} at {log.location}"

def format_bag_empty_error(log: _logs_pb.BagEmptyError) -> str:
    """Formats log `BagEmptyError` to a string."""
    return f"The replay bag file is empty."

def format_discarding_data_error(log: _logs_pb.DiscardingDataError) -> str:
    """Formats log `DiscardingDataError` to a string."""
    return f"Discarding data in {log.component}: {log.details}"

def format_discarding_data_warning(log: _logs_pb.DiscardingDataWarning) -> str:
    """Formats log `DiscardingDataWarning` to a string."""
    return f"Discarding data in {log.component}: {log.details}."

def format_nothing_to_process(log: _logs_pb.NothingToProcess) -> str:
    """Formats log `NothingToProcess` to a string."""
    return f"Nothing to process: {log.details}"

def format_invalid_component_configuration(log: _logs_pb.InvalidComponentConfiguration) -> str:
    """Formats log `InvalidComponentConfiguration` to a string."""
    return f"Invalid configuration for {log.component}: {log.details}"

def format_channel_receiver_not_found_warning(log: _logs_pb.ChannelReceiverNotFoundWarning) -> str:
    """Formats log `ChannelReceiverNotFoundWarning` to a string."""
    return f"Channel receiver {log.key} not found"

def format_model_load_failure(log: _logs_pb.ModelLoadFailure) -> str:
    """Formats log `ModelLoadFailure` to a string."""
    return f"Model {log.model} failed to load: {log.details}"

def format_model_execution_failure_error(log: _logs_pb.ModelExecutionFailureError) -> str:
    """Formats log `ModelExecutionFailureError` to a string."""
    return f"Model {log.model} failed to execute: {log.details}"

def format_service_ready_info(log: _logs_pb.ServiceReadyInfo) -> str:
    """Formats log `ServiceReadyInfo` to a string."""
    return f"{log.service} service is ready"

def format_model_preparing_info(log: _logs_pb.ModelPreparingInfo) -> str:
    """Formats log `ModelPreparingInfo` to a string."""
    return f"Preparing model {log.model}, it may take several seconds to stabilize."

def format_model_initialized_info(log: _logs_pb.ModelInitializedInfo) -> str:
    """Formats log `ModelInitializedInfo` to a string."""
    return f"{log.name} model has been initialized and is ready for use."

def format_model_initialization_failure_warning(log: _logs_pb.ModelInitializationFailureWarning) -> str:
    """Formats log `ModelInitializationFailureWarning` to a string."""
    return f"Failed to initialize model {log.model}: {log.details}"

def format_ros_spinner_stopped_warning(log: _logs_pb.RosSpinnerStoppedWarning) -> str:
    """Formats log `RosSpinnerStoppedWarning` to a string."""
    return f"ROS spinner stopped: {log.details}"

def format_actor_system_unavailable_error(log: _logs_pb.ActorSystemUnavailableError) -> str:
    """Formats log `ActorSystemUnavailableError` to a string."""
    return f"Actor system is not available. Please reinstantiate the pipeline."

def format_config_node_not_found_error(log: _logs_pb.ConfigNodeNotFoundError) -> str:
    """Formats log `ConfigNodeNotFoundError` to a string."""
    return f"Configuration for node {log.node} is unavailable"

def format_bag_timestamp_out_of_order_error(log: _logs_pb.BagTimestampOutOfOrderError) -> str:
    """Formats log `BagTimestampOutOfOrderError` to a string."""
    return f"Timestamps are out of order: {log.prior_timestamp} > {log.current_timestamp}"

def format_bag_replay_unexpected_timestamp_error(log: _logs_pb.BagReplayUnexpectedTimestampError) -> str:
    """Formats log `BagReplayUnexpectedTimestampError` to a string."""
    return f"Unexpected timestamp: expected {log.expected_timestamp} but received {log.received_timestamp}"

def format_websocket_closed_info(log: _logs_pb.WebsocketClosedInfo) -> str:
    """Formats log `WebsocketClosedInfo` to a string."""
    return f"WebSocket RPC connection to {log.uri} closed: {log.reason}"

def format_websocket_opened_info(log: _logs_pb.WebsocketOpenedInfo) -> str:
    """Formats log `WebsocketOpenedInfo` to a string."""
    return f"WebSocket RPC connection to {log.uri} opened (as {log.endpoint_is_server_client})"

def format_subscriber_disconnected_info(log: _logs_pb.SubscriberDisconnectedInfo) -> str:
    """Formats log `SubscriberDisconnectedInfo` to a string."""
    return f"Subscriber {log.name} disconnected ({log.uri})"

def format_thread_pool_slowing_down_warning(log: _logs_pb.ThreadPoolSlowingDownWarning) -> str:
    """Formats log `ThreadPoolSlowingDownWarning` to a string."""
    return f"Thread pool is slowing down for {log.number} times!"

def format_thread_pool_not_responding_warning(log: _logs_pb.ThreadPoolNotRespondingWarning) -> str:
    """Formats log `ThreadPoolNotRespondingWarning` to a string."""
    return f"Thread pool is not responding for {log.not_responding_for}."

def format_thread_pool_broken_promise_warning(log: _logs_pb.ThreadPoolBrokenPromiseWarning) -> str:
    """Formats log `ThreadPoolBrokenPromiseWarning` to a string."""
    return f"Thread pool task future is no longer available. The thread pool might have been restarted during events such as input source change."

def format_box_fitting_points_below_base_warning(log: _logs_pb.BoxFittingPointsBelowBaseWarning) -> str:
    """Formats log `BoxFittingPointsBelowBaseWarning` to a string."""
    return f"Some points are below the base Z-coordinate when fitting the box. This might affect the detection results."

def format_failed_to_remove_stale_points_warning(log: _logs_pb.FailedToRemoveStalePointsWarning) -> str:
    """Formats log `FailedToRemoveStalePointsWarning` to a string."""
    return f"Failed to remove stale points: {log.details}"

def format_irregular_broadcasting_period_warning(log: _logs_pb.IrregularBroadcastingPeriodWarning) -> str:
    """Formats log `IrregularBroadcastingPeriodWarning` to a string."""
    return f"{log.num_irregular} irregular broadcast(s) detected in the last {log.duration} (mean deviation: {log.mean_deviation})."

def format_clusterer_points_out_of_range_warning(log: _logs_pb.ClustererPointsOutOfRangeWarning) -> str:
    """Formats log `ClustererPointsOutOfRangeWarning` to a string."""
    return f"There are {log.num_points} points out of range in the clusterer. Some points: {log.sample_points}."

def format_internal_error(log: _logs_pb.InternalError) -> str:
    """Formats log `InternalError` to a string."""
    return f"An internal error occurred: {log.details}"

def format_internal_fatal(log: _logs_pb.InternalFatal) -> str:
    """Formats log `InternalFatal` to a string."""
    return f"An internal fatal error occurred: {log.details}"

def format_service_starting_info(log: _logs_pb.ServiceStartingInfo) -> str:
    """Formats log `ServiceStartingInfo` to a string."""
    return f"{log.service} is starting"

def format_config_node_not_found_fatal(log: _logs_pb.ConfigNodeNotFoundFatal) -> str:
    """Formats log `ConfigNodeNotFoundFatal` to a string."""
    return f"Configuration for node {log.node} is unavailable"

def format_service_setup_error(log: _logs_pb.ServiceSetupError) -> str:
    """Formats log `ServiceSetupError` to a string."""
    return f"Failed to setup: {log.details}"

def format_filesystem_error(log: _logs_pb.FilesystemError) -> str:
    """Formats log `FilesystemError` to a string."""
    return f"Filesystem error: {log.details}"

def format_invalid_preset_warning(log: _logs_pb.InvalidPresetWarning) -> str:
    """Formats log `InvalidPresetWarning` to a string."""
    return f"Tried to apply invalid preset {log.preset} due to {log.reason}, falling back to {log.fallback_name}."

def format_websocket_failed_clear_pending_error(log: _logs_pb.WebsocketFailedClearPendingError) -> str:
    """Formats log `WebsocketFailedClearPendingError` to a string."""
    return f"WebSocket failed to clear pending queues: {log.exception}"

def format_websocket_failed_to_stop_error(log: _logs_pb.WebsocketFailedToStopError) -> str:
    """Formats log `WebsocketFailedToStopError` to a string."""
    return f"WebSocket failed to stop: {log.exception}"

def format_websocket_failed_to_create_error(log: _logs_pb.WebsocketFailedToCreateError) -> str:
    """Formats log `WebsocketFailedToCreateError` to a string."""
    return f"Failed to create RPC server ws://{log.host}:{log.port}/: {log.exception}"

def format_unexpected_rpc_error(log: _logs_pb.UnexpectedRpcError) -> str:
    """Formats log `UnexpectedRpcError` to a string."""
    return f"Unexpected RPC error: {log.details}"

def format_license_poll_failed(log: _logs_pb.LicensePollFailed) -> str:
    """Formats log `LicensePollFailed` to a string."""
    return f"Failed to poll license server: {log.details}"

def format_license_expired_warning(log: _logs_pb.LicenseExpiredWarning) -> str:
    """Formats log `LicenseExpiredWarning` to a string."""
    return f"License expired on {log.expiration_time}."

def format_license_usage_exceeded_warning(log: _logs_pb.LicenseUsageExceededWarning) -> str:
    """Formats log `LicenseUsageExceededWarning` to a string."""
    return f"License count of {log.lidar_count} exceeds allowed usage of {log.max_lidar_count}"

def format_static_thread_pool_slow_task_warning(log: _logs_pb.StaticThreadPoolSlowTaskWarning) -> str:
    """Formats log `StaticThreadPoolSlowTaskWarning` to a string."""
    return f"ThreadPool task took {log.time_elapsed} to complete task enqueued in {log.enqueue_location}"

def format_rpc_unsupported_service_warning(log: _logs_pb.RpcUnsupportedServiceWarning) -> str:
    """Formats log `RpcUnsupportedServiceWarning` to a string."""
    return f"Received request for unsupported service: {log.service_id}"

def format_websocket_handler_problem(log: _logs_pb.WebsocketHandlerProblem) -> str:
    """Formats log `WebsocketHandlerProblem` to a string."""
    return f"WebSocket RPC handler encountered an error: {log.what}"

def format_websocket_deserialize_error(log: _logs_pb.WebsocketDeserializeError) -> str:
    """Formats log `WebsocketDeserializeError` to a string."""
    return f"WebSocket RPC received message cannot be deserialized: {log.what}"

def format_websocket_expired_rpc_endpoint_error(log: _logs_pb.WebsocketExpiredRpcEndpointError) -> str:
    """Formats log `WebsocketExpiredRpcEndpointError` to a string."""
    return f"Attempting to access expired WebSocket RPC endpoint {log.uri}"

def format_websocket_queue_overloaded_warning(log: _logs_pb.WebsocketQueueOverloadedWarning) -> str:
    """Formats log `WebsocketQueueOverloadedWarning` to a string."""
    return f"Websocket message queue overloaded: {log.current} / {log.max}"

def format_rpc_failed_to_notify_warning(log: _logs_pb.RpcFailedToNotifyWarning) -> str:
    """Formats log `RpcFailedToNotifyWarning` to a string."""
    return f"Could not notify {log.notification_name} to {log.service} {log.uri}: {log.what}"

def format_config_subscription_failed_warning(log: _logs_pb.ConfigSubscriptionFailedWarning) -> str:
    """Formats log `ConfigSubscriptionFailedWarning` to a string."""
    return f"Failed to subscribe to config: {log.details}"

def format_thread_pool_clamped_workers_warning(log: _logs_pb.ThreadPoolClampedWorkersWarning) -> str:
    """Formats log `ThreadPoolClampedWorkersWarning` to a string."""
    return f"Clamped thread pool workers to {log.clamped} instead of {log.requested}"

def format_stopping_horus_bag_recorder_already_stopped(log: _logs_pb.StoppingHorusBagRecorderAlreadyStopped) -> str:
    """Formats log `StoppingHorusBagRecorderAlreadyStopped` to a string."""
    return f"Trying to stop the Horus bag recorder which is already stopped."

def format_recorder_config_update_while_running(log: _logs_pb.RecorderConfigUpdateWhileRunning) -> str:
    """Formats log `RecorderConfigUpdateWhileRunning` to a string."""
    return f"An update for the Horus bag recorder has been received while it was running. This update will not be taken into account until the next recording."

def format_clamping_data_warning(log: _logs_pb.ClampingDataWarning) -> str:
    """Formats log `ClampingDataWarning` to a string."""
    return f"Clamping point cloud: {log.details}"

def format_lidar_incompatible_values(log: _logs_pb.LidarIncompatibleValues) -> str:
    """Formats log `LidarIncompatibleValues` to a string."""
    return f"Lidars {log.lidar_ip1} and {log.lidar_ip2} have incompatible {log.value_names}: {log.value1} vs. {log.value2}; {log.resolution}."

def format_cannot_determine_container_id_error(log: _logs_pb.CannotDetermineContainerIdError) -> str:
    """Formats log `CannotDetermineContainerIdError` to a string."""
    return f"Cannot determine current Docker container ID; unknown {log.container_id_file_path} format."

def format_started_lidar_driver(log: _logs_pb.StartedLidarDriver) -> str:
    """Formats log `StartedLidarDriver` to a string."""
    return f"Started lidar driver container {log.lidar_id}."

def format_cannot_start_lidar_driver(log: _logs_pb.CannotStartLidarDriver) -> str:
    """Formats log `CannotStartLidarDriver` to a string."""
    return f"Cannot start lidar driver container {log.lidar_id}: {log.details}."

def format_stopped_lidar_driver(log: _logs_pb.StoppedLidarDriver) -> str:
    """Formats log `StoppedLidarDriver` to a string."""
    return f"Stopped lidar driver container {log.lidar_id}."

def format_cannot_stop_lidar_driver(log: _logs_pb.CannotStopLidarDriver) -> str:
    """Formats log `CannotStopLidarDriver` to a string."""
    return f"Cannot stop lidar driver container {log.lidar_id}: {log.details}."

def format_restarted_lidar_driver(log: _logs_pb.RestartedLidarDriver) -> str:
    """Formats log `RestartedLidarDriver` to a string."""
    return f"Restarted lidar driver container {log.lidar_id}."

def format_cannot_restart_lidar_driver(log: _logs_pb.CannotRestartLidarDriver) -> str:
    """Formats log `CannotRestartLidarDriver` to a string."""
    return f"Cannot restart lidar driver container {log.lidar_id}: {log.details}."

def format_removed_unused_lidar_driver(log: _logs_pb.RemovedUnusedLidarDriver) -> str:
    """Formats log `RemovedUnusedLidarDriver` to a string."""
    return f"Removed unused lidar driver container {log.lidar_id}."

def format_cannot_remove_unused_lidar_driver(log: _logs_pb.CannotRemoveUnusedLidarDriver) -> str:
    """Formats log `CannotRemoveUnusedLidarDriver` to a string."""
    return f"Cannot remove unused lidar driver container {log.lidar_id}: {log.details}."

def format_lidar_driver_gc_failure(log: _logs_pb.LidarDriverGcFailure) -> str:
    """Formats log `LidarDriverGcFailure` to a string."""
    return f"Error encountered while removing unused lidar driver containers: {log.details}."

def format_id_space_exhausted(log: _logs_pb.IdSpaceExhausted) -> str:
    """Formats log `IdSpaceExhausted` to a string."""
    return f"Cannot allocate new identifier with prefix \"{log.prefix}\"."

def format_preprocessing_to_point_aggregator_points_skipped(log: _logs_pb.PreprocessingToPointAggregatorPointsSkipped) -> str:
    """Formats log `PreprocessingToPointAggregatorPointsSkipped` to a string."""
    return f"The point cloud publishing to the point aggregator service has been skipped {log.num_skipped_points} time(s) in the last {log.check_interval}."

def format_min_msg_interval_less_than_threshold(log: _logs_pb.MinMsgIntervalLessThanThreshold) -> str:
    """Formats log `MinMsgIntervalLessThanThreshold` to a string."""
    return f"Discarding lidar points from {log.lidar_id} since the time interval between two point-cloud messages is too close (<{log.threshold}). Adjust the Min-Message Interval parameter to change this behavior."

def format_failed_to_cleanup_ros_warning(log: _logs_pb.FailedToCleanupRosWarning) -> str:
    """Formats log `FailedToCleanupRosWarning` to a string."""
    return f"Failed to clean up ROS nodes and processes: {log.details}"

def format_rpc_disconnected_warning(log: _logs_pb.RpcDisconnectedWarning) -> str:
    """Formats log `RpcDisconnectedWarning` to a string."""
    return f"RPC request {log.request_name} to {log.endpoint} failed since endpoint is disconnected."

def format_rpc_unhandled_error(log: _logs_pb.RpcUnhandledError) -> str:
    """Formats log `RpcUnhandledError` to a string."""
    return f"RPC request handler for {log.request_name} failed due to an unhandled internal error: {log.details}."

def format_time_diff_out_of_range_warning(log: _logs_pb.TimeDiffOutOfRangeWarning) -> str:
    """Formats log `TimeDiffOutOfRangeWarning` to a string."""
    return f"Time difference is out of range: {log.diff}. Previous timestamp: {log.prev_time}, Current timestamp: {log.curr_time}."

def format_tensorrt_log(log: _logs_pb.TensorrtLog) -> str:
    """Formats log `TensorrtLog` to a string."""
    return f"[TensorRT] {log.log}"

def format_building_tensorrt_engine_info(log: _logs_pb.BuildingTensorrtEngineInfo) -> str:
    """Formats log `BuildingTensorrtEngineInfo` to a string."""
    return f"Building TensorRT engine since the model has changed or the engine is not available. This may take a while."

def format_loading_tensorrt_engine_info(log: _logs_pb.LoadingTensorrtEngineInfo) -> str:
    """Formats log `LoadingTensorrtEngineInfo` to a string."""
    return f"Loading TensorRT engine. This may take a while."

def format_calibration_map_not_found(log: _logs_pb.CalibrationMapNotFound) -> str:
    """Formats log `CalibrationMapNotFound` to a string."""
    return f"The calibration map {log.path} was not found."

def format_calibration_map_not_valid(log: _logs_pb.CalibrationMapNotValid) -> str:
    """Formats log `CalibrationMapNotValid` to a string."""
    return f"The calibration map {log.path} is not valid."

def format_calibration_map_path_already_exists(log: _logs_pb.CalibrationMapPathAlreadyExists) -> str:
    """Formats log `CalibrationMapPathAlreadyExists` to a string."""
    return f"The calibration map path {log.path} already exists."

def format_failed_to_save_calibration_map(log: _logs_pb.FailedToSaveCalibrationMap) -> str:
    """Formats log `FailedToSaveCalibrationMap` to a string."""
    return f"Failed to save the calibration map to {log.path}."

def format_failed_to_remove_calibration_map(log: _logs_pb.FailedToRemoveCalibrationMap) -> str:
    """Formats log `FailedToRemoveCalibrationMap` to a string."""
    return f"Failed to remove the calibration map at {log.path}."

def format_failed_to_iterate_in_directory(log: _logs_pb.FailedToIterateInDirectory) -> str:
    """Formats log `FailedToIterateInDirectory` to a string."""
    return f"Failed to iterate in the {log.directory_name} directory {log.directory_path}: {log.details}."

def format_map_based_calibration_without_map_loading(log: _logs_pb.MapBasedCalibrationWithoutMapLoading) -> str:
    """Formats log `MapBasedCalibrationWithoutMapLoading` to a string."""
    return f"The calibration map must be running before triggering the map-based calibration."

def format_map_based_calibration_already_running(log: _logs_pb.MapBasedCalibrationAlreadyRunning) -> str:
    """Formats log `MapBasedCalibrationAlreadyRunning` to a string."""
    return f"The map-based calibration is already running."

def format_cancel_map_based_calibration_not_running(log: _logs_pb.CancelMapBasedCalibrationNotRunning) -> str:
    """Formats log `CancelMapBasedCalibrationNotRunning` to a string."""
    return f"A cancel map-based calibration request has been received while the calibration is not running."

def format_bag_stream_not_found(log: _logs_pb.BagStreamNotFound) -> str:
    """Formats log `BagStreamNotFound` to a string."""
    return f"The stream {log.stream_id} was not found from {log.bag_path}."

def format_evaluation_bag_started_info(log: _logs_pb.EvaluationBagStartedInfo) -> str:
    """Formats log `EvaluationBagStartedInfo` to a string."""
    return f"Evaluation bag started for {log.bag_path}"

def format_evaluation_bag_finished_info(log: _logs_pb.EvaluationBagFinishedInfo) -> str:
    """Formats log `EvaluationBagFinishedInfo` to a string."""
    return f"Evaluation bag finished for {log.bag_path}"

def format_bag_not_found(log: _logs_pb.BagNotFound) -> str:
    """Formats log `BagNotFound` to a string."""
    return f"The bag {log.bag_path} was not found."

def format_building_pipeline_info(log: _logs_pb.BuildingPipelineInfo) -> str:
    """Formats log `BuildingPipelineInfo` to a string."""
    return f"Building the {log.pipeline_name} pipeline."

def format_bag_is_not_evaluation(log: _logs_pb.BagIsNotEvaluation) -> str:
    """Formats log `BagIsNotEvaluation` to a string."""
    return f"The bag {log.bag_path} is not an evaluation bag."

def format_horus_bag_running(log: _logs_pb.HorusBagRunning) -> str:
    """Formats log `HorusBagRunning` to a string."""
    return f"A Horus bag is currently running."

def format_auto_ground_calibration_warning(log: _logs_pb.AutoGroundCalibrationWarning) -> str:
    """Formats log `AutoGroundCalibrationWarning` to a string."""
    return f"Automatic ground calibration failed for the following lidars: {log.failed_lidar_ids}."

def format_auto_ground_calibration_error(log: _logs_pb.AutoGroundCalibrationError) -> str:
    """Formats log `AutoGroundCalibrationError` to a string."""
    return f"Automatic ground calibration failed for all lidars."

def format_object_detector_not_loaded_warning(log: _logs_pb.ObjectDetectorNotLoadedWarning) -> str:
    """Formats log `ObjectDetectorNotLoadedWarning` to a string."""
    return f"The object detector is not loaded yet. Temporarily using rule based detection which may result in degraded detection results."

def format_calibration_is_running_error(log: _logs_pb.CalibrationIsRunningError) -> str:
    """Formats log `CalibrationIsRunningError` to a string."""
    return f"A calibration process is already running."

def format_model_init_in_progress_info(log: _logs_pb.ModelInitInProgressInfo) -> str:
    """Formats log `ModelInitInProgressInfo` to a string."""
    return f"{log.component} will start processing data once all models have finished initializing."

def format_rpc_timeout_with_resolution_warning(log: _logs_pb.RpcTimeoutWithResolutionWarning) -> str:
    """Formats log `RpcTimeoutWithResolutionWarning` to a string."""
    return f"RPC request \"{log.request_name}\" to \"{log.endpoint}\" timed out after {log.elapsed}. Resolution: {log.resolution}"

def format_calibration_was_cancelled_info(log: _logs_pb.CalibrationWasCancelledInfo) -> str:
    """Formats log `CalibrationWasCancelledInfo` to a string."""
    return f"The {log.calibration_process_name} process was cancelled."

def format_calibration_map_recording_failed_to_start(log: _logs_pb.CalibrationMapRecordingFailedToStart) -> str:
    """Formats log `CalibrationMapRecordingFailedToStart` to a string."""
    return f"Failed to start recording the calibration map: {log.path}"

def format_detection_pipeline_requested_reset_info(log: _logs_pb.DetectionPipelineRequestedResetInfo) -> str:
    """Formats log `DetectionPipelineRequestedResetInfo` to a string."""
    return f"The detection pipeline has been requested to be reset because {log.reason}."

def format_preprocessing_service_pipeline_unavailable(log: _logs_pb.PreprocessingServicePipelineUnavailable) -> str:
    """Formats log `PreprocessingServicePipelineUnavailable` to a string."""
    return f"Preprocessing service pipeline is not available yet due to {log.reason}."

def format_circular_recording_disabled_warning(log: _logs_pb.CircularRecordingDisabledWarning) -> str:
    """Formats log `CircularRecordingDisabledWarning` to a string."""
    return f"The timeframe snapshot is disabled: {log.reason}"

def format_snapshot_already_running_warning(log: _logs_pb.SnapshotAlreadyRunningWarning) -> str:
    """Formats log `SnapshotAlreadyRunningWarning` to a string."""
    return f"A snapshot is already running."

def format_active_project_changed_info(log: _logs_pb.ActiveProjectChangedInfo) -> str:
    """Formats log `ActiveProjectChangedInfo` to a string."""
    return f"The active project has been changed."

def format_project_config_updated_info(log: _logs_pb.ProjectConfigUpdatedInfo) -> str:
    """Formats log `ProjectConfigUpdatedInfo` to a string."""
    return f"The project configuration has been updated."

def format_invalid_lidar_timestamp(log: _logs_pb.InvalidLidarTimestamp) -> str:
    """Formats log `InvalidLidarTimestamp` to a string."""
    return f"Invalid timestamp {log.timestamp} sent by lidar {log.lidar_id}."

def format_calibration_accumulating_points_info(log: _logs_pb.CalibrationAccumulatingPointsInfo) -> str:
    """Formats log `CalibrationAccumulatingPointsInfo` to a string."""
    return f"Calibration is accumulating points for {log.time}"

def format_sparse_noise_filter_usage_non_rotational_lidars(log: _logs_pb.SparseNoiseFilterUsageNonRotationalLidars) -> str:
    """Formats log `SparseNoiseFilterUsageNonRotationalLidars` to a string."""
    return f"The sparse noise filter cannot be used with non-rotational lidars."

def format_file_write_warning(log: _logs_pb.FileWriteWarning) -> str:
    """Formats log `FileWriteWarning` to a string."""
    return f"Failed to write to file \"{log.filename}\": \"{log.details}\"."

def format_license_forbidden_feature(log: _logs_pb.LicenseForbiddenFeature) -> str:
    """Formats log `LicenseForbiddenFeature` to a string."""
    return f"{log.feature_name} is not allowed by the current license."

def format_failed_to_update_configuration(log: _logs_pb.FailedToUpdateConfiguration) -> str:
    """Formats log `FailedToUpdateConfiguration` to a string."""
    return f"Failed to update the configuration: {log.details}."

def format_obstruction_detector_bad_reference_warning(log: _logs_pb.ObstructionDetectorBadReferenceWarning) -> str:
    """Formats log `ObstructionDetectorBadReferenceWarning` to a string."""
    return f"The obstruction detector reference is not valid since it contains zero points."

def format_project_file_invalid_permissions_error(log: _logs_pb.ProjectFileInvalidPermissionsError) -> str:
    """Formats log `ProjectFileInvalidPermissionsError` to a string."""
    return f"Project file \"{log.filename}\" has invalid permissions. Please restart Horus to fix the issue."

def format_pipeline_scheduler_error(log: _logs_pb.PipelineSchedulerError) -> str:
    """Formats log `PipelineSchedulerError` to a string."""
    return f"Pipeline scheduler error: {log.details}."

def format_multi_lidar_calibration_warning(log: _logs_pb.MultiLidarCalibrationWarning) -> str:
    """Formats log `MultiLidarCalibrationWarning` to a string."""
    return f"Multi lidar calibration failed for the following lidars: {log.failed_lidar_ids}."

def format_multi_lidar_calibration_error(log: _logs_pb.MultiLidarCalibrationError) -> str:
    """Formats log `MultiLidarCalibrationError` to a string."""
    return f"Multi lidar calibration failed for all lidars."

def format_old_bag_warning(log: _logs_pb.OldBagWarning) -> str:
    """Formats log `OldBagWarning` to a string."""
    return f"The bag file {log.bag_path} with version {log.version} is old. Please consider updating it with the API, refer to API documentation."

def format_upgrading_bag_info(log: _logs_pb.UpgradingBagInfo) -> str:
    """Formats log `UpgradingBagInfo` to a string."""
    return f"Upgrading bag file {log.bag_path} to latest version."

def format_bag_calibration_save_failed(log: _logs_pb.BagCalibrationSaveFailed) -> str:
    """Formats log `BagCalibrationSaveFailed` to a string."""
    return f"Failed to save the bag calibration: {log.details}."

def format_bag_upgrade_failed(log: _logs_pb.BagUpgradeFailed) -> str:
    """Formats log `BagUpgradeFailed` to a string."""
    return f"Failed to upgrade the bag file {log.bag_path}: {log.details}."

def format_unknown_lidar_error(log: _logs_pb.UnknownLidarError) -> str:
    """Formats log `UnknownLidarError` to a string."""
    return f"Received points from unknown lidar {log.lidar_id}."

def format_invalid_point_cloud_warning(log: _logs_pb.InvalidPointCloudWarning) -> str:
    """Formats log `InvalidPointCloudWarning` to a string."""
    return f"Invalid point cloud received from lidar {log.lidar_id}: {log.reason}"

def format_lidar_is_dropping_packets(log: _logs_pb.LidarIsDroppingPackets) -> str:
    """Formats log `LidarIsDroppingPackets` to a string."""
    return f"The lidar {log.lidar_id} is dropping packets (dropped {log.num_total_dropped_packets} packets out of {log.num_total_expected_packets})."

def format_removed_invalid_lidars_from_config_warning(log: _logs_pb.RemovedInvalidLidarsFromConfigWarning) -> str:
    """Formats log `RemovedInvalidLidarsFromConfigWarning` to a string."""
    return f"Some deprecated lidars have been found in the loaded project. These invalid lidars were removed from the active project but a backup config file has been saved in {log.backup_path}."

def format_calibration_map_recording_failed(log: _logs_pb.CalibrationMapRecordingFailed) -> str:
    """Formats log `CalibrationMapRecordingFailed` to a string."""
    return f"Failed recording the calibration map: {log.message}"

def format_detection_node_not_found_error(log: _logs_pb.DetectionNodeNotFoundError) -> str:
    """Formats log `DetectionNodeNotFoundError` to a string."""
    return f"Detection node {log.node_name} not found."

def format_created_version_backup_info(log: _logs_pb.CreatedVersionBackupInfo) -> str:
    """Formats log `CreatedVersionBackupInfo` to a string."""
    return f"Horus upgraded from {log.old_horus_version} to {log.new_horus_version}. The project was backed up to {log.backup_path} before being upgraded."

def format_ply_file_load_failed_error(log: _logs_pb.PlyFileLoadFailedError) -> str:
    """Formats log `PlyFileLoadFailedError` to a string."""
    return f"Failed to load PLY file {log.file_path}: {log.details}"

def format_hesai_driver_lifecycle(log: _logs_pb.HesaiDriverLifecycle) -> str:
    """Formats log `HesaiDriverLifecycle` to a string."""
    return f"Hesai XT32 driver {log.action} for LiDAR {log.lidar_id}."

def format_hesai_driver_error(log: _logs_pb.HesaiDriverError) -> str:
    """Formats log `HesaiDriverError` to a string."""
    return f"Hesai XT32 driver error: {log.details}"

def format_hesai_packet_processing_failed(log: _logs_pb.HesaiPacketProcessingFailed) -> str:
    """Formats log `HesaiPacketProcessingFailed` to a string."""
    return f"Failed to process packet for Hesai LiDAR {log.lidar_id}: {log.details}"

def format_hesai_correction_file_error(log: _logs_pb.HesaiCorrectionFileError) -> str:
    """Formats log `HesaiCorrectionFileError` to a string."""
    return f"Failed to load Hesai XT32 correction file {log.file_type}: {log.details}"

def format_hesai_packet_statistics(log: _logs_pb.HesaiPacketStatistics) -> str:
    """Formats log `HesaiPacketStatistics` to a string."""
    return f"Hesai packet statistics - Received: {log.packets_received}, Published: {log.packets_published}, Dropped: {log.packets_dropped}, Decode Failed: {log.packets_decode_failed}, Success Rate: {log.success_rate} %"

def format_ply_file_write_failed_error(log: _logs_pb.PlyFileWriteFailedError) -> str:
    """Formats log `PlyFileWriteFailedError` to a string."""
    return f"Failed to write PLY file {log.file_path}: {log.details}"

def format_project_save_error(log: _logs_pb.ProjectSaveError) -> str:
    """Formats log `ProjectSaveError` to a string."""
    return f"Failed to save project: {log.error_message}."

def format_save_static_environment_success(log: _logs_pb.SaveStaticEnvironmentSuccess) -> str:
    """Formats log `SaveStaticEnvironmentSuccess` to a string."""
    return f"Saved static environment to {log.path}"

def format_save_static_environment_failed(log: _logs_pb.SaveStaticEnvironmentFailed) -> str:
    """Formats log `SaveStaticEnvironmentFailed` to a string."""
    return f"Failed to serialize static environment to {log.path}: {log.details}"

def format_load_static_environment_success(log: _logs_pb.LoadStaticEnvironmentSuccess) -> str:
    """Formats log `LoadStaticEnvironmentSuccess` to a string."""
    return f"Static environment loaded from {log.path}"

def format_load_static_environment_failed(log: _logs_pb.LoadStaticEnvironmentFailed) -> str:
    """Formats log `LoadStaticEnvironmentFailed` to a string."""
    return f"Failed to load static environment from {log.path}: {log.details}"

def format_attempt_to_inject_invalid_lidar_id_warning(log: _logs_pb.AttemptToInjectInvalidLidarIdWarning) -> str:
    """Formats log `AttemptToInjectInvalidLidarIdWarning` to a string."""
    return f"Attempt to inject invalid lidar ID {log.lidar_id} into the system."

def format_reset_bundled_packet_due_to_unexpected_packet(log: _logs_pb.ResetBundledPacketDueToUnexpectedPacket) -> str:
    """Formats log `ResetBundledPacketDueToUnexpectedPacket` to a string."""
    return f"Resetting bundled packet due to unexpected packet from lidar {log.lidar_id}."

def format_packet_bundler_dropped_packets_warning(log: _logs_pb.PacketBundlerDroppedPacketsWarning) -> str:
    """Formats log `PacketBundlerDroppedPacketsWarning` to a string."""
    return f"Lidar {log.lidar_id} dropped {log.num_dropped} packets over {log.duration}"

def format_packet_bundler_frame_jump_warning(log: _logs_pb.PacketBundlerFrameJumpWarning) -> str:
    """Formats log `PacketBundlerFrameJumpWarning` to a string."""
    return f"Frame sequence jump detected on lidar {log.lidar_id}: from {log.frame_id} to {log.next_frame_id}"

def format_lidar_correction_loading_success(log: _logs_pb.LidarCorrectionLoadingSuccess) -> str:
    """Formats log `LidarCorrectionLoadingSuccess` to a string."""
    return f"Successfully loaded {log.correction_type} corrections from the lidar"

def format_lidar_correction_loading_failure(log: _logs_pb.LidarCorrectionLoadingFailure) -> str:
    """Formats log `LidarCorrectionLoadingFailure` to a string."""
    return f"Failed to load {log.correction_type} corrections from the lidar ({log.details}); using default correction values"

def format_hesai_packet_statistics_lidar(log: _logs_pb.HesaiPacketStatisticsLidar) -> str:
    """Formats log `HesaiPacketStatisticsLidar` to a string."""
    return f"[{log.lidar_id}] Hesai Packet Statistics - Received: {log.packets_received}, Published: {log.packets_published}, Dropped: {log.packets_dropped}, Decode Failed: {log.packets_decode_failed}, Success Rate: {log.success_rate} %"