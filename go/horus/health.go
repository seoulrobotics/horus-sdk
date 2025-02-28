package horus

import (
	"errors"
	"time"

	"github.com/seoulrobotics/horus-sdk/go/internal"
	ls_pb "github.com/seoulrobotics/horus-sdk/go/proto/license_server/messages_pb"
	pp_pb "github.com/seoulrobotics/horus-sdk/go/proto/preprocessing/messages_pb"
	pm_pb "github.com/seoulrobotics/horus-sdk/go/proto/project_manager/service_pb"
)

// MARK: License status

// Current license information.
type LicenseInfo struct {
	// Expiration timestamp of the current license.
	Expiration time.Time
	// Number of lidars allowed by the current license.
	LidarCount uint32
	// List of features allowed by the license.
	AllowedFeatures []LicenseFeature
}

// Builds a LicenseInfo from protobuf counterpart.
func newLicenseInfoFromPb(pb *ls_pb.LicenseInfo) *LicenseInfo {
	features := make([]LicenseFeature, len(pb.GetAllowedFeatures()))
	for i, feature := range pb.GetAllowedFeatures() {
		features[i] = LicenseFeature(feature.GetFeature())
	}

	return &LicenseInfo{
		Expiration:      time.Unix(pb.GetExpirationDate().GetSeconds(), int64(pb.GetExpirationDate().GetNanos())),
		LidarCount:      pb.GetLidarCount(),
		AllowedFeatures: features,
	}
}

// Enum representing the features allowed by a license.
type LicenseFeature uint32

const (
	LicenseFeatureCan              LicenseFeature = LicenseFeature(ls_pb.LicenseFeature_LICENSE_FEATURE_CAN)
	LicenseFeatureVinAssociator    LicenseFeature = LicenseFeature(ls_pb.LicenseFeature_LICENSE_FEATURE_VIN_ASSOCIATOR)
	LicenseFeatureMultiRosbag      LicenseFeature = LicenseFeature(ls_pb.LicenseFeature_LICENSE_FEATURE_MULTI_ROSBAG)
	LicenseFeatureDebuggingSupport LicenseFeature = LicenseFeature(ls_pb.LicenseFeature_LICENSE_FEATURE_DEBUGGING_SUPPORT)
	LicenseFeatureAtlas            LicenseFeature = LicenseFeature(ls_pb.LicenseFeature_LICENSE_FEATURE_ATLAS)
	LicenseFeatureMacgyver         LicenseFeature = LicenseFeature(ls_pb.LicenseFeature_LICENSE_FEATURE_MACGYVER)
)

// Converts LicenseFeature enum to string.
func (feature LicenseFeature) String() string {
	switch feature {
	case LicenseFeatureCan:
		return "CAN bus"
	case LicenseFeatureVinAssociator:
		return "VIN associator"
	case LicenseFeatureMultiRosbag:
		return "Multiple rosbags"
	case LicenseFeatureDebuggingSupport:
		return "Debugging support"
	case LicenseFeatureAtlas:
		return "Atlas"
	case LicenseFeatureMacgyver:
		return "SR Analytics"
	default:
		return "Unknown"
	}
}

// License status gathering all license information.
type LicenseStatus struct {
	// Level number, from 1 to 4.
	LicenseLevel uint32
	// Reason why the license is at that level.
	Reason string
	// This map gathers all existing privileges as key. If a privilege is granted, then value is true.
	Privileges map[LicensePrivilege]bool
	// If a license has been found (i.e license level >= 3), contains the license info.
	LicenseInfo *LicenseInfo
}

// Builds a LicenseStatus from protobuf counterpart.
func newLicenseStatusFromPb(pb *pm_pb.LicenseStatus) (*LicenseStatus, error) {
	var licenseLevel uint32
	var reason string
	var privileges map[LicensePrivilege]bool
	var licenseInfo *LicenseInfo

	switch pb.GetLicenseLevel().WhichLevel() {
	case pm_pb.LicenseLevel_Level_1_case:
		licenseLevel = 1
		reason = internal.FormatAnyLogMessage(pb.GetLicenseLevel().GetLevel_1().GetLicenseServerUnreachable())
	case pm_pb.LicenseLevel_Level_2_case:
		licenseLevel = 2
		reason = internal.FormatAnyLogMessage(pb.GetLicenseLevel().GetLevel_2().GetLicenseNotFound())
	case pm_pb.LicenseLevel_Level_3_case:
		licenseLevel = 3
		switch pb.GetLicenseLevel().GetLevel_3().WhichReason() {
		case pm_pb.LicenseLevel_Level3_LicenseExpired_case:
			reason = internal.FormatAnyLogMessage(pb.GetLicenseLevel().GetLevel_3().GetLicenseExpired())
		case pm_pb.LicenseLevel_Level3_LicenseServerDisconnected_case:
			reason = internal.FormatAnyLogMessage(pb.GetLicenseLevel().GetLevel_3().GetLicenseServerDisconnected())
		default:
			return nil, errors.New("unexpected license level3 reason: " + pb.GetLicenseLevel().GetLevel_3().WhichReason().String())
		}
	case pm_pb.LicenseLevel_Level_4_case:
		licenseLevel = 4
		reason = internal.FormatAnyLogMessage(pb.GetLicenseLevel().GetLevel_4().GetLicenseActive())
	default:
		return nil, errors.New("unexpected license level: " + pb.GetLicenseLevel().WhichLevel().String())
	}

	privileges = make(map[LicensePrivilege]bool)
	privilegesToTest := []LicensePrivilege{LicensePrivilegeLowest, LicensePrivilegeLicenseActions, LicensePrivilegeSubscribe, LicensePrivilegeRead, LicensePrivilegeWrite}
	for _, privilege := range privilegesToTest {
		privileges[privilege] = pb.GetLicenseLevel().GetPrivilege()&uint32(privilege) == uint32(privilege)
	}

	if pb.HasLicenseInfo() {
		licenseInfo = newLicenseInfoFromPb(pb.GetLicenseInfo())
	}

	return &LicenseStatus{
		LicenseLevel: licenseLevel,
		Reason:       reason,
		Privileges:   privileges,
		LicenseInfo:  licenseInfo,
	}, nil
}

// Returns whether the license grants a specific privilege.
func (licenseStatus *LicenseStatus) HasPrivilege(privilege LicensePrivilege) bool {
	return licenseStatus.Privileges[privilege]
}

// Enum representing the privileges granted by a license level.
type LicensePrivilege uint32

const (
	LicensePrivilegeLowest         LicensePrivilege = LicensePrivilege(pm_pb.LicensePrivilege_LOWEST)
	LicensePrivilegeSubscribe      LicensePrivilege = LicensePrivilege(pm_pb.LicensePrivilege_SUBSCRIBE)
	LicensePrivilegeLicenseActions LicensePrivilege = LicensePrivilege(pm_pb.LicensePrivilege_LICENSE_ACTIONS)
	LicensePrivilegeRead           LicensePrivilege = LicensePrivilege(pm_pb.LicensePrivilege_READ)
	LicensePrivilegeWrite          LicensePrivilege = LicensePrivilege(pm_pb.LicensePrivilege_WRITE)
)

// Converts LicensePrivilege enum to string.
func (privilege LicensePrivilege) String() string {
	switch privilege {
	case LicensePrivilegeLowest:
		return "Lowest"
	case LicensePrivilegeSubscribe:
		return "Subscribe"
	case LicensePrivilegeLicenseActions:
		return "License actions"
	case LicensePrivilegeRead:
		return "Read"
	case LicensePrivilegeWrite:
		return "Write"
	default:
		return "Unknown"
	}
}

// MARK: Sensor info

// Information about a lidar.
type SensorHealth struct {
	// Unique identifier of the lidar.
	LidarId string
	// This map gathers all existing status as key. If the sensor has a specific status, then value is true.
	Statuses map[SensorStatus]bool
	// Measured frequency of the lidar.
	MeasuredFrequencyHz float64
	// Indicates why the sensor status could not been fetched. Empty if not unreachable.
	UnreachableReason string
}

// Enum representing the status applying to a lidar.
type SensorStatus uint32

const (
	SensorStatusNoData        SensorStatus = SensorStatus(pp_pb.SensorStatus_NO_DATA)
	SensorStatusReceivingData SensorStatus = SensorStatus(pp_pb.SensorStatus_RECEIVING_DATA)
	SensorStatusLowFrequency  SensorStatus = SensorStatus(pp_pb.SensorStatus_LOW_FREQUENCY)
	SensorStatusHighFrequency SensorStatus = SensorStatus(pp_pb.SensorStatus_HIGH_FREQUENCY)
	SensorStatusTilted        SensorStatus = SensorStatus(pp_pb.SensorStatus_TILTED)
	SensorStatusObstructed    SensorStatus = SensorStatus(pp_pb.SensorStatus_OBSTRUCTED)
)

// Converts SensorHealth flag enum to string.
func (status SensorStatus) String() string {
	switch status {
	case SensorStatusNoData:
		return "No data"
	case SensorStatusReceivingData:
		return "Receiving data"
	case SensorStatusLowFrequency:
		return "Low frequency"
	case SensorStatusHighFrequency:
		return "High frequency"
	case SensorStatusTilted:
		return "Tilted"
	case SensorStatusObstructed:
		return "Obstructed"
	default:
		return "Unknown"
	}
}

// Builds a LicenseStatus from protobuf counterpart.
func newSensorHealthFromPb(pb *pm_pb.GetHealthStatusResponse_SensorHealth) *SensorHealth {
	var statuses map[SensorStatus]bool
	var unreachableReason string

	// If status is 0 in the protobuf message, the sensor info is unreachable.
	if pb.GetInfo().GetStatus() == 0 {
		statuses = nil
		unreachableReason = internal.FormatAnyLogMessage(pb.GetTimeout())
	} else {
		statuses = make(map[SensorStatus]bool)
		statusesToTest := []SensorStatus{SensorStatusNoData, SensorStatusReceivingData, SensorStatusLowFrequency, SensorStatusHighFrequency, SensorStatusTilted, SensorStatusObstructed}
		for _, status := range statusesToTest {
			statuses[status] = (pb.GetInfo().GetStatus() & uint32(status)) == uint32(status)
		}
	}

	return &SensorHealth{
		LidarId:             pb.GetInfo().GetLidarId(),
		Statuses:            statuses,
		MeasuredFrequencyHz: pb.GetInfo().GetMeasuredFrequency(),
		UnreachableReason:   unreachableReason,
	}
}

// Returns whether a sensor has a specific privilege.
func (sensorStatus *SensorHealth) HasStatus(status SensorStatus) bool {
	return sensorStatus.Statuses[status]
}

// Returns whether a sensor is unreachable.
func (sensorStatus *SensorHealth) IsUnreachable() bool {
	return sensorStatus.UnreachableReason == ""
}

// MARK: Node status

// Connectivity status of a node.
type NodeHealth struct {
	// Service which node is representing.
	Service NodeHealthService
	// Unique identifier of the node.
	NodeId string
	// Connectivity status of the node.
	Status NodeHealthStatus
}

// Enum representing the service that a node is representing.
type NodeHealthService uint32

const (
	NodeHealthServiceServiceDetection       NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_DETECTION)
	NodeHealthServiceServiceLidarRunner     NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_LIDAR_RUNNER)
	NodeHealthServiceServiceNotification    NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_NOTIFICATION)
	NodeHealthServiceServicePointAggregator NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_POINT_AGGREGATOR)
	NodeHealthServiceServicePreprocessing   NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_PREPROCESSING)
	NodeHealthServiceServiceProjectManager  NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_PROJECT_MANAGER)
)

// Converts NodeHealthService enum to string.
func (service NodeHealthService) String() string {
	switch service {
	case NodeHealthServiceServiceDetection:
		return "Detection"
	case NodeHealthServiceServiceLidarRunner:
		return "Lidar runner"
	case NodeHealthServiceServiceNotification:
		return "Notification"
	case NodeHealthServiceServicePointAggregator:
		return "Point aggregator"
	case NodeHealthServiceServicePreprocessing:
		return "Preprocessing"
	default:
		return "Unknown"
	}
}

// Enum representing the connectivity status of a node.
type NodeHealthStatus uint32

const (
	NodeHealthUnreachable NodeHealthStatus = NodeHealthStatus(pm_pb.GetHealthStatusResponse_NodeHealth_STATUS_UNREACHABLE)
	NodeHealthAlive       NodeHealthStatus = NodeHealthStatus(pm_pb.GetHealthStatusResponse_NodeHealth_STATUS_ALIVE)
)

// Converts NodeHealthStatus enum to string.
func (status NodeHealthStatus) String() string {
	switch status {
	case NodeHealthUnreachable:
		return "Unreachable"
	case NodeHealthAlive:
		return "Alive"
	default:
		return "Unknown"
	}
}

// Builds a LicenseStatus from protobuf counterpart.
func newNodeHealthFromPb(pb *pm_pb.GetHealthStatusResponse_NodeHealth) NodeHealth {
	return NodeHealth{
		Service: NodeHealthService(pb.GetService()),
		NodeId:  pb.GetNodeId(),
		Status:  NodeHealthStatus(pb.GetNodeStatus()),
	}
}

// MARK: GetHealthStatus{Request,Response}

// Request to get the health status from the project manager.
type GetHealthStatusRequest struct {
}

// Builds a protobuf GetHealthStatusRequest from its sdk counterpart.
func (*GetHealthStatusRequest) toPb() *pm_pb.GetHealthStatusRequest {
	return &pm_pb.GetHealthStatusRequest{}
}

// Health status returned by the project manager.
type HealthStatus struct {
	// License status.
	LicenseStatus LicenseStatus
	// Status of lidars. Set to nil if license does not grant Read privilege
	SensorStatuses []SensorHealth
	// Status of services. Set to nil if license does not grant Read privilege
	ServiceStatuses []NodeHealth
}

// Builds a GetHealthStatusResponse from protobuf counterpart.
func newHealthStatusFromPb(pb *pm_pb.GetHealthStatusResponse) (*HealthStatus, error) {
	licenseStatus, err := newLicenseStatusFromPb(pb.GetLicenseStatus())
	if err != nil {
		return nil, err
	}
	var sensorStatuses []SensorHealth
	var serviceStatuses []NodeHealth

	if licenseStatus.HasPrivilege(LicensePrivilegeRead) {
		sensorStatuses = make([]SensorHealth, len(pb.GetSensorStatuses()))
		for i, sensorStatus := range pb.GetSensorStatuses() {
			sensorStatuses[i] = *newSensorHealthFromPb(sensorStatus)
		}

		serviceStatuses = make([]NodeHealth, len(pb.GetServiceStatuses()))
		for i, nodeStatus := range pb.GetServiceStatuses() {
			serviceStatuses[i] = newNodeHealthFromPb(nodeStatus)
		}
	}

	return &HealthStatus{
		LicenseStatus:   *licenseStatus,
		SensorStatuses:  sensorStatuses,
		ServiceStatuses: serviceStatuses,
	}, nil
}
