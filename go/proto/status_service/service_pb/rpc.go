package service_pb

import (
	"context"
	"fmt"

	"github.com/seoulrobotics/horus-sdk/go/internal/rpc"

	"google.golang.org/protobuf/proto"
)

// StatusServiceClient provides access to a remote StatusService:
// Service running in every binary, returning status information.
type StatusServiceClient struct {
	e *rpc.Endpoint
}

// NewStatusServiceClient creates a [StatusServiceClient] which communicates with the
// remote service via e. e cannot be nil.
func NewStatusServiceClient(e *rpc.Endpoint) *StatusServiceClient {
	if e == nil {
		panic("Endpoint cannot be nil")
	}
	return &StatusServiceClient{e: e}
}

// ServiceFullName returns the full name of the StatusService.
func (*StatusServiceClient) ServiceFullName() string {
	return "horus.pb.StatusService"
}

// ServiceId returns the service ID of the StatusService (15).
func (*StatusServiceClient) ServiceId() rpc.ServiceId {
	return 15
}

// Endpoint returns the (non-nil) [rpc.Endpoint] that the StatusService communicates with.
func (c *StatusServiceClient) Endpoint() *rpc.Endpoint {
	return c.e
}

// Returns the current version of horus.
func (c *StatusServiceClient) GetVersion(ctx context.Context, req *GetVersionRequest) (*GetVersionResponse, error) {
	res := &GetVersionResponse{}
	err := c.e.SendTwoWay(ctx, c.ServiceId(), 1, req, res)
	if err != nil { return nil, err }
	return res, nil
}

// StatusServiceHandler defines how to handle requests sent to a StatusService:
// Service running in every binary, returning status information.
type StatusServiceHandler struct {
	// Returns the current version of horus.
	GetVersion func(ctx context.Context, req *GetVersionRequest) (*GetVersionResponse, error)
}

// ServiceId returns the service ID of the StatusService (15).
func (*StatusServiceHandler) ServiceId() rpc.ServiceId {
	return 15
}

// Handle processes a request sent to the StatusService and returns its response.
func (h *StatusServiceHandler) Handle(ctx context.Context, method rpc.MethodId, bytes []byte) (proto.Message, error) {
	switch method {
	case 1:
		req := &GetVersionRequest{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.GetVersion == nil {
			return nil, nil
		}
		return h.GetVersion(ctx, req)
	default:
		return nil, fmt.Errorf("method not found: %d", method)
	}
}

// StatusServiceInterface defines the methods that a StatusService must implement:
// Service running in every binary, returning status information.
type StatusServiceInterface interface {
	// Returns the current version of horus.
	GetVersion(ctx context.Context, req *GetVersionRequest) (*GetVersionResponse, error)
}

// StatusServiceInterfaceToHandler converts a [StatusServiceInterface] to a [rpc.Handler].
func StatusServiceInterfaceToHandler(h StatusServiceInterface) rpc.Handler {
	return &StatusServiceHandler{
		GetVersion: h.GetVersion,
	}
}
