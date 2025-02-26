package horus

import (
	"errors"
	"time"

	"github.com/seoulrobotics/horus-sdk/go/internal"
	pp_pb "github.com/seoulrobotics/horus-sdk/go/proto/preprocessing/messages_pb"
	pm_pb "github.com/seoulrobotics/horus-sdk/go/proto/project_manager/service_pb"
)

// MARK: License status

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
	privilegesToTest := []LicensePrivilege{LicensePrivilege_LOWEST, LicensePrivilege_LICENSE_ACTIONS, LicensePrivilege_SUBSCRIBE, LicensePrivilege_READ, LicensePrivilege_WRITE}
	for _, privilege := range privilegesToTest {
		privileges[privilege] = pb.GetLicenseLevel().GetPrivilege()&uint32(privilege) == uint32(privilege)
	}

	if pb.HasLicenseInfo() {
		licenseInfo = &LicenseInfo{
			LidarCount: pb.GetLicenseInfo().GetLidarCount(),
			Expiration: time.Unix(pb.GetLicenseInfo().GetExpirationDate().GetSeconds(), int64(pb.GetLicenseInfo().GetExpirationDate().GetNanos())),
		}
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
	LicensePrivilege_LOWEST          LicensePrivilege = LicensePrivilege(pm_pb.LicensePrivilege_LOWEST)
	LicensePrivilege_SUBSCRIBE       LicensePrivilege = LicensePrivilege(pm_pb.LicensePrivilege_SUBSCRIBE)
	LicensePrivilege_LICENSE_ACTIONS LicensePrivilege = LicensePrivilege(pm_pb.LicensePrivilege_LICENSE_ACTIONS)
	LicensePrivilege_READ            LicensePrivilege = LicensePrivilege(pm_pb.LicensePrivilege_READ)
	LicensePrivilege_WRITE           LicensePrivilege = LicensePrivilege(pm_pb.LicensePrivilege_WRITE)
)

// Converts LicensePrivilege enum to string.
func (privilege LicensePrivilege) String() string {
	switch privilege {
	case LicensePrivilege_LOWEST:
		return "Lowest"
	case LicensePrivilege_SUBSCRIBE:
		return "Subscribe"
	case LicensePrivilege_LICENSE_ACTIONS:
		return "License actions"
	case LicensePrivilege_READ:
		return "Read"
	case LicensePrivilege_WRITE:
		return "Write"
	default:
		return "Unknown"
	}
}

// Current license information.
type LicenseInfo struct {
	// Number of lidars allowed by the current license.
	LidarCount uint32
	// Expiration timestamp of the current license.
	Expiration time.Time
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
	SensorStatus_NO_DATA        SensorStatus = SensorStatus(pp_pb.SensorStatus_NO_DATA)
	SensorStatus_RECEIVING_DATA SensorStatus = SensorStatus(pp_pb.SensorStatus_RECEIVING_DATA)
	SensorStatus_LOW_FREQUENCY  SensorStatus = SensorStatus(pp_pb.SensorStatus_LOW_FREQUENCY)
	SensorStatus_HIGH_FREQUENCY SensorStatus = SensorStatus(pp_pb.SensorStatus_HIGH_FREQUENCY)
	SensorStatus_TILTED         SensorStatus = SensorStatus(pp_pb.SensorStatus_TILTED)
	SensorStatus_OBSTRUCTED     SensorStatus = SensorStatus(pp_pb.SensorStatus_OBSTRUCTED)
)

// Converts SensorHealth flag enum to string.
func (status SensorStatus) String() string {
	switch status {
	case SensorStatus_NO_DATA:
		return "No data"
	case SensorStatus_RECEIVING_DATA:
		return "Receiving data"
	case SensorStatus_LOW_FREQUENCY:
		return "Low frequency"
	case SensorStatus_HIGH_FREQUENCY:
		return "High frequency"
	case SensorStatus_TILTED:
		return "Tilted"
	case SensorStatus_OBSTRUCTED:
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
		statusesToTest := []SensorStatus{SensorStatus_NO_DATA, SensorStatus_RECEIVING_DATA, SensorStatus_LOW_FREQUENCY, SensorStatus_HIGH_FREQUENCY, SensorStatus_TILTED, SensorStatus_OBSTRUCTED}
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
	NodeHealthService_SERVICE_DETECTION        NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_DETECTION)
	NodeHealthService_SERVICE_LIDAR_RUNNER     NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_LIDAR_RUNNER)
	NodeHealthService_SERVICE_NOTIFICATION     NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_NOTIFICATION)
	NodeHealthService_SERVICE_POINT_AGGREGATOR NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_POINT_AGGREGATOR)
	NodeHealthService_SERVICE_PREPROCESSING    NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_PREPROCESSING)
	NodeHealthService_SERVICE_PROJECT_MANAGER  NodeHealthService = NodeHealthService(pm_pb.GetHealthStatusResponse_NodeHealth_SERVICE_PROJECT_MANAGER)
)

// Converts NodeHealthService enum to string.
func (service NodeHealthService) String() string {
	switch service {
	case NodeHealthService_SERVICE_DETECTION:
		return "Detection"
	case NodeHealthService_SERVICE_LIDAR_RUNNER:
		return "Lidar runner"
	case NodeHealthService_SERVICE_NOTIFICATION:
		return "Notification"
	case NodeHealthService_SERVICE_POINT_AGGREGATOR:
		return "Point aggregator"
	case NodeHealthService_SERVICE_PREPROCESSING:
		return "Preprocessing"
	default:
		return "Unknown"
	}
}

// Enum representing the connectivity status of a node.
type NodeHealthStatus uint32

const (
	NodeHealth_UNREACHABLE NodeHealthStatus = NodeHealthStatus(pm_pb.GetHealthStatusResponse_NodeHealth_STATUS_UNREACHABLE)
	NodeHealth_ALIVE       NodeHealthStatus = NodeHealthStatus(pm_pb.GetHealthStatusResponse_NodeHealth_STATUS_ALIVE)
)

// Converts NodeHealthStatus enum to string.
func (status NodeHealthStatus) String() string {
	switch status {
	case NodeHealth_UNREACHABLE:
		return "Unreachable"
	case NodeHealth_ALIVE:
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

	if licenseStatus.HasPrivilege(LicensePrivilege_READ) {
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
