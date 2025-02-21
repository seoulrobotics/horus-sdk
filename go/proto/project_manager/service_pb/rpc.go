package service_pb

import (
	"context"
	"fmt"

	"github.com/seoulrobotics/horus-sdk/go/internal/rpc"

	"google.golang.org/protobuf/proto"
)

// ProjectManagerServiceClient provides access to a remote ProjectManagerService:
// Service which stores configuration for all other services and coordinates updates to this configuration.
type ProjectManagerServiceClient struct {
	e *rpc.Endpoint
}

// NewProjectManagerServiceClient creates a [ProjectManagerServiceClient] which communicates with the
// remote service via e. e cannot be nil.
func NewProjectManagerServiceClient(e *rpc.Endpoint) *ProjectManagerServiceClient {
	if e == nil {
		panic("Endpoint cannot be nil")
	}
	return &ProjectManagerServiceClient{e: e}
}

// ServiceFullName returns the full name of the ProjectManagerService.
func (*ProjectManagerServiceClient) ServiceFullName() string {
	return "horus.pb.ProjectManagerService"
}

// ServiceId returns the service ID of the ProjectManagerService (3).
func (*ProjectManagerServiceClient) ServiceId() rpc.ServiceId {
	return 3
}

// Endpoint returns the (non-nil) [rpc.Endpoint] that the ProjectManagerService communicates with.
func (c *ProjectManagerServiceClient) Endpoint() *rpc.Endpoint {
	return c.e
}

// Provides horus health indicators.
func (c *ProjectManagerServiceClient) GetHealthStatus(ctx context.Context, req *GetHealthStatusRequest) (*GetHealthStatusResponse, error) {
	res := &GetHealthStatusResponse{}
	err := c.e.SendTwoWay(ctx, c.ServiceId(), 38, req, res)
	if err != nil { return nil, err }
	return res, nil
}

// ProjectManagerServiceHandler defines how to handle requests sent to a ProjectManagerService:
// Service which stores configuration for all other services and coordinates updates to this configuration.
type ProjectManagerServiceHandler struct {
	// Provides horus health indicators.
	GetHealthStatus func(ctx context.Context, req *GetHealthStatusRequest) (*GetHealthStatusResponse, error)
}

// ServiceId returns the service ID of the ProjectManagerService (3).
func (*ProjectManagerServiceHandler) ServiceId() rpc.ServiceId {
	return 3
}

// Handle processes a request sent to the ProjectManagerService and returns its response.
func (h *ProjectManagerServiceHandler) Handle(ctx context.Context, method rpc.MethodId, bytes []byte) (proto.Message, error) {
	switch method {
	case 38:
		req := &GetHealthStatusRequest{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.GetHealthStatus == nil {
			return nil, nil
		}
		return h.GetHealthStatus(ctx, req)
	default:
		return nil, fmt.Errorf("method not found: %d", method)
	}
}

// ProjectManagerServiceInterface defines the methods that a ProjectManagerService must implement:
// Service which stores configuration for all other services and coordinates updates to this configuration.
type ProjectManagerServiceInterface interface {
	// Provides horus health indicators.
	GetHealthStatus(ctx context.Context, req *GetHealthStatusRequest) (*GetHealthStatusResponse, error)
}

// ProjectManagerServiceInterfaceToHandler converts a [ProjectManagerServiceInterface] to a [rpc.Handler].
func ProjectManagerServiceInterfaceToHandler(h ProjectManagerServiceInterface) rpc.Handler {
	return &ProjectManagerServiceHandler{
		GetHealthStatus: h.GetHealthStatus,
	}
}
