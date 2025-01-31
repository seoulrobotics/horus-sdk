package detection_service_pb

import (
	"context"
	"fmt"

	"github.com/seoulrobotics/horus-sdk/go/internal/rpc"

	"github.com/seoulrobotics/horus-sdk/go/proto/detection_service/detection_pb"
	"github.com/seoulrobotics/horus-sdk/go/proto/rpc_pb"
	"google.golang.org/protobuf/proto"
)

// DetectionServiceClient provides access to a remote DetectionService:
// Service for receiving pre-processed foreground points and sending detection results.
type DetectionServiceClient struct {
	e *rpc.Endpoint
}

// NewDetectionServiceClient creates a [DetectionServiceClient] which communicates with the
// remote service via e. e cannot be nil.
func NewDetectionServiceClient(e *rpc.Endpoint) *DetectionServiceClient {
	if e == nil {
		panic("Endpoint cannot be nil")
	}
	return &DetectionServiceClient{e: e}
}

// ServiceFullName returns the full name of the DetectionService.
func (*DetectionServiceClient) ServiceFullName() string {
	return "horus.pb.DetectionService"
}

// ServiceId returns the service ID of the DetectionService (7).
func (*DetectionServiceClient) ServiceId() rpc.ServiceId {
	return 7
}

// Endpoint returns the (non-nil) [rpc.Endpoint] that the DetectionService communicates with.
func (c *DetectionServiceClient) Endpoint() *rpc.Endpoint {
	return c.e
}

// Subscribes to receive future detection results as they are made available.
func (c *DetectionServiceClient) Subscribe(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error) {
	res := &rpc_pb.DefaultSubscribeResponse{}
	err := c.e.SendTwoWay(ctx, c.ServiceId(), 2, req, res)
	if err != nil { return nil, err }
	return res, nil
}

// Unsubscribe following a call to `Subscribe()`.
func (c *DetectionServiceClient) Unsubscribe(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error) {
	res := &rpc_pb.DefaultUnsubscribeResponse{}
	err := c.e.SendTwoWay(ctx, c.ServiceId(), 3, req, res)
	if err != nil { return nil, err }
	return res, nil
}

// DetectionSubscriberServiceClient provides access to a remote DetectionSubscriberService:
// Service exposed by clients of the `DetectionService` that wish to receive updates when new detection results are processed.
type DetectionSubscriberServiceClient struct {
	e *rpc.Endpoint
}

// NewDetectionSubscriberServiceClient creates a [DetectionSubscriberServiceClient] which communicates with the
// remote service via e. e cannot be nil.
func NewDetectionSubscriberServiceClient(e *rpc.Endpoint) *DetectionSubscriberServiceClient {
	if e == nil {
		panic("Endpoint cannot be nil")
	}
	return &DetectionSubscriberServiceClient{e: e}
}

// ServiceFullName returns the full name of the DetectionSubscriberService.
func (*DetectionSubscriberServiceClient) ServiceFullName() string {
	return "horus.pb.DetectionSubscriberService"
}

// ServiceId returns the service ID of the DetectionSubscriberService (12).
func (*DetectionSubscriberServiceClient) ServiceId() rpc.ServiceId {
	return 12
}

// Endpoint returns the (non-nil) [rpc.Endpoint] that the DetectionSubscriberService communicates with.
func (c *DetectionSubscriberServiceClient) Endpoint() *rpc.Endpoint {
	return c.e
}

// Notify new detection results.
func (c *DetectionSubscriberServiceClient) BroadcastDetection(ctx context.Context, req *detection_pb.DetectionEvent) error {
	return c.e.SendOneWay(ctx, c.ServiceId(), 1, req)
}

// DetectionServiceHandler defines how to handle requests sent to a DetectionService:
// Service for receiving pre-processed foreground points and sending detection results.
type DetectionServiceHandler struct {
	// Subscribes to receive future detection results as they are made available.
	Subscribe func(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error)
	// Unsubscribe following a call to `Subscribe()`.
	Unsubscribe func(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error)
}

// ServiceId returns the service ID of the DetectionService (7).
func (*DetectionServiceHandler) ServiceId() rpc.ServiceId {
	return 7
}

// Handle processes a request sent to the DetectionService and returns its response.
func (h *DetectionServiceHandler) Handle(ctx context.Context, method rpc.MethodId, bytes []byte) (proto.Message, error) {
	switch method {
	case 2:
		req := &rpc_pb.DefaultSubscribeRequest{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.Subscribe == nil {
			return nil, nil
		}
		return h.Subscribe(ctx, req)
	case 3:
		req := &rpc_pb.DefaultUnsubscribeRequest{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.Unsubscribe == nil {
			return nil, nil
		}
		return h.Unsubscribe(ctx, req)
	default:
		return nil, fmt.Errorf("method not found: %d", method)
	}
}

// DetectionServiceInterface defines the methods that a DetectionService must implement:
// Service for receiving pre-processed foreground points and sending detection results.
type DetectionServiceInterface interface {
	// Subscribes to receive future detection results as they are made available.
	Subscribe(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error)
	// Unsubscribe following a call to `Subscribe()`.
	Unsubscribe(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error)
}

// DetectionServiceInterfaceToHandler converts a [DetectionServiceInterface] to a [rpc.Handler].
func DetectionServiceInterfaceToHandler(h DetectionServiceInterface) rpc.Handler {
	return &DetectionServiceHandler{
		Subscribe: h.Subscribe,
		Unsubscribe: h.Unsubscribe,
	}
}

// DetectionSubscriberServiceHandler defines how to handle requests sent to a DetectionSubscriberService:
// Service exposed by clients of the `DetectionService` that wish to receive updates when new detection results are processed.
type DetectionSubscriberServiceHandler struct {
	// Notify new detection results.
	BroadcastDetection func(ctx context.Context, req *detection_pb.DetectionEvent) error
}

// ServiceId returns the service ID of the DetectionSubscriberService (12).
func (*DetectionSubscriberServiceHandler) ServiceId() rpc.ServiceId {
	return 12
}

// Handle processes a request sent to the DetectionSubscriberService and returns its response.
func (h *DetectionSubscriberServiceHandler) Handle(ctx context.Context, method rpc.MethodId, bytes []byte) (proto.Message, error) {
	switch method {
	case 1:
		req := &detection_pb.DetectionEvent{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.BroadcastDetection == nil {
			return nil, nil
		}
		return nil, h.BroadcastDetection(ctx, req)
	default:
		return nil, fmt.Errorf("method not found: %d", method)
	}
}

// DetectionSubscriberServiceInterface defines the methods that a DetectionSubscriberService must implement:
// Service exposed by clients of the `DetectionService` that wish to receive updates when new detection results are processed.
type DetectionSubscriberServiceInterface interface {
	// Notify new detection results.
	BroadcastDetection(ctx context.Context, req *detection_pb.DetectionEvent) error
}

// DetectionSubscriberServiceInterfaceToHandler converts a [DetectionSubscriberServiceInterface] to a [rpc.Handler].
func DetectionSubscriberServiceInterfaceToHandler(h DetectionSubscriberServiceInterface) rpc.Handler {
	return &DetectionSubscriberServiceHandler{
		BroadcastDetection: h.BroadcastDetection,
	}
}
