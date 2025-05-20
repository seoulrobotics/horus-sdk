package internal

import (
	"fmt"

	"github.com/seoulrobotics/horus-sdk/go/proto/logs/logs_pb"
	"google.golang.org/protobuf/proto"
)

// FormatAnyLogMessage formats the given log message into a human-readable string.
func FormatAnyLogMessage(message proto.Message) string {
	switch data := message.(type) {
	case *logs_pb.Generic:
		return fmt.Sprintf("%v", data.GetMessage())
	case *logs_pb.Oom:
		return "Out of memory"
	case *logs_pb.RpcConnectionError:
		return fmt.Sprintf("Cannot initiate connection to %v at %v: %v", data.GetTargetService(), data.GetTargetUri(), data.GetDetails())
	case *logs_pb.InvalidProjectName:
		return fmt.Sprintf("Project name `%v` is invalid: %v", data.GetName(), data.GetDetails())
	case *logs_pb.ProjectNotFound:
		return fmt.Sprintf("Project `%v` not found", data.GetName())
	case *logs_pb.ProjectAlreadyExists:
		return fmt.Sprintf("Project `%v` already exists", data.GetName())
	case *logs_pb.InvalidConfiguration:
		return "Configuration is invalid"
	case *logs_pb.EntityNotFound:
		return fmt.Sprintf("%v `%v` not found", data.GetEntityType(), data.GetId())
	case *logs_pb.ActiveProjectCannotBeDeleted:
		return "Active project cannot be deleted"
	case *logs_pb.RpcDisconnectionError:
		return fmt.Sprintf("Cannot terminate connection to %v at %v: %v", data.GetTargetService(), data.GetTargetUri(), data.GetDetails())
	case *logs_pb.DroppedLogs:
		return fmt.Sprintf("Dropped %v log messages", data.GetN())
	case *logs_pb.OpenedProject:
		return fmt.Sprintf("Opened project %v", data.GetProjectName())
	case *logs_pb.CreatedProject:
		return fmt.Sprintf("Created project %v from %v", data.GetProjectName(), data.GetSourceName())
	case *logs_pb.ConfigUnavailable:
		return "Configuration is unavailable; request cannot be processed."
	case *logs_pb.InvalidRequest:
		return fmt.Sprintf("Invalid request received: %v", data.GetDetails())
	case *logs_pb.SanityCheckError:
		return fmt.Sprintf("%v failed: %v", data.GetSanityCheckName(), data.GetDetails())
	case *logs_pb.BagFailedToOpen:
		return fmt.Sprintf("The bag file (%v) could not be opened: %v", data.GetHorusBagPath(), data.GetDetails())
	case *logs_pb.BagFailedToClose:
		return fmt.Sprintf("Bag file could not be closed: %v", data.GetDetails())
	case *logs_pb.BagConversionFailed:
		return fmt.Sprintf("Bag file conversion failed: %v", data.GetDetails())
	case *logs_pb.BagFailedToWrite:
		return fmt.Sprintf("Bag file writing failed %v: %v", data.GetName(), data.GetDetails())
	case *logs_pb.CalibrationError:
		return fmt.Sprintf("Calibration failed: %v", data.GetDetails())
	case *logs_pb.ProjectManagerFailedToStartRecording:
		return fmt.Sprintf("Failed to start recording: %v", data.GetDetails())
	case *logs_pb.ProjectManagerFailedToStopRecording:
		return fmt.Sprintf("Failed to stop recording: %v", data.GetDetails())
	case *logs_pb.ServiceConnectionTimedOut:
		return fmt.Sprintf("The connection with the requested %v node with ID %v timed out", data.GetNodeType(), data.GetNodeId())
	case *logs_pb.BagRecorderAlreadyRunning:
		return fmt.Sprintf("Bag recorder is already running for %v", data.GetPath())
	case *logs_pb.LicenseServerConnectionError:
		return fmt.Sprintf("Could not connect to license server: %v.", data.GetDetails())
	case *logs_pb.LicenseError:
		return fmt.Sprintf("Could not read license: %v.", data.GetDetails())
	case *logs_pb.LicenseNotFoundError:
		return "Found no valid license."
	case *logs_pb.LicenseExpiredError:
		return fmt.Sprintf("License expired on %v.", data.GetExpirationTime())
	case *logs_pb.LicenseExceededError:
		return fmt.Sprintf("Project exceeds number of LiDARs allowed by license (%v > %v).", data.GetLidarCount(), data.GetMaxLidarCount())
	case *logs_pb.LicenseHostMachineError:
		return fmt.Sprintf("Could not validate license with current hardware - %v.", data.GetDetails())
	case *logs_pb.LicensePrivilegeError:
		return fmt.Sprintf("Endpoint requires \"%v\" privilege: %v.", data.GetMissingPrivilege(), data.GetLevelError())
	case *logs_pb.LicenseActiveInfo:
		return "License is active."
	case *logs_pb.MultipleLicensesWarning:
		return fmt.Sprintf("Found %v non-expired concurrent licenses. Only \"%v\" is used.", data.GetNonExpiredLicensesCount(), data.GetActiveLicenseFilename())
	case *logs_pb.LicenseCurrentLicenseStatusInfo:
		return fmt.Sprintf("Current license accepts up to %v and expires on %v.", data.GetMaxLidarCount(), data.GetExpirationTime())
	case *logs_pb.BagRecordingStoppedInfo:
		return fmt.Sprintf("Bag recording stopped for %v: %v", data.GetPath(), data.GetDetails())
	case *logs_pb.BagRecordingFailedToStart:
		return fmt.Sprintf("Bag recording failed to start: %v", data.GetDetails())
	case *logs_pb.BagRecordingStartedInfo:
		return fmt.Sprintf("Bag recording started for %v", data.GetPath())
	case *logs_pb.ReplayRestartedInfo:
		return "Replay Restarted"
	case *logs_pb.InputSourceChangeRequestedInfo:
		return fmt.Sprintf("Input source %v requested", data.GetSource())
	case *logs_pb.InputSourceSwitchedInfo:
		return fmt.Sprintf("Input source changed to %v", data.GetSource())
	case *logs_pb.RpcTimeoutWarning:
		return fmt.Sprintf("RPC request %v to %v timed out after %v.", data.GetRequestName(), data.GetEndpoint(), data.GetElapsed())
	case *logs_pb.CannotWriteLogFile:
		return fmt.Sprintf("Notification service cannot write log in %v: %v", data.GetPath(), data.GetDetails())
	case *logs_pb.PointCloudParsingFailureWarning:
		return fmt.Sprintf("Point cloud parsing failed: %v", data.GetDetails())
	case *logs_pb.LidarIsDead:
		return fmt.Sprintf("The lidar %v is considered dead. No data has been received for a while.", data.GetLidarId())
	case *logs_pb.LidarIsNotDeadAnymore:
		return fmt.Sprintf("The lidar %v is not considered dead anymore. A point cloud has been received again after some time.", data.GetLidarId())
	case *logs_pb.LidarIsObstructed:
		return fmt.Sprintf("The lidar %v is obstructed.", data.GetLidarId())
	case *logs_pb.LidarIsNotObstructedAnymore:
		return fmt.Sprintf("The lidar %v is not obstructed anymore.", data.GetLidarId())
	case *logs_pb.LidarIsTilted:
		return fmt.Sprintf("The lidar %v is tilted.", data.GetLidarId())
	case *logs_pb.LidarIsNotTiltedAnymore:
		return fmt.Sprintf("The lidar %v is not tilted anymore.", data.GetLidarId())
	case *logs_pb.LidarHasBeenAutomaticallyRecalibrated:
		return fmt.Sprintf("The lidar %v has been automatically re-calibrated.", data.GetLidarId())
	case *logs_pb.ReceivedFirstDataForLidar:
		return fmt.Sprintf("Data has been received for the first time for the lidar %v.", data.GetLidarId())
	case *logs_pb.TerminationFailureError:
		return fmt.Sprintf("Failed to destruct %v: %v", data.GetComponent(), data.GetDetails())
	case *logs_pb.FrameProcessingError:
		return fmt.Sprintf("The Frame Task Scheduler failed to process frame task: %v", data.GetDetails())
	case *logs_pb.ThreadPoolUnavailableError:
		return fmt.Sprintf("Thread pool is not available: %v", data.GetConsequence())
	case *logs_pb.InvalidArgument:
		return fmt.Sprintf("Invalid argument: %v", data.GetDetails())
	case *logs_pb.ComponentInitializationFailureFatal:
		return fmt.Sprintf("Failed to initialize %v: %v", data.GetComponent(), data.GetDetails())
	case *logs_pb.UnhandledEnumCaseError:
		return fmt.Sprintf("Unhandled enum case %v at %v", data.GetCaseName(), data.GetLocation())
	case *logs_pb.BagEmptyError:
		return "The replay bag file is empty."
	case *logs_pb.DiscardingDataError:
		return fmt.Sprintf("Discarding data in %v: %v", data.GetComponent(), data.GetDetails())
	case *logs_pb.DiscardingDataWarning:
		return fmt.Sprintf("Discarding data in %v: %v.", data.GetComponent(), data.GetDetails())
	case *logs_pb.NothingToProcess:
		return fmt.Sprintf("Nothing to process: %v", data.GetDetails())
	case *logs_pb.InvalidComponentConfiguration:
		return fmt.Sprintf("Invalid configuration for %v: %v", data.GetComponent(), data.GetDetails())
	case *logs_pb.ChannelReceiverNotFoundWarning:
		return fmt.Sprintf("Channel receiver %v not found", data.GetKey())
	case *logs_pb.ModelLoadFailure:
		return fmt.Sprintf("Model %v failed to load: %v", data.GetModel(), data.GetDetails())
	case *logs_pb.ModelExecutionFailureError:
		return fmt.Sprintf("Model %v failed to execute: %v", data.GetModel(), data.GetDetails())
	case *logs_pb.ServiceReadyInfo:
		return fmt.Sprintf("%v service is ready", data.GetService())
	case *logs_pb.ModelPreparingInfo:
		return fmt.Sprintf("Preparing model %v, it may take several seconds to stabilize.", data.GetModel())
	case *logs_pb.ModelInitializedInfo:
		return fmt.Sprintf("%v model has been initialized and is ready for use.", data.GetName())
	case *logs_pb.ModelInitializationFailureWarning:
		return fmt.Sprintf("Failed to initialize model %v: %v", data.GetModel(), data.GetDetails())
	case *logs_pb.RosSpinnerStoppedWarning:
		return fmt.Sprintf("ROS spinner stopped: %v", data.GetDetails())
	case *logs_pb.ActorSystemUnavailableError:
		return "Actor system is not available. Please reinstantiate the pipeline."
	case *logs_pb.ConfigNodeNotFoundError:
		return fmt.Sprintf("Configuration for node %v is unavailable", data.GetNode())
	case *logs_pb.BagTimestampOutOfOrderError:
		return fmt.Sprintf("Timestamps are out of order: %v > %v", data.GetPriorTimestamp(), data.GetCurrentTimestamp())
	case *logs_pb.BagReplayUnexpectedTimestampError:
		return fmt.Sprintf("Unexpected timestamp: expected %v but received %v", data.GetExpectedTimestamp(), data.GetReceivedTimestamp())
	case *logs_pb.WebsocketClosedInfo:
		return fmt.Sprintf("WebSocket RPC connection to %v closed", data.GetUri())
	case *logs_pb.WebsocketOpenedInfo:
		return fmt.Sprintf("WebSocket RPC connection to %v opened (as %v)", data.GetUri(), data.GetEndpointIsServerClient())
	case *logs_pb.SubscriberDisconnectedInfo:
		return fmt.Sprintf("Subscriber %v disconnected (%v)", data.GetName(), data.GetUri())
	case *logs_pb.ThreadPoolSlowingDownWarning:
		return fmt.Sprintf("Thread pool is slowing down for %v times!", data.GetNumber())
	case *logs_pb.ThreadPoolNotRespondingWarning:
		return fmt.Sprintf("Thread pool is not responding for %v.", data.GetNotRespondingFor())
	case *logs_pb.ThreadPoolBrokenPromiseWarning:
		return "Thread pool task future is no longer available. The thread pool might have been restarted during events such as input source change."
	case *logs_pb.BoxFittingPointsBelowBaseWarning:
		return "Some points are below the base Z-coordinate when fitting the box. This might affect the detection results."
	case *logs_pb.FailedToRemoveStalePointsWarning:
		return fmt.Sprintf("Failed to remove stale points: %v", data.GetDetails())
	case *logs_pb.IrregularBroadcastingPeriodWarning:
		return fmt.Sprintf("%v irregular broadcast(s) detected in the last %v (mean deviation: %v).", data.GetNumIrregular(), data.GetDuration(), data.GetMeanDeviation())
	case *logs_pb.ClustererPointsOutOfRangeWarning:
		return fmt.Sprintf("There are %v points out of range in the clusterer. Some points: %v.", data.GetNumPoints(), data.GetSamplePoints())
	case *logs_pb.InternalError:
		return fmt.Sprintf("An internal error occurred: %v", data.GetDetails())
	case *logs_pb.InternalFatal:
		return fmt.Sprintf("An internal fatal error occurred: %v", data.GetDetails())
	case *logs_pb.ServiceStartingInfo:
		return fmt.Sprintf("%v is starting", data.GetService())
	case *logs_pb.ConfigNodeNotFoundFatal:
		return fmt.Sprintf("Configuration for node %v is unavailable", data.GetNode())
	case *logs_pb.ServiceSetupError:
		return fmt.Sprintf("Failed to setup: %v", data.GetDetails())
	case *logs_pb.FilesystemError:
		return fmt.Sprintf("Filesystem error: %v", data.GetDetails())
	case *logs_pb.InvalidPresetWarning:
		return fmt.Sprintf("Tried to apply invalid preset %v due to %v, falling back to %v.", data.GetPreset(), data.GetReason(), data.GetFallbackName())
	case *logs_pb.WebsocketFailedClearPendingError:
		return fmt.Sprintf("WebSocket failed to clear pending queues: %v", data.GetException())
	case *logs_pb.WebsocketFailedToStopError:
		return fmt.Sprintf("WebSocket failed to stop: %v", data.GetException())
	case *logs_pb.WebsocketFailedToCreateError:
		return fmt.Sprintf("Failed to create RPC server ws://%v:%v/: %v", data.GetHost(), data.GetPort(), data.GetException())
	case *logs_pb.UnexpectedRpcError:
		return fmt.Sprintf("Unexpected RPC error: %v", data.GetDetails())
	case *logs_pb.LicensePollFailed:
		return fmt.Sprintf("Failed to poll license server: %v", data.GetDetails())
	case *logs_pb.LicenseExpiredWarning:
		return fmt.Sprintf("License expired on %v.", data.GetExpirationTime())
	case *logs_pb.LicenseUsageExceededWarning:
		return fmt.Sprintf("License count of %v exceeds allowed usage of %v", data.GetLidarCount(), data.GetMaxLidarCount())
	case *logs_pb.StaticThreadPoolSlowTaskWarning:
		return fmt.Sprintf("ThreadPool task took %v to complete task enqueued in %v", data.GetTimeElapsed(), data.GetEnqueueLocation())
	case *logs_pb.RpcUnsupportedServiceWarning:
		return fmt.Sprintf("Received request for unsupported service: %v", data.GetServiceId())
	case *logs_pb.WebsocketHandlerProblem:
		return fmt.Sprintf("WebSocket RPC handler encountered an error: %v", data.GetWhat())
	case *logs_pb.WebsocketDeserializeError:
		return fmt.Sprintf("WebSocket RPC received message cannot be deserialized: %v", data.GetWhat())
	case *logs_pb.WebsocketExpiredRpcEndpointError:
		return fmt.Sprintf("Attempting to access expired WebSocket RPC endpoint %v", data.GetUri())
	case *logs_pb.WebsocketQueueOverloadedWarning:
		return fmt.Sprintf("Websocket message queue overloaded: %v / %v", data.GetCurrent(), data.GetMax())
	case *logs_pb.RpcFailedToNotifyWarning:
		return fmt.Sprintf("Could not notify %v to %v %v: %v", data.GetNotificationName(), data.GetService(), data.GetUri(), data.GetWhat())
	case *logs_pb.ConfigSubscriptionFailedWarning:
		return fmt.Sprintf("Failed to subscribe to config: %v", data.GetDetails())
	case *logs_pb.ThreadPoolClampedWorkersWarning:
		return fmt.Sprintf("Clamped thread pool workers to %v instead of %v", data.GetClamped(), data.GetRequested())
	case *logs_pb.StoppingHorusBagRecorderAlreadyStopped:
		return "Trying to stop the Horus bag recorder which is already stopped."
	case *logs_pb.RecorderConfigUpdateWhileRunning:
		return "An update for the Horus bag recorder has been received while it was running. This update will not be taken into account until the next recording."
	case *logs_pb.ClampingDataWarning:
		return fmt.Sprintf("Clamping point cloud: %v", data.GetDetails())
	case *logs_pb.LidarIncompatibleValues:
		return fmt.Sprintf("Lidars %v and %v have incompatible %v: %v vs. %v; %v.", data.GetLidarIp1(), data.GetLidarIp2(), data.GetValueNames(), data.GetValue1(), data.GetValue2(), data.GetResolution())
	case *logs_pb.CannotDetermineContainerIdError:
		return fmt.Sprintf("Cannot determine current Docker container ID; unknown %v format.", data.GetContainerIdFilePath())
	case *logs_pb.StartedLidarDriver:
		return fmt.Sprintf("Started lidar driver container %v.", data.GetLidarId())
	case *logs_pb.CannotStartLidarDriver:
		return fmt.Sprintf("Cannot start lidar driver container %v: %v.", data.GetLidarId(), data.GetDetails())
	case *logs_pb.StoppedLidarDriver:
		return fmt.Sprintf("Stopped lidar driver container %v.", data.GetLidarId())
	case *logs_pb.CannotStopLidarDriver:
		return fmt.Sprintf("Cannot stop lidar driver container %v: %v.", data.GetLidarId(), data.GetDetails())
	case *logs_pb.RestartedLidarDriver:
		return fmt.Sprintf("Restarted lidar driver container %v.", data.GetLidarId())
	case *logs_pb.CannotRestartLidarDriver:
		return fmt.Sprintf("Cannot restart lidar driver container %v: %v.", data.GetLidarId(), data.GetDetails())
	case *logs_pb.RemovedUnusedLidarDriver:
		return fmt.Sprintf("Removed unused lidar driver container %v.", data.GetLidarId())
	case *logs_pb.CannotRemoveUnusedLidarDriver:
		return fmt.Sprintf("Cannot remove unused lidar driver container %v: %v.", data.GetLidarId(), data.GetDetails())
	case *logs_pb.LidarDriverGcFailure:
		return fmt.Sprintf("Error encountered while removing unused lidar driver containers: %v.", data.GetDetails())
	case *logs_pb.IdSpaceExhausted:
		return fmt.Sprintf("Cannot allocate new identifier with prefix \"%v\".", data.GetPrefix())
	case *logs_pb.PreprocessingToPointAggregatorPointsSkipped:
		return fmt.Sprintf("The point cloud publishing to the point aggregator service has been skipped %v time(s) in the last %v.", data.GetNumSkippedPoints(), data.GetCheckInterval())
	case *logs_pb.MinMsgIntervalLessThanThreshold:
		return fmt.Sprintf("Discarding lidar points from %v since the time interval between two point-cloud messages is too close (<%v). Adjust the Min-Message Interval parameter to change this behavior.", data.GetLidarId(), data.GetThreshold())
	case *logs_pb.FailedToCleanupRosWarning:
		return fmt.Sprintf("Failed to clean up ROS nodes and processes: %v", data.GetDetails())
	case *logs_pb.RpcDisconnectedWarning:
		return fmt.Sprintf("RPC request %v to %v failed since endpoint is disconnected.", data.GetRequestName(), data.GetEndpoint())
	case *logs_pb.RpcUnhandledError:
		return fmt.Sprintf("RPC request handler for %v failed due to an unhandled internal error: %v.", data.GetRequestName(), data.GetDetails())
	case *logs_pb.TimeDiffOutOfRangeWarning:
		return fmt.Sprintf("Time difference is out of range: %v. Previous timestamp: %v, Current timestamp: %v.", data.GetDiff(), data.GetPrevTime(), data.GetCurrTime())
	case *logs_pb.TensorrtLog:
		return fmt.Sprintf("[TensorRT] %v", data.GetLog())
	case *logs_pb.BuildingTensorrtEngineInfo:
		return "Building TensorRT engine since the model has changed or the engine is not available. This may take a while."
	case *logs_pb.LoadingTensorrtEngineInfo:
		return "Loading TensorRT engine. This may take a while."
	case *logs_pb.CalibrationMapNotFound:
		return fmt.Sprintf("The calibration map %v was not found.", data.GetPath())
	case *logs_pb.CalibrationMapNotValid:
		return fmt.Sprintf("The calibration map %v is not valid.", data.GetPath())
	case *logs_pb.CalibrationMapPathAlreadyExists:
		return fmt.Sprintf("The calibration map path %v already exists.", data.GetPath())
	case *logs_pb.FailedToSaveCalibrationMap:
		return fmt.Sprintf("Failed to save the calibration map to %v.", data.GetPath())
	case *logs_pb.FailedToRemoveCalibrationMap:
		return fmt.Sprintf("Failed to remove the calibration map at %v.", data.GetPath())
	case *logs_pb.FailedToIterateInDirectory:
		return fmt.Sprintf("Failed to iterate in the %v directory %v: %v.", data.GetDirectoryName(), data.GetDirectoryPath(), data.GetDetails())
	case *logs_pb.MapBasedCalibrationWithoutMapLoading:
		return "The calibration map must be running before triggering the map-based calibration."
	case *logs_pb.MapBasedCalibrationAlreadyRunning:
		return "The map-based calibration is already running."
	case *logs_pb.CancelMapBasedCalibrationNotRunning:
		return "A cancel map-based calibration request has been received while the calibration is not running."
	case *logs_pb.BagStreamNotFound:
		return fmt.Sprintf("The stream %v was not found from %v.", data.GetStreamId(), data.GetBagPath())
	case *logs_pb.EvaluationBagStartedInfo:
		return fmt.Sprintf("Evaluation bag started for %v", data.GetBagPath())
	case *logs_pb.EvaluationBagFinishedInfo:
		return fmt.Sprintf("Evaluation bag finished for %v", data.GetBagPath())
	case *logs_pb.BagNotFound:
		return fmt.Sprintf("The bag %v was not found.", data.GetBagPath())
	case *logs_pb.BuildingPipelineInfo:
		return fmt.Sprintf("Building the %v pipeline.", data.GetPipelineName())
	case *logs_pb.BagIsNotEvaluation:
		return fmt.Sprintf("The bag %v is not an evaluation bag.", data.GetBagPath())
	case *logs_pb.HorusBagRunning:
		return "A Horus bag is currently running."
	case *logs_pb.AutoGroundCalibrationWarning:
		return fmt.Sprintf("Automatic ground calibration failed for the following lidars: %v.", data.GetFailedLidarIds())
	case *logs_pb.AutoGroundCalibrationError:
		return "Automatic ground calibration failed for all lidars."
	case *logs_pb.ObjectDetectorNotLoadedWarning:
		return "The object detector is not loaded yet. Temporarily using rule based detection which may result in degraded detection results."
	case *logs_pb.CalibrationIsRunningError:
		return "A calibration process is already running."
	case *logs_pb.ModelInitInProgressInfo:
		return fmt.Sprintf("%v will start processing data once all models have finished initializing.", data.GetComponent())
	case *logs_pb.RpcTimeoutWithResolutionWarning:
		return fmt.Sprintf("RPC request \"%v\" to \"%v\" timed out after %v. Resolution: %v", data.GetRequestName(), data.GetEndpoint(), data.GetElapsed(), data.GetResolution())
	case *logs_pb.CalibrationWasCancelledInfo:
		return fmt.Sprintf("The %v process was cancelled.", data.GetCalibrationProcessName())
	case *logs_pb.CalibrationMapRecordingFailedToStart:
		return fmt.Sprintf("Failed to start recording the calibration map: %v", data.GetPath())
	case *logs_pb.DetectionPipelineRequestedResetInfo:
		return fmt.Sprintf("The detection pipeline has been requested to be reset because %v.", data.GetReason())
	case *logs_pb.PreprocessingServicePipelineUnavailable:
		return fmt.Sprintf("Preprocessing service pipeline is not available yet due to %v.", data.GetReason())
	case *logs_pb.CircularRecordingDisabledWarning:
		return fmt.Sprintf("The timeframe snapshot is disabled: %v", data.GetReason())
	case *logs_pb.SnapshotAlreadyRunningWarning:
		return "A snapshot is already running."
	case *logs_pb.ActiveProjectChangedInfo:
		return "The active project has been changed."
	case *logs_pb.ProjectConfigUpdatedInfo:
		return "The project configuration has been updated."
	case *logs_pb.InvalidLidarTimestamp:
		return fmt.Sprintf("Invalid timestamp %v sent by lidar %v.", data.GetTimestamp(), data.GetLidarId())
	case *logs_pb.CalibrationAccumulatingPointsInfo:
		return fmt.Sprintf("Calibration is accumulating points for %v", data.GetTime())
	case *logs_pb.SparseNoiseFilterUsageNonRotationalLidars:
		return "The sparse noise filter cannot be used with non-rotational lidars."
	case *logs_pb.FileWriteError:
		return fmt.Sprintf("Failed to write to file \"%v\": \"%v\".", data.GetFilename(), data.GetDetails())
	case *logs_pb.LicenseForbiddenFeature:
		return fmt.Sprintf("%v is not allowed by the current license.", data.GetFeatureName())
	case *logs_pb.FailedToUpdateConfiguration:
		return fmt.Sprintf("Failed to update the configuration: %v.", data.GetDetails())
	case *logs_pb.ObstructionDetectorBadReferenceWarning:
		return "The obstruction detector reference is not valid since it contains zero points."
	case *logs_pb.ProjectFileInvalidPermissionsError:
		return fmt.Sprintf("Project file \"%v\" has invalid permissions. Please restart Horus to fix the issue.", data.GetFilename())
	case *logs_pb.PipelineSchedulerError:
		return fmt.Sprintf("Pipeline scheduler error: %v.", data.GetDetails())
	case *logs_pb.MultiLidarCalibrationWarning:
		return fmt.Sprintf("Multi lidar calibration failed for the following lidars: %v.", data.GetFailedLidarIds())
	case *logs_pb.MultiLidarCalibrationError:
		return "Multi lidar calibration failed for all lidars."
	case *logs_pb.OldBagWarning:
		return fmt.Sprintf("The bag file %v with version %v is old. Please consider updating it with the API, refer to API documentation.", data.GetBagPath(), data.GetVersion())
	case *logs_pb.UpgradingBagInfo:
		return fmt.Sprintf("Upgrading bag file %v to latest version.", data.GetBagPath())
	case *logs_pb.BagCalibrationSaveFailed:
		return fmt.Sprintf("Failed to save the bag calibration: %v.", data.GetDetails())
	case *logs_pb.BagUpgradeFailed:
		return fmt.Sprintf("Failed to upgrade the bag file %v: %v.", data.GetBagPath(), data.GetDetails())
	case *logs_pb.UnknownLidarError:
		return fmt.Sprintf("Received points from unknown lidar %v.", data.GetLidarId())
	case *logs_pb.InvalidPointCloudWarning:
		return fmt.Sprintf("Invalid point cloud received from lidar %v: %v", data.GetLidarId(), data.GetReason())
	default:
		return "unknown log message"
	}
}
