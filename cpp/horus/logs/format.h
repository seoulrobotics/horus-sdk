/// @file
///
/// Generated functions for emitting logs.

#ifndef HORUS_LOGS_FORMAT_H_
#define HORUS_LOGS_FORMAT_H_

#include "horus/pb/logs/message_pb.h"
#include "horus/pb/logs/metadata_pb.h"
#include "horus/strings/erased_sink.h"

#if HORUS_SDK_USE_PB_NAMESPACE_ALIAS
#include "horus/pb/alias.h"  // IWYU pragma: export
#endif

namespace horus {
namespace sdk {
namespace pb {

/// Formats a `duration` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LogMetadata_Duration& duration);

/// Formats a `source_location` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LogMetadata_SourceLocation& source_location);

/// Formats a `timestamp` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LogMetadata_Timestamp& timestamp);

/// Formats the message of a `LogData` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LogData& log_data);

namespace logs {

/// Formats the log `logs::Generic` to a `sink`.
void HorusStringify(const ErasedSink& sink, const Generic& data);

/// Formats the log `logs::Oom` to a `sink`.
void HorusStringify(const ErasedSink& sink, const Oom& /*data*/);

/// Formats the log `logs::RpcConnectionError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RpcConnectionError& data);

/// Formats the log `logs::InvalidProjectName` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InvalidProjectName& data);

/// Formats the log `logs::ProjectNotFound` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ProjectNotFound& data);

/// Formats the log `logs::ProjectAlreadyExists` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ProjectAlreadyExists& data);

/// Formats the log `logs::InvalidConfiguration` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InvalidConfiguration& /*data*/);

/// Formats the log `logs::EntityNotFound` to a `sink`.
void HorusStringify(const ErasedSink& sink, const EntityNotFound& data);

/// Formats the log `logs::ActiveProjectCannotBeDeleted` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ActiveProjectCannotBeDeleted& /*data*/);

/// Formats the log `logs::RpcDisconnectionError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RpcDisconnectionError& data);

/// Formats the log `logs::DroppedLogs` to a `sink`.
void HorusStringify(const ErasedSink& sink, const DroppedLogs& data);

/// Formats the log `logs::OpenedProject` to a `sink`.
void HorusStringify(const ErasedSink& sink, const OpenedProject& data);

/// Formats the log `logs::CreatedProject` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CreatedProject& data);

/// Formats the log `logs::ConfigUnavailable` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ConfigUnavailable& /*data*/);

/// Formats the log `logs::InvalidRequest` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InvalidRequest& data);

/// Formats the log `logs::SanityCheckError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const SanityCheckError& data);

/// Formats the log `logs::BagFailedToOpen` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagFailedToOpen& data);

/// Formats the log `logs::BagFailedToClose` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagFailedToClose& data);

/// Formats the log `logs::BagConversionFailed` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagConversionFailed& data);

/// Formats the log `logs::BagFailedToWrite` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagFailedToWrite& data);

/// Formats the log `logs::CalibrationError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CalibrationError& data);

/// Formats the log `logs::ProjectManagerFailedToStartRecording` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ProjectManagerFailedToStartRecording& data);

/// Formats the log `logs::ProjectManagerFailedToStopRecording` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ProjectManagerFailedToStopRecording& data);

/// Formats the log `logs::ServiceConnectionTimedOut` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ServiceConnectionTimedOut& data);

/// Formats the log `logs::BagRecorderAlreadyRunning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagRecorderAlreadyRunning& data);

/// Formats the log `logs::LicenseServerConnectionError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseServerConnectionError& data);

/// Formats the log `logs::LicenseError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseError& data);

/// Formats the log `logs::LicenseNotFoundError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseNotFoundError& /*data*/);

/// Formats the log `logs::LicenseExpiredError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseExpiredError& data);

/// Formats the log `logs::LicenseExceededError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseExceededError& data);

/// Formats the log `logs::LicenseHostMachineError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseHostMachineError& data);

/// Formats the log `logs::LicensePrivilegeError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicensePrivilegeError& data);

/// Formats the log `logs::LicenseActiveInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseActiveInfo& /*data*/);

/// Formats the log `logs::MultipleLicensesWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const MultipleLicensesWarning& data);

/// Formats the log `logs::LicenseCurrentLicenseStatusInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseCurrentLicenseStatusInfo& data);

/// Formats the log `logs::BagRecordingStoppedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagRecordingStoppedInfo& data);

/// Formats the log `logs::BagRecordingFailedToStart` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagRecordingFailedToStart& data);

/// Formats the log `logs::BagRecordingStartedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagRecordingStartedInfo& data);

/// Formats the log `logs::ReplayRestartedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ReplayRestartedInfo& /*data*/);

/// Formats the log `logs::InputSourceChangeRequestedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InputSourceChangeRequestedInfo& data);

/// Formats the log `logs::InputSourceSwitchedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InputSourceSwitchedInfo& data);

/// Formats the log `logs::RpcTimeoutWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RpcTimeoutWarning& data);

/// Formats the log `logs::CannotWriteLogFile` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CannotWriteLogFile& data);

/// Formats the log `logs::PointCloudParsingFailureWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const PointCloudParsingFailureWarning& data);

/// Formats the log `logs::LidarIsDead` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LidarIsDead& data);

/// Formats the log `logs::LidarIsNotDeadAnymore` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LidarIsNotDeadAnymore& data);

/// Formats the log `logs::LidarIsObstructed` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LidarIsObstructed& data);

/// Formats the log `logs::LidarIsNotObstructedAnymore` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LidarIsNotObstructedAnymore& data);

/// Formats the log `logs::LidarIsTilted` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LidarIsTilted& data);

/// Formats the log `logs::LidarIsNotTiltedAnymore` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LidarIsNotTiltedAnymore& data);

/// Formats the log `logs::LidarHasBeenAutomaticallyRecalibrated` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LidarHasBeenAutomaticallyRecalibrated& data);

/// Formats the log `logs::ReceivedFirstDataForLidar` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ReceivedFirstDataForLidar& data);

/// Formats the log `logs::TerminationFailureError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const TerminationFailureError& data);

/// Formats the log `logs::FrameProcessingError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const FrameProcessingError& data);

/// Formats the log `logs::ThreadPoolUnavailableError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ThreadPoolUnavailableError& data);

/// Formats the log `logs::InvalidArgument` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InvalidArgument& data);

/// Formats the log `logs::ComponentInitializationFailureFatal` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ComponentInitializationFailureFatal& data);

/// Formats the log `logs::UnhandledEnumCaseError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const UnhandledEnumCaseError& data);

/// Formats the log `logs::BagEmptyError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagEmptyError& /*data*/);

/// Formats the log `logs::DiscardingDataError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const DiscardingDataError& data);

/// Formats the log `logs::DiscardingDataWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const DiscardingDataWarning& data);

/// Formats the log `logs::NothingToProcess` to a `sink`.
void HorusStringify(const ErasedSink& sink, const NothingToProcess& data);

/// Formats the log `logs::InvalidComponentConfiguration` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InvalidComponentConfiguration& data);

/// Formats the log `logs::ChannelReceiverNotFoundWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ChannelReceiverNotFoundWarning& data);

/// Formats the log `logs::ModelLoadFailure` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ModelLoadFailure& data);

/// Formats the log `logs::ModelExecutionFailureError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ModelExecutionFailureError& data);

/// Formats the log `logs::ServiceReadyInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ServiceReadyInfo& data);

/// Formats the log `logs::ModelPreparingInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ModelPreparingInfo& data);

/// Formats the log `logs::ModelInitializedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ModelInitializedInfo& data);

/// Formats the log `logs::ModelInitializationFailureWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ModelInitializationFailureWarning& data);

/// Formats the log `logs::RosSpinnerStoppedWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RosSpinnerStoppedWarning& data);

/// Formats the log `logs::ActorSystemUnavailableError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ActorSystemUnavailableError& /*data*/);

/// Formats the log `logs::ConfigNodeNotFoundError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ConfigNodeNotFoundError& data);

/// Formats the log `logs::BagTimestampOutOfOrderError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagTimestampOutOfOrderError& data);

/// Formats the log `logs::BagReplayUnexpectedTimestampError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagReplayUnexpectedTimestampError& data);

/// Formats the log `logs::WebsocketClosedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const WebsocketClosedInfo& data);

/// Formats the log `logs::WebsocketOpenedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const WebsocketOpenedInfo& data);

/// Formats the log `logs::SubscriberDisconnectedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const SubscriberDisconnectedInfo& data);

/// Formats the log `logs::ThreadPoolSlowingDownWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ThreadPoolSlowingDownWarning& data);

/// Formats the log `logs::ThreadPoolNotRespondingWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ThreadPoolNotRespondingWarning& data);

/// Formats the log `logs::ThreadPoolBrokenPromiseWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ThreadPoolBrokenPromiseWarning& /*data*/);

/// Formats the log `logs::BoxFittingPointsBelowBaseWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BoxFittingPointsBelowBaseWarning& /*data*/);

/// Formats the log `logs::FailedToRemoveStalePointsWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const FailedToRemoveStalePointsWarning& data);

/// Formats the log `logs::IrregularBroadcastingPeriodWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const IrregularBroadcastingPeriodWarning& data);

/// Formats the log `logs::ClustererPointsOutOfRangeWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ClustererPointsOutOfRangeWarning& data);

/// Formats the log `logs::InternalError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InternalError& data);

/// Formats the log `logs::InternalFatal` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InternalFatal& data);

/// Formats the log `logs::ServiceStartingInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ServiceStartingInfo& data);

/// Formats the log `logs::ConfigNodeNotFoundFatal` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ConfigNodeNotFoundFatal& data);

/// Formats the log `logs::ServiceSetupError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ServiceSetupError& data);

/// Formats the log `logs::FilesystemError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const FilesystemError& data);

/// Formats the log `logs::InvalidPresetWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InvalidPresetWarning& data);

/// Formats the log `logs::WebsocketFailedClearPendingError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const WebsocketFailedClearPendingError& data);

/// Formats the log `logs::WebsocketFailedToStopError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const WebsocketFailedToStopError& data);

/// Formats the log `logs::WebsocketFailedToCreateError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const WebsocketFailedToCreateError& data);

/// Formats the log `logs::UnexpectedRpcError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const UnexpectedRpcError& data);

/// Formats the log `logs::LicensePollFailed` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicensePollFailed& data);

/// Formats the log `logs::LicenseExpiredWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseExpiredWarning& data);

/// Formats the log `logs::LicenseUsageExceededWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseUsageExceededWarning& data);

/// Formats the log `logs::StaticThreadPoolSlowTaskWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const StaticThreadPoolSlowTaskWarning& data);

/// Formats the log `logs::RpcUnsupportedServiceWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RpcUnsupportedServiceWarning& data);

/// Formats the log `logs::WebsocketHandlerProblem` to a `sink`.
void HorusStringify(const ErasedSink& sink, const WebsocketHandlerProblem& data);

/// Formats the log `logs::WebsocketDeserializeError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const WebsocketDeserializeError& data);

/// Formats the log `logs::WebsocketExpiredRpcEndpointError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const WebsocketExpiredRpcEndpointError& data);

/// Formats the log `logs::WebsocketQueueOverloadedWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const WebsocketQueueOverloadedWarning& data);

/// Formats the log `logs::RpcFailedToNotifyWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RpcFailedToNotifyWarning& data);

/// Formats the log `logs::ConfigSubscriptionFailedWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ConfigSubscriptionFailedWarning& data);

/// Formats the log `logs::ThreadPoolClampedWorkersWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ThreadPoolClampedWorkersWarning& data);

/// Formats the log `logs::StoppingHorusBagRecorderAlreadyStopped` to a `sink`.
void HorusStringify(const ErasedSink& sink, const StoppingHorusBagRecorderAlreadyStopped& /*data*/);

/// Formats the log `logs::RecorderConfigUpdateWhileRunning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RecorderConfigUpdateWhileRunning& /*data*/);

/// Formats the log `logs::ClampingDataWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ClampingDataWarning& data);

/// Formats the log `logs::LidarIncompatibleValues` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LidarIncompatibleValues& data);

/// Formats the log `logs::CannotDetermineContainerIdError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CannotDetermineContainerIdError& data);

/// Formats the log `logs::StartedLidarDriver` to a `sink`.
void HorusStringify(const ErasedSink& sink, const StartedLidarDriver& data);

/// Formats the log `logs::CannotStartLidarDriver` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CannotStartLidarDriver& data);

/// Formats the log `logs::StoppedLidarDriver` to a `sink`.
void HorusStringify(const ErasedSink& sink, const StoppedLidarDriver& data);

/// Formats the log `logs::CannotStopLidarDriver` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CannotStopLidarDriver& data);

/// Formats the log `logs::RestartedLidarDriver` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RestartedLidarDriver& data);

/// Formats the log `logs::CannotRestartLidarDriver` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CannotRestartLidarDriver& data);

/// Formats the log `logs::RemovedUnusedLidarDriver` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RemovedUnusedLidarDriver& data);

/// Formats the log `logs::CannotRemoveUnusedLidarDriver` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CannotRemoveUnusedLidarDriver& data);

/// Formats the log `logs::LidarDriverGcFailure` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LidarDriverGcFailure& data);

/// Formats the log `logs::IdSpaceExhausted` to a `sink`.
void HorusStringify(const ErasedSink& sink, const IdSpaceExhausted& data);

/// Formats the log `logs::PreprocessingToPointAggregatorPointsSkipped` to a `sink`.
void HorusStringify(const ErasedSink& sink, const PreprocessingToPointAggregatorPointsSkipped& data);

/// Formats the log `logs::MinMsgIntervalLessThanThreshold` to a `sink`.
void HorusStringify(const ErasedSink& sink, const MinMsgIntervalLessThanThreshold& data);

/// Formats the log `logs::FailedToCleanupRosWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const FailedToCleanupRosWarning& data);

/// Formats the log `logs::RpcDisconnectedWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RpcDisconnectedWarning& data);

/// Formats the log `logs::RpcUnhandledError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RpcUnhandledError& data);

/// Formats the log `logs::TimeDiffOutOfRangeWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const TimeDiffOutOfRangeWarning& data);

/// Formats the log `logs::TensorrtLog` to a `sink`.
void HorusStringify(const ErasedSink& sink, const TensorrtLog& data);

/// Formats the log `logs::BuildingTensorrtEngineInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BuildingTensorrtEngineInfo& /*data*/);

/// Formats the log `logs::LoadingTensorrtEngineInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LoadingTensorrtEngineInfo& /*data*/);

/// Formats the log `logs::CalibrationMapNotFound` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CalibrationMapNotFound& data);

/// Formats the log `logs::CalibrationMapNotValid` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CalibrationMapNotValid& data);

/// Formats the log `logs::CalibrationMapPathAlreadyExists` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CalibrationMapPathAlreadyExists& data);

/// Formats the log `logs::FailedToSaveCalibrationMap` to a `sink`.
void HorusStringify(const ErasedSink& sink, const FailedToSaveCalibrationMap& data);

/// Formats the log `logs::FailedToRemoveCalibrationMap` to a `sink`.
void HorusStringify(const ErasedSink& sink, const FailedToRemoveCalibrationMap& data);

/// Formats the log `logs::FailedToIterateInDirectory` to a `sink`.
void HorusStringify(const ErasedSink& sink, const FailedToIterateInDirectory& data);

/// Formats the log `logs::MapBasedCalibrationWithoutMapLoading` to a `sink`.
void HorusStringify(const ErasedSink& sink, const MapBasedCalibrationWithoutMapLoading& /*data*/);

/// Formats the log `logs::MapBasedCalibrationAlreadyRunning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const MapBasedCalibrationAlreadyRunning& /*data*/);

/// Formats the log `logs::CancelMapBasedCalibrationNotRunning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CancelMapBasedCalibrationNotRunning& /*data*/);

/// Formats the log `logs::BagStreamNotFound` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagStreamNotFound& data);

/// Formats the log `logs::EvaluationBagStartedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const EvaluationBagStartedInfo& data);

/// Formats the log `logs::EvaluationBagFinishedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const EvaluationBagFinishedInfo& data);

/// Formats the log `logs::BagNotFound` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagNotFound& data);

/// Formats the log `logs::BuildingPipelineInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BuildingPipelineInfo& data);

/// Formats the log `logs::BagIsNotEvaluation` to a `sink`.
void HorusStringify(const ErasedSink& sink, const BagIsNotEvaluation& data);

/// Formats the log `logs::HorusBagRunning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const HorusBagRunning& /*data*/);

/// Formats the log `logs::AutoGroundCalibrationWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const AutoGroundCalibrationWarning& data);

/// Formats the log `logs::AutoGroundCalibrationError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const AutoGroundCalibrationError& /*data*/);

/// Formats the log `logs::ObjectDetectorNotLoadedWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ObjectDetectorNotLoadedWarning& /*data*/);

/// Formats the log `logs::CalibrationIsRunningError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CalibrationIsRunningError& /*data*/);

/// Formats the log `logs::ModelInitInProgressInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ModelInitInProgressInfo& data);

/// Formats the log `logs::RpcTimeoutWithResolutionWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const RpcTimeoutWithResolutionWarning& data);

/// Formats the log `logs::CalibrationWasCancelledInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CalibrationWasCancelledInfo& data);

/// Formats the log `logs::CalibrationMapRecordingFailedToStart` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CalibrationMapRecordingFailedToStart& data);

/// Formats the log `logs::DetectionPipelineRequestedResetInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const DetectionPipelineRequestedResetInfo& data);

/// Formats the log `logs::PreprocessingServicePipelineUnavailable` to a `sink`.
void HorusStringify(const ErasedSink& sink, const PreprocessingServicePipelineUnavailable& data);

/// Formats the log `logs::CircularRecordingDisabledWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CircularRecordingDisabledWarning& data);

/// Formats the log `logs::SnapshotAlreadyRunningWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const SnapshotAlreadyRunningWarning& /*data*/);

/// Formats the log `logs::ActiveProjectChangedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ActiveProjectChangedInfo& /*data*/);

/// Formats the log `logs::ProjectConfigUpdatedInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ProjectConfigUpdatedInfo& /*data*/);

/// Formats the log `logs::InvalidLidarTimestamp` to a `sink`.
void HorusStringify(const ErasedSink& sink, const InvalidLidarTimestamp& data);

/// Formats the log `logs::CalibrationAccumulatingPointsInfo` to a `sink`.
void HorusStringify(const ErasedSink& sink, const CalibrationAccumulatingPointsInfo& data);

/// Formats the log `logs::SparseNoiseFilterUsageNonRotationalLidars` to a `sink`.
void HorusStringify(const ErasedSink& sink, const SparseNoiseFilterUsageNonRotationalLidars& /*data*/);

/// Formats the log `logs::FileWriteError` to a `sink`.
void HorusStringify(const ErasedSink& sink, const FileWriteError& data);

/// Formats the log `logs::LicenseForbiddenFeature` to a `sink`.
void HorusStringify(const ErasedSink& sink, const LicenseForbiddenFeature& data);

/// Formats the log `logs::FailedToUpdateConfiguration` to a `sink`.
void HorusStringify(const ErasedSink& sink, const FailedToUpdateConfiguration& data);

/// Formats the log `logs::ObstructionDetectorBadReferenceWarning` to a `sink`.
void HorusStringify(const ErasedSink& sink, const ObstructionDetectorBadReferenceWarning& /*data*/);

}  // namespace logs

}  // namespace pb
}  // namespace sdk
}  // namespace horus

#endif  // HORUS_LOGS_FORMAT_H_
