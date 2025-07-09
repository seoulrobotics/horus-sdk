package horus

import (
	"fmt"

	status_service_messages_pb "github.com/seoulrobotics/horus-sdk/go/proto/status_service/messages_pb"
	status_service_service_pb "github.com/seoulrobotics/horus-sdk/go/proto/status_service/service_pb"
)

type Version struct {
	// Major version.
	Major uint32
	// Patch version.
	Patch uint32
	// pre-release version such as "beta". May be empty.
	Pre string
}

// Builds a Version from protobuf counterpart.
func newVersionFromPb(pb *status_service_messages_pb.Version) *Version {
	return &Version{
		Major: pb.GetMajor(),
		Patch: pb.GetPatch(),
		Pre:   pb.GetPre(),
	}
}

func (version Version) String() string {
	preSep := ""
	if version.Pre != "" {
		preSep = "-"
	}
	return fmt.Sprintf("r%d.%d%s%s", version.Major, version.Patch, preSep, version.Pre)
}

type GetVersionRequest struct {
}

// Builds a protobuf GetVersionRequest from its sdk counterpart.
func (*GetVersionRequest) toPb() *status_service_service_pb.GetVersionRequest {
	return &status_service_service_pb.GetVersionRequest{}
}
