package service_pb

import (
	"context"
	"fmt"

	"github.com/seoulrobotics/horus-sdk/go/internal/rpc"

	"github.com/seoulrobotics/horus-sdk/go/proto/detection_service/detection_pb"
	"github.com/seoulrobotics/horus-sdk/go/proto/rpc_pb"
	"google.golang.org/protobuf/proto"
)

// DetectionMergerServiceClient provides access to a remote DetectionMergerService:
// Service for merging detection results from multiple sources.
type DetectionMergerServiceClient struct {
	e *rpc.Endpoint
}

// NewDetectionMergerServiceClient creates a [DetectionMergerServiceClient] which communicates with the
// remote service via e. e cannot be nil.
func NewDetectionMergerServiceClient(e *rpc.Endpoint) *DetectionMergerServiceClient {
	if e == nil {
		panic("Endpoint cannot be nil")
	}
	return &DetectionMergerServiceClient{e: e}
}

// ServiceFullName returns the full name of the DetectionMergerService.
func (*DetectionMergerServiceClient) ServiceFullName() string {
	return "horus.pb.DetectionMergerService"
}

// ServiceId returns the service ID of the DetectionMergerService (11).
func (*DetectionMergerServiceClient) ServiceId() rpc.ServiceId {
	return 11
}

// Endpoint returns the (non-nil) [rpc.Endpoint] that the DetectionMergerService communicates with.
func (c *DetectionMergerServiceClient) Endpoint() *rpc.Endpoint {
	return c.e
}

// Subscribe to receive merged detection results.
func (c *DetectionMergerServiceClient) Subscribe(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error) {
	res := &rpc_pb.DefaultSubscribeResponse{}
	err := c.e.SendTwoWay(ctx, c.ServiceId(), 1, req, res)
	if err != nil { return nil, err }
	return res, nil
}

// Unsubscribe from merged detection results.
func (c *DetectionMergerServiceClient) Unsubscribe(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error) {
	res := &rpc_pb.DefaultUnsubscribeResponse{}
	err := c.e.SendTwoWay(ctx, c.ServiceId(), 2, req, res)
	if err != nil { return nil, err }
	return res, nil
}

// DetectionMergerSubscriberServiceClient provides access to a remote DetectionMergerSubscriberService:
// Service for receiving merged detection results.
type DetectionMergerSubscriberServiceClient struct {
	e *rpc.Endpoint
}

// NewDetectionMergerSubscriberServiceClient creates a [DetectionMergerSubscriberServiceClient] which communicates with the
// remote service via e. e cannot be nil.
func NewDetectionMergerSubscriberServiceClient(e *rpc.Endpoint) *DetectionMergerSubscriberServiceClient {
	if e == nil {
		panic("Endpoint cannot be nil")
	}
	return &DetectionMergerSubscriberServiceClient{e: e}
}

// ServiceFullName returns the full name of the DetectionMergerSubscriberService.
func (*DetectionMergerSubscriberServiceClient) ServiceFullName() string {
	return "horus.pb.DetectionMergerSubscriberService"
}

// ServiceId returns the service ID of the DetectionMergerSubscriberService (16).
func (*DetectionMergerSubscriberServiceClient) ServiceId() rpc.ServiceId {
	return 16
}

// Endpoint returns the (non-nil) [rpc.Endpoint] that the DetectionMergerSubscriberService communicates with.
func (c *DetectionMergerSubscriberServiceClient) Endpoint() *rpc.Endpoint {
	return c.e
}

// Receive merged detection results.
func (c *DetectionMergerSubscriberServiceClient) BroadcastDetection(ctx context.Context, req *detection_pb.DetectionEvent) error {
	return c.e.SendOneWay(ctx, c.ServiceId(), 1, req)
}

// DetectionMergerServiceHandler defines how to handle requests sent to a DetectionMergerService:
// Service for merging detection results from multiple sources.
type DetectionMergerServiceHandler struct {
	// Subscribe to receive merged detection results.
	Subscribe func(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error)
	// Unsubscribe from merged detection results.
	Unsubscribe func(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error)
}

// ServiceId returns the service ID of the DetectionMergerService (11).
func (*DetectionMergerServiceHandler) ServiceId() rpc.ServiceId {
	return 11
}

// Handle processes a request sent to the DetectionMergerService and returns its response.
func (h *DetectionMergerServiceHandler) Handle(ctx context.Context, method rpc.MethodId, bytes []byte) (proto.Message, error) {
	switch method {
	case 1:
		req := &rpc_pb.DefaultSubscribeRequest{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.Subscribe == nil {
			return nil, nil
		}
		return h.Subscribe(ctx, req)
	case 2:
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

// DetectionMergerServiceInterface defines the methods that a DetectionMergerService must implement:
// Service for merging detection results from multiple sources.
type DetectionMergerServiceInterface interface {
	// Subscribe to receive merged detection results.
	Subscribe(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error)
	// Unsubscribe from merged detection results.
	Unsubscribe(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error)
}

// DetectionMergerServiceInterfaceToHandler converts a [DetectionMergerServiceInterface] to a [rpc.Handler].
func DetectionMergerServiceInterfaceToHandler(h DetectionMergerServiceInterface) rpc.Handler {
	return &DetectionMergerServiceHandler{
		Subscribe: h.Subscribe,
		Unsubscribe: h.Unsubscribe,
	}
}

// DetectionMergerSubscriberServiceHandler defines how to handle requests sent to a DetectionMergerSubscriberService:
// Service for receiving merged detection results.
type DetectionMergerSubscriberServiceHandler struct {
	// Receive merged detection results.
	BroadcastDetection func(ctx context.Context, req *detection_pb.DetectionEvent) error
}

// ServiceId returns the service ID of the DetectionMergerSubscriberService (16).
func (*DetectionMergerSubscriberServiceHandler) ServiceId() rpc.ServiceId {
	return 16
}

// Handle processes a request sent to the DetectionMergerSubscriberService and returns its response.
func (h *DetectionMergerSubscriberServiceHandler) Handle(ctx context.Context, method rpc.MethodId, bytes []byte) (proto.Message, error) {
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

// DetectionMergerSubscriberServiceInterface defines the methods that a DetectionMergerSubscriberService must implement:
// Service for receiving merged detection results.
type DetectionMergerSubscriberServiceInterface interface {
	// Receive merged detection results.
	BroadcastDetection(ctx context.Context, req *detection_pb.DetectionEvent) error
}

// DetectionMergerSubscriberServiceInterfaceToHandler converts a [DetectionMergerSubscriberServiceInterface] to a [rpc.Handler].
func DetectionMergerSubscriberServiceInterfaceToHandler(h DetectionMergerSubscriberServiceInterface) rpc.Handler {
	return &DetectionMergerSubscriberServiceHandler{
		BroadcastDetection: h.BroadcastDetection,
	}
}
