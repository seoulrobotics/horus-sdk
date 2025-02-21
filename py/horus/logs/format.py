import typing
from ..pb.logs.message_pb2 import LogData

_formatters: typing.Dict[int, typing.Callable[[LogData], str]] = {
    LogData.GENERIC_FIELD_NUMBER: lambda m: f"{m.generic.message}",
    LogData.OOM_FIELD_NUMBER: lambda m: f"Out of memory",
    LogData.RPC_CONNECTION_ERROR_FIELD_NUMBER: lambda m: f"Cannot initiate connection to {m.rpc_connection_error.target_service} at {m.rpc_connection_error.target_uri}: {m.rpc_connection_error.details}",
    LogData.INVALID_PROJECT_NAME_FIELD_NUMBER: lambda m: f"Project name `{m.invalid_project_name.name}` is invalid: {m.invalid_project_name.details}",
    LogData.PROJECT_NOT_FOUND_FIELD_NUMBER: lambda m: f"Project `{m.project_not_found.name}` not found",
    LogData.PROJECT_ALREADY_EXISTS_FIELD_NUMBER: lambda m: f"Project `{m.project_already_exists.name}` already exists",
    LogData.INVALID_CONFIGURATION_FIELD_NUMBER: lambda m: f"Configuration is invalid",
    LogData.ENTITY_NOT_FOUND_FIELD_NUMBER: lambda m: f"{m.entity_not_found.entity_type} `{m.entity_not_found.id}` not found",
    LogData.ACTIVE_PROJECT_CANNOT_BE_DELETED_FIELD_NUMBER: lambda m: f"Active project cannot be deleted",
    LogData.RPC_DISCONNECTION_ERROR_FIELD_NUMBER: lambda m: f"Cannot terminate connection to {m.rpc_disconnection_error.target_service} at {m.rpc_disconnection_error.target_uri}: {m.rpc_disconnection_error.details}",
    LogData.DROPPED_LOGS_FIELD_NUMBER: lambda m: f"Dropped {m.dropped_logs.n} log messages",
    LogData.OPENED_PROJECT_FIELD_NUMBER: lambda m: f"Opened project {m.opened_project.project_name}",
    LogData.CREATED_PROJECT_FIELD_NUMBER: lambda m: f"Created project {m.created_project.project_name} from {m.created_project.source_name}",
    LogData.CONFIG_UNAVAILABLE_FIELD_NUMBER: lambda m: f"Configuration is unavailable; request cannot be processed.",
    LogData.INVALID_REQUEST_FIELD_NUMBER: lambda m: f"Invalid request received: {m.invalid_request.details}",
    LogData.SANITY_CHECK_ERROR_FIELD_NUMBER: lambda m: f"{m.sanity_check_error.sanity_check_name} failed: {m.sanity_check_error.details}",
    LogData.BAG_FAILED_TO_OPEN_FIELD_NUMBER: lambda m: f"The bag file ({m.bag_failed_to_open.horus_bag_path}) could not be opened: {m.bag_failed_to_open.details}",
    LogData.BAG_FAILED_TO_CLOSE_FIELD_NUMBER: lambda m: f"Bag file could not be closed: {m.bag_failed_to_close.details}",
    LogData.BAG_CONVERSION_FAILED_FIELD_NUMBER: lambda m: f"Bag file conversion failed: {m.bag_conversion_failed.details}",
    LogData.BAG_FAILED_TO_WRITE_FIELD_NUMBER: lambda m: f"Bag file writing failed {m.bag_failed_to_write.name}: {m.bag_failed_to_write.details}",
    LogData.CALIBRATION_ERROR_FIELD_NUMBER: lambda m: f"Calibration failed: {m.calibration_error.details}",
    LogData.PROJECT_MANAGER_FAILED_TO_START_RECORDING_FIELD_NUMBER: lambda m: f"Failed to start recording: {m.project_manager_failed_to_start_recording.details}",
    LogData.PROJECT_MANAGER_FAILED_TO_STOP_RECORDING_FIELD_NUMBER: lambda m: f"Failed to stop recording: {m.project_manager_failed_to_stop_recording.details}",
    LogData.SERVICE_CONNECTION_TIMED_OUT_FIELD_NUMBER: lambda m: f"The connection with the requested {m.service_connection_timed_out.node_type} node with ID {m.service_connection_timed_out.node_id} timed out",
    LogData.BAG_RECORDER_ALREADY_RUNNING_FIELD_NUMBER: lambda m: f"Bag recorder is already running for {m.bag_recorder_already_running.path}",
    LogData.LICENSE_SERVER_CONNECTION_ERROR_FIELD_NUMBER: lambda m: f"Could not connect to license server: {m.license_server_connection_error.details}.",
    LogData.LICENSE_ERROR_FIELD_NUMBER: lambda m: f"Could not read license: {m.license_error.details}.",
    LogData.LICENSE_NOT_FOUND_ERROR_FIELD_NUMBER: lambda m: f"Found no valid license.",
    LogData.LICENSE_EXPIRED_ERROR_FIELD_NUMBER: lambda m: f"License expired on {m.license_expired_error.expiration_time}.",
    LogData.LICENSE_EXCEEDED_ERROR_FIELD_NUMBER: lambda m: f"Project exceeds number of LiDARs allowed by license ({m.license_exceeded_error.lidar_count} > {m.license_exceeded_error.max_lidar_count}).",
    LogData.LICENSE_HOST_MACHINE_ERROR_FIELD_NUMBER: lambda m: f"Could not validate license with current hardware - {m.license_host_machine_error.details}.",
    LogData.LICENSE_PRIVILEGE_ERROR_FIELD_NUMBER: lambda m: f"Endpoint requires \"{m.license_privilege_error.missing_privilege}\" privilege: {m.license_privilege_error.level_error}.",
    LogData.LICENSE_ACTIVE_INFO_FIELD_NUMBER: lambda m: f"License is active.",
    LogData.MULTIPLE_LICENSES_WARNING_FIELD_NUMBER: lambda m: f"Found {m.multiple_licenses_warning.non_expired_licenses_count} non-expired concurrent licenses. Only \"{m.multiple_licenses_warning.active_license_filename}\" is used.",
    LogData.LICENSE_CURRENT_LICENSE_STATUS_INFO_FIELD_NUMBER: lambda m: f"Current license accepts up to {m.license_current_license_status_info.max_lidar_count} and expires on {m.license_current_license_status_info.expiration_time}.",
    LogData.BAG_RECORDING_STOPPED_INFO_FIELD_NUMBER: lambda m: f"Bag recording stopped for {m.bag_recording_stopped_info.path}: {m.bag_recording_stopped_info.details}",
    LogData.BAG_RECORDING_FAILED_TO_START_FIELD_NUMBER: lambda m: f"Bag recording failed to start: {m.bag_recording_failed_to_start.details}",
    LogData.BAG_RECORDING_STARTED_INFO_FIELD_NUMBER: lambda m: f"Bag recording started for {m.bag_recording_started_info.path}",
    LogData.REPLAY_RESTARTED_INFO_FIELD_NUMBER: lambda m: f"Replay Restarted",
    LogData.INPUT_SOURCE_CHANGE_REQUESTED_INFO_FIELD_NUMBER: lambda m: f"Input source {m.input_source_change_requested_info.source} requested",
    LogData.INPUT_SOURCE_SWITCHED_INFO_FIELD_NUMBER: lambda m: f"Input source changed to {m.input_source_switched_info.source}",
    LogData.RPC_TIMEOUT_WARNING_FIELD_NUMBER: lambda m: f"RPC request {m.rpc_timeout_warning.request_name} to {m.rpc_timeout_warning.endpoint} timed out after {m.rpc_timeout_warning.elapsed}.",
    LogData.CANNOT_WRITE_LOG_FILE_FIELD_NUMBER: lambda m: f"Notification service cannot write log in {m.cannot_write_log_file.path}: {m.cannot_write_log_file.details}",
    LogData.POINT_CLOUD_PARSING_FAILURE_WARNING_FIELD_NUMBER: lambda m: f"Point cloud parsing failed: {m.point_cloud_parsing_failure_warning.details}",
    LogData.LIDAR_IS_DEAD_FIELD_NUMBER: lambda m: f"The lidar {m.lidar_is_dead.lidar_id} is considered dead. No data has been received for a while.",
    LogData.LIDAR_IS_NOT_DEAD_ANYMORE_FIELD_NUMBER: lambda m: f"The lidar {m.lidar_is_not_dead_anymore.lidar_id} is not considered dead anymore. A point cloud has been received again after some time.",
    LogData.LIDAR_IS_OBSTRUCTED_FIELD_NUMBER: lambda m: f"The lidar {m.lidar_is_obstructed.lidar_id} is obstructed.",
    LogData.LIDAR_IS_NOT_OBSTRUCTED_ANYMORE_FIELD_NUMBER: lambda m: f"The lidar {m.lidar_is_not_obstructed_anymore.lidar_id} is not obstructed anymore.",
    LogData.LIDAR_IS_TILTED_FIELD_NUMBER: lambda m: f"The lidar {m.lidar_is_tilted.lidar_id} is tilted.",
    LogData.LIDAR_IS_NOT_TILTED_ANYMORE_FIELD_NUMBER: lambda m: f"The lidar {m.lidar_is_not_tilted_anymore.lidar_id} is not tilted anymore.",
    LogData.LIDAR_HAS_BEEN_AUTOMATICALLY_RECALIBRATED_FIELD_NUMBER: lambda m: f"The lidar {m.lidar_has_been_automatically_recalibrated.lidar_id} has been automatically re-calibrated.",
    LogData.RECEIVED_FIRST_DATA_FOR_LIDAR_FIELD_NUMBER: lambda m: f"Data has been received for the first time for the lidar {m.received_first_data_for_lidar.lidar_id}.",
    LogData.TERMINATION_FAILURE_ERROR_FIELD_NUMBER: lambda m: f"Failed to destruct {m.termination_failure_error.component}: {m.termination_failure_error.details}",
    LogData.FRAME_PROCESSING_ERROR_FIELD_NUMBER: lambda m: f"The Frame Task Scheduler failed to process frame task: {m.frame_processing_error.details}",
    LogData.THREAD_POOL_UNAVAILABLE_ERROR_FIELD_NUMBER: lambda m: f"Thread pool is not available: {m.thread_pool_unavailable_error.consequence}",
    LogData.INVALID_ARGUMENT_FIELD_NUMBER: lambda m: f"Invalid argument: {m.invalid_argument.details}",
    LogData.COMPONENT_INITIALIZATION_FAILURE_FATAL_FIELD_NUMBER: lambda m: f"Failed to initialize {m.component_initialization_failure_fatal.component}: {m.component_initialization_failure_fatal.details}",
    LogData.UNHANDLED_ENUM_CASE_ERROR_FIELD_NUMBER: lambda m: f"Unhandled enum case {m.unhandled_enum_case_error.case_name} at {m.unhandled_enum_case_error.location}",
    LogData.BAG_EMPTY_ERROR_FIELD_NUMBER: lambda m: f"The replay bag file is empty.",
    LogData.DISCARDING_DATA_ERROR_FIELD_NUMBER: lambda m: f"Discarding data in {m.discarding_data_error.component}: {m.discarding_data_error.details}",
    LogData.DISCARDING_DATA_WARNING_FIELD_NUMBER: lambda m: f"Discarding data in {m.discarding_data_warning.component}: {m.discarding_data_warning.details}.",
    LogData.NOTHING_TO_PROCESS_FIELD_NUMBER: lambda m: f"Nothing to process: {m.nothing_to_process.details}",
    LogData.INVALID_COMPONENT_CONFIGURATION_FIELD_NUMBER: lambda m: f"Invalid configuration for {m.invalid_component_configuration.component}: {m.invalid_component_configuration.details}",
    LogData.CHANNEL_RECEIVER_NOT_FOUND_WARNING_FIELD_NUMBER: lambda m: f"Channel receiver {m.channel_receiver_not_found_warning.key} not found",
    LogData.MODEL_LOAD_FAILURE_FIELD_NUMBER: lambda m: f"Model {m.model_load_failure.model} failed to load: {m.model_load_failure.details}",
    LogData.MODEL_EXECUTION_FAILURE_ERROR_FIELD_NUMBER: lambda m: f"Model {m.model_execution_failure_error.model} failed to execute: {m.model_execution_failure_error.details}",
    LogData.SERVICE_READY_INFO_FIELD_NUMBER: lambda m: f"{m.service_ready_info.service} service is ready",
    LogData.MODEL_PREPARING_INFO_FIELD_NUMBER: lambda m: f"Preparing model {m.model_preparing_info.model}, it may take several seconds to stabilize.",
    LogData.MODEL_INITIALIZED_INFO_FIELD_NUMBER: lambda m: f"{m.model_initialized_info.name} model has been initialized and is ready for use.",
    LogData.MODEL_INITIALIZATION_FAILURE_WARNING_FIELD_NUMBER: lambda m: f"Failed to initialize model {m.model_initialization_failure_warning.model}: {m.model_initialization_failure_warning.details}",
    LogData.ROS_SPINNER_STOPPED_WARNING_FIELD_NUMBER: lambda m: f"ROS spinner stopped: {m.ros_spinner_stopped_warning.details}",
    LogData.ACTOR_SYSTEM_UNAVAILABLE_ERROR_FIELD_NUMBER: lambda m: f"Actor system is not available. Please reinstantiate the pipeline.",
    LogData.CONFIG_NODE_NOT_FOUND_ERROR_FIELD_NUMBER: lambda m: f"Configuration for node {m.config_node_not_found_error.node} is unavailable",
    LogData.BAG_TIMESTAMP_OUT_OF_ORDER_ERROR_FIELD_NUMBER: lambda m: f"Timestamps are out of order: {m.bag_timestamp_out_of_order_error.prior_timestamp} > {m.bag_timestamp_out_of_order_error.current_timestamp}",
    LogData.BAG_REPLAY_UNEXPECTED_TIMESTAMP_ERROR_FIELD_NUMBER: lambda m: f"Unexpected timestamp: expected {m.bag_replay_unexpected_timestamp_error.expected_timestamp} but received {m.bag_replay_unexpected_timestamp_error.received_timestamp}",
    LogData.WEBSOCKET_CLOSED_INFO_FIELD_NUMBER: lambda m: f"WebSocket RPC connection to {m.websocket_closed_info.uri} closed",
    LogData.WEBSOCKET_OPENED_INFO_FIELD_NUMBER: lambda m: f"WebSocket RPC connection to {m.websocket_opened_info.uri} opened (as {m.websocket_opened_info.endpoint_is_server_client})",
    LogData.SUBSCRIBER_DISCONNECTED_INFO_FIELD_NUMBER: lambda m: f"Subscriber {m.subscriber_disconnected_info.name} disconnected ({m.subscriber_disconnected_info.uri})",
    LogData.THREAD_POOL_SLOWING_DOWN_WARNING_FIELD_NUMBER: lambda m: f"Thread pool is slowing down for {m.thread_pool_slowing_down_warning.number} times!",
    LogData.THREAD_POOL_NOT_RESPONDING_WARNING_FIELD_NUMBER: lambda m: f"Thread pool is not responding for {m.thread_pool_not_responding_warning.not_responding_for}.",
    LogData.THREAD_POOL_BROKEN_PROMISE_WARNING_FIELD_NUMBER: lambda m: f"Thread pool task future is no longer available. The thread pool might have been restarted during events such as input source change.",
    LogData.BOX_FITTING_POINTS_BELOW_BASE_WARNING_FIELD_NUMBER: lambda m: f"Some points are below the base Z-coordinate when fitting the box. This might affect the detection results.",
    LogData.FAILED_TO_REMOVE_STALE_POINTS_WARNING_FIELD_NUMBER: lambda m: f"Failed to remove stale points: {m.failed_to_remove_stale_points_warning.details}",
    LogData.IRREGULAR_BROADCASTING_PERIOD_WARNING_FIELD_NUMBER: lambda m: f"{m.irregular_broadcasting_period_warning.num_irregular} irregular broadcast(s) detected in the last {m.irregular_broadcasting_period_warning.duration} (mean deviation: {m.irregular_broadcasting_period_warning.mean_deviation}).",
    LogData.CLUSTERER_POINTS_OUT_OF_RANGE_WARNING_FIELD_NUMBER: lambda m: f"There are {m.clusterer_points_out_of_range_warning.num_points} points out of range in the clusterer. Some points: {m.clusterer_points_out_of_range_warning.sample_points}.",
    LogData.INTERNAL_ERROR_FIELD_NUMBER: lambda m: f"An internal error occurred: {m.internal_error.details}",
    LogData.INTERNAL_FATAL_FIELD_NUMBER: lambda m: f"An internal fatal error occurred: {m.internal_fatal.details}",
    LogData.SERVICE_STARTING_INFO_FIELD_NUMBER: lambda m: f"{m.service_starting_info.service} is starting",
    LogData.CONFIG_NODE_NOT_FOUND_FATAL_FIELD_NUMBER: lambda m: f"Configuration for node {m.config_node_not_found_fatal.node} is unavailable",
    LogData.SERVICE_SETUP_ERROR_FIELD_NUMBER: lambda m: f"Failed to setup: {m.service_setup_error.details}",
    LogData.FILESYSTEM_ERROR_FIELD_NUMBER: lambda m: f"Filesystem error: {m.filesystem_error.details}",
    LogData.INVALID_PRESET_WARNING_FIELD_NUMBER: lambda m: f"Tried to apply invalid preset {m.invalid_preset_warning.preset} due to {m.invalid_preset_warning.reason}, falling back to {m.invalid_preset_warning.fallback_name}.",
    LogData.WEBSOCKET_FAILED_CLEAR_PENDING_ERROR_FIELD_NUMBER: lambda m: f"WebSocket failed to clear pending queues: {m.websocket_failed_clear_pending_error.exception}",
    LogData.WEBSOCKET_FAILED_TO_STOP_ERROR_FIELD_NUMBER: lambda m: f"WebSocket failed to stop: {m.websocket_failed_to_stop_error.exception}",
    LogData.WEBSOCKET_FAILED_TO_CREATE_ERROR_FIELD_NUMBER: lambda m: f"Failed to create RPC server ws://{m.websocket_failed_to_create_error.host}:{m.websocket_failed_to_create_error.port}/: {m.websocket_failed_to_create_error.exception}",
    LogData.UNEXPECTED_RPC_ERROR_FIELD_NUMBER: lambda m: f"Unexpected RPC error: {m.unexpected_rpc_error.details}",
    LogData.LICENSE_POLL_FAILED_FIELD_NUMBER: lambda m: f"Failed to poll license server: {m.license_poll_failed.details}",
    LogData.LICENSE_EXPIRED_WARNING_FIELD_NUMBER: lambda m: f"License expired on {m.license_expired_warning.expiration_time}.",
    LogData.LICENSE_USAGE_EXCEEDED_WARNING_FIELD_NUMBER: lambda m: f"License count of {m.license_usage_exceeded_warning.lidar_count} exceeds allowed usage of {m.license_usage_exceeded_warning.max_lidar_count}",
    LogData.STATIC_THREAD_POOL_SLOW_TASK_WARNING_FIELD_NUMBER: lambda m: f"ThreadPool task took {m.static_thread_pool_slow_task_warning.time_elapsed} to complete task enqueued in {m.static_thread_pool_slow_task_warning.enqueue_location}",
    LogData.RPC_UNSUPPORTED_SERVICE_WARNING_FIELD_NUMBER: lambda m: f"Received request for unsupported service: {m.rpc_unsupported_service_warning.service_id}",
    LogData.WEBSOCKET_HANDLER_PROBLEM_FIELD_NUMBER: lambda m: f"WebSocket RPC handler encountered an error: {m.websocket_handler_problem.what}",
    LogData.WEBSOCKET_DESERIALIZE_ERROR_FIELD_NUMBER: lambda m: f"WebSocket RPC received message cannot be deserialized: {m.websocket_deserialize_error.what}",
    LogData.WEBSOCKET_EXPIRED_RPC_ENDPOINT_ERROR_FIELD_NUMBER: lambda m: f"Attempting to access expired WebSocket RPC endpoint {m.websocket_expired_rpc_endpoint_error.uri}",
    LogData.WEBSOCKET_QUEUE_OVERLOADED_WARNING_FIELD_NUMBER: lambda m: f"Websocket message queue overloaded: {m.websocket_queue_overloaded_warning.current} / {m.websocket_queue_overloaded_warning.max}",
    LogData.RPC_FAILED_TO_NOTIFY_WARNING_FIELD_NUMBER: lambda m: f"Could not notify {m.rpc_failed_to_notify_warning.notification_name} to {m.rpc_failed_to_notify_warning.service} {m.rpc_failed_to_notify_warning.uri}: {m.rpc_failed_to_notify_warning.what}",
    LogData.CONFIG_SUBSCRIPTION_FAILED_WARNING_FIELD_NUMBER: lambda m: f"Failed to subscribe to config: {m.config_subscription_failed_warning.details}",
    LogData.THREAD_POOL_CLAMPED_WORKERS_WARNING_FIELD_NUMBER: lambda m: f"Clamped thread pool workers to {m.thread_pool_clamped_workers_warning.clamped} instead of {m.thread_pool_clamped_workers_warning.requested}",
    LogData.STOPPING_HORUS_BAG_RECORDER_ALREADY_STOPPED_FIELD_NUMBER: lambda m: f"Trying to stop the Horus bag recorder which is already stopped.",
    LogData.RECORDER_CONFIG_UPDATE_WHILE_RUNNING_FIELD_NUMBER: lambda m: f"An update for the Horus bag recorder has been received while it was running. This update will not be taken into account until the next recording.",
    LogData.CLAMPING_DATA_WARNING_FIELD_NUMBER: lambda m: f"Clamping point cloud: {m.clamping_data_warning.details}",
    LogData.LIDAR_INCOMPATIBLE_VALUES_FIELD_NUMBER: lambda m: f"Lidars {m.lidar_incompatible_values.lidar_ip1} and {m.lidar_incompatible_values.lidar_ip2} have incompatible {m.lidar_incompatible_values.value_names}: {m.lidar_incompatible_values.value1} vs. {m.lidar_incompatible_values.value2}; {m.lidar_incompatible_values.resolution}.",
    LogData.CANNOT_DETERMINE_CONTAINER_ID_ERROR_FIELD_NUMBER: lambda m: f"Cannot determine current Docker container ID; unknown {m.cannot_determine_container_id_error.container_id_file_path} format.",
    LogData.STARTED_LIDAR_DRIVER_FIELD_NUMBER: lambda m: f"Started lidar driver container {m.started_lidar_driver.lidar_id}.",
    LogData.CANNOT_START_LIDAR_DRIVER_FIELD_NUMBER: lambda m: f"Cannot start lidar driver container {m.cannot_start_lidar_driver.lidar_id}: {m.cannot_start_lidar_driver.details}.",
    LogData.STOPPED_LIDAR_DRIVER_FIELD_NUMBER: lambda m: f"Stopped lidar driver container {m.stopped_lidar_driver.lidar_id}.",
    LogData.CANNOT_STOP_LIDAR_DRIVER_FIELD_NUMBER: lambda m: f"Cannot stop lidar driver container {m.cannot_stop_lidar_driver.lidar_id}: {m.cannot_stop_lidar_driver.details}.",
    LogData.RESTARTED_LIDAR_DRIVER_FIELD_NUMBER: lambda m: f"Restarted lidar driver container {m.restarted_lidar_driver.lidar_id}.",
    LogData.CANNOT_RESTART_LIDAR_DRIVER_FIELD_NUMBER: lambda m: f"Cannot restart lidar driver container {m.cannot_restart_lidar_driver.lidar_id}: {m.cannot_restart_lidar_driver.details}.",
    LogData.REMOVED_UNUSED_LIDAR_DRIVER_FIELD_NUMBER: lambda m: f"Removed unused lidar driver container {m.removed_unused_lidar_driver.lidar_id}.",
    LogData.CANNOT_REMOVE_UNUSED_LIDAR_DRIVER_FIELD_NUMBER: lambda m: f"Cannot remove unused lidar driver container {m.cannot_remove_unused_lidar_driver.lidar_id}: {m.cannot_remove_unused_lidar_driver.details}.",
    LogData.LIDAR_DRIVER_GC_FAILURE_FIELD_NUMBER: lambda m: f"Error encountered while removing unused lidar driver containers: {m.lidar_driver_gc_failure.details}.",
    LogData.ID_SPACE_EXHAUSTED_FIELD_NUMBER: lambda m: f"Cannot allocate new identifier with prefix \"{m.id_space_exhausted.prefix}\".",
    LogData.PREPROCESSING_TO_POINT_AGGREGATOR_POINTS_SKIPPED_FIELD_NUMBER: lambda m: f"The point cloud publishing to the point aggregator service has been skipped {m.preprocessing_to_point_aggregator_points_skipped.num_skipped_points} time(s) in the last {m.preprocessing_to_point_aggregator_points_skipped.check_interval}.",
    LogData.MIN_MSG_INTERVAL_LESS_THAN_THRESHOLD_FIELD_NUMBER: lambda m: f"Discarding lidar points from {m.min_msg_interval_less_than_threshold.lidar_id} since the time interval between two point-cloud messages is too close (<{m.min_msg_interval_less_than_threshold.threshold}). Adjust the Min-Message Interval parameter to change this behavior.",
    LogData.FAILED_TO_CLEANUP_ROS_WARNING_FIELD_NUMBER: lambda m: f"Failed to clean up ROS nodes and processes: {m.failed_to_cleanup_ros_warning.details}",
    LogData.RPC_DISCONNECTED_WARNING_FIELD_NUMBER: lambda m: f"RPC request {m.rpc_disconnected_warning.request_name} to {m.rpc_disconnected_warning.endpoint} failed since endpoint is disconnected.",
    LogData.RPC_UNHANDLED_ERROR_FIELD_NUMBER: lambda m: f"RPC request handler for {m.rpc_unhandled_error.request_name} failed due to an unhandled internal error: {m.rpc_unhandled_error.details}.",
    LogData.TIME_DIFF_OUT_OF_RANGE_WARNING_FIELD_NUMBER: lambda m: f"Time difference is out of range: {m.time_diff_out_of_range_warning.diff}. Previous timestamp: {m.time_diff_out_of_range_warning.prev_time}, Current timestamp: {m.time_diff_out_of_range_warning.curr_time}.",
    LogData.TENSORRT_LOG_FIELD_NUMBER: lambda m: f"[TensorRT] {m.tensorrt_log.log}",
    LogData.BUILDING_TENSORRT_ENGINE_INFO_FIELD_NUMBER: lambda m: f"Building TensorRT engine since the model has changed or the engine is not available. This may take a while.",
    LogData.LOADING_TENSORRT_ENGINE_INFO_FIELD_NUMBER: lambda m: f"Loading TensorRT engine. This may take a while.",
    LogData.CALIBRATION_MAP_NOT_FOUND_FIELD_NUMBER: lambda m: f"The calibration map {m.calibration_map_not_found.path} was not found.",
    LogData.CALIBRATION_MAP_NOT_VALID_FIELD_NUMBER: lambda m: f"The calibration map {m.calibration_map_not_valid.path} is not valid.",
    LogData.CALIBRATION_MAP_PATH_ALREADY_EXISTS_FIELD_NUMBER: lambda m: f"The calibration map path {m.calibration_map_path_already_exists.path} already exists.",
    LogData.FAILED_TO_SAVE_CALIBRATION_MAP_FIELD_NUMBER: lambda m: f"Failed to save the calibration map to {m.failed_to_save_calibration_map.path}.",
    LogData.FAILED_TO_REMOVE_CALIBRATION_MAP_FIELD_NUMBER: lambda m: f"Failed to remove the calibration map at {m.failed_to_remove_calibration_map.path}.",
    LogData.FAILED_TO_ITERATE_IN_DIRECTORY_FIELD_NUMBER: lambda m: f"Failed to iterate in the {m.failed_to_iterate_in_directory.directory_name} directory {m.failed_to_iterate_in_directory.directory_path}: {m.failed_to_iterate_in_directory.details}.",
    LogData.MAP_BASED_CALIBRATION_WITHOUT_MAP_LOADING_FIELD_NUMBER: lambda m: f"The calibration map must be running before triggering the map-based calibration.",
    LogData.MAP_BASED_CALIBRATION_ALREADY_RUNNING_FIELD_NUMBER: lambda m: f"The map-based calibration is already running.",
    LogData.CANCEL_MAP_BASED_CALIBRATION_NOT_RUNNING_FIELD_NUMBER: lambda m: f"A cancel map-based calibration request has been received while the calibration is not running.",
    LogData.BAG_STREAM_NOT_FOUND_FIELD_NUMBER: lambda m: f"The stream {m.bag_stream_not_found.stream_id} was not found from {m.bag_stream_not_found.bag_path}.",
    LogData.EVALUATION_BAG_STARTED_INFO_FIELD_NUMBER: lambda m: f"Evaluation bag started for {m.evaluation_bag_started_info.bag_path}",
    LogData.EVALUATION_BAG_FINISHED_INFO_FIELD_NUMBER: lambda m: f"Evaluation bag finished for {m.evaluation_bag_finished_info.bag_path}",
    LogData.BAG_NOT_FOUND_FIELD_NUMBER: lambda m: f"The bag {m.bag_not_found.bag_path} was not found.",
    LogData.BUILDING_PIPELINE_INFO_FIELD_NUMBER: lambda m: f"Building the {m.building_pipeline_info.pipeline_name} pipeline.",
    LogData.BAG_IS_NOT_EVALUATION_FIELD_NUMBER: lambda m: f"The bag {m.bag_is_not_evaluation.bag_path} is not an evaluation bag.",
    LogData.HORUS_BAG_RUNNING_FIELD_NUMBER: lambda m: f"A Horus bag is currently running.",
    LogData.AUTO_GROUND_CALIBRATION_WARNING_FIELD_NUMBER: lambda m: f"Automatic ground calibration failed for the following lidars: {m.auto_ground_calibration_warning.failed_lidar_ids}.",
    LogData.AUTO_GROUND_CALIBRATION_ERROR_FIELD_NUMBER: lambda m: f"Automatic ground calibration failed for all lidars.",
    LogData.OBJECT_DETECTOR_NOT_LOADED_WARNING_FIELD_NUMBER: lambda m: f"The object detector is not loaded yet. Temporarily using rule based detection which may result in degraded detection results.",
    LogData.CALIBRATION_IS_RUNNING_ERROR_FIELD_NUMBER: lambda m: f"A calibration process is already running.",
    LogData.MODEL_INIT_IN_PROGRESS_INFO_FIELD_NUMBER: lambda m: f"{m.model_init_in_progress_info.component} will start processing data once all models have finished initializing.",
    LogData.RPC_TIMEOUT_WITH_RESOLUTION_WARNING_FIELD_NUMBER: lambda m: f"RPC request \"{m.rpc_timeout_with_resolution_warning.request_name}\" to \"{m.rpc_timeout_with_resolution_warning.endpoint}\" timed out after {m.rpc_timeout_with_resolution_warning.elapsed}. Resolution: {m.rpc_timeout_with_resolution_warning.resolution}",
    LogData.CALIBRATION_WAS_CANCELLED_INFO_FIELD_NUMBER: lambda m: f"The {m.calibration_was_cancelled_info.calibration_process_name} process was cancelled.",
    LogData.CALIBRATION_MAP_RECORDING_FAILED_TO_START_FIELD_NUMBER: lambda m: f"Failed to start recording the calibration map: {m.calibration_map_recording_failed_to_start.path}",
    LogData.DETECTION_PIPELINE_REQUESTED_RESET_INFO_FIELD_NUMBER: lambda m: f"The detection pipeline has been requested to be reset because {m.detection_pipeline_requested_reset_info.reason}.",
    LogData.PREPROCESSING_SERVICE_PIPELINE_UNAVAILABLE_FIELD_NUMBER: lambda m: f"Preprocessing service pipeline is not available yet due to {m.preprocessing_service_pipeline_unavailable.reason}.",
    LogData.CIRCULAR_RECORDING_DISABLED_WARNING_FIELD_NUMBER: lambda m: f"The timeframe snapshot is disabled: {m.circular_recording_disabled_warning.reason}",
    LogData.SNAPSHOT_ALREADY_RUNNING_WARNING_FIELD_NUMBER: lambda m: f"A snapshot is already running.",
    LogData.ACTIVE_PROJECT_CHANGED_INFO_FIELD_NUMBER: lambda m: f"The active project has been changed.",
    LogData.PROJECT_CONFIG_UPDATED_INFO_FIELD_NUMBER: lambda m: f"The project configuration has been updated.",
    LogData.INVALID_LIDAR_TIMESTAMP_FIELD_NUMBER: lambda m: f"Invalid timestamp {m.invalid_lidar_timestamp.timestamp} sent by lidar {m.invalid_lidar_timestamp.lidar_id}.",
    LogData.CALIBRATION_ACCUMULATING_POINTS_INFO_FIELD_NUMBER: lambda m: f"Calibration is accumulating points for {m.calibration_accumulating_points_info.time}",
    LogData.SPARSE_NOISE_FILTER_USAGE_NON_ROTATIONAL_LIDARS_FIELD_NUMBER: lambda m: f"The sparse noise filter cannot be used with non-rotational lidars.",
    LogData.FILE_WRITE_ERROR_FIELD_NUMBER: lambda m: f"Failed to write to file \"{m.file_write_error.filename}\": \"{m.file_write_error.details}\".",
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
