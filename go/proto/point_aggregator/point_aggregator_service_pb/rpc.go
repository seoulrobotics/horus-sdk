package point_aggregator_service_pb

import (
	"context"
	"fmt"

	"github.com/seoulrobotics/horus-sdk/go/internal/rpc"

	"github.com/seoulrobotics/horus-sdk/go/proto/point/point_message_pb"
	"github.com/seoulrobotics/horus-sdk/go/proto/preprocessing/messages_pb"
	"github.com/seoulrobotics/horus-sdk/go/proto/rpc_pb"
	"google.golang.org/protobuf/proto"
)

// PointAggregatorServiceClient provides access to a remote PointAggregatorService:
// Service for receiving pre-processed point clouds.
type PointAggregatorServiceClient struct {
	e *rpc.Endpoint
}

// NewPointAggregatorServiceClient creates a [PointAggregatorServiceClient] which communicates with the
// remote service via e. e cannot be nil.
func NewPointAggregatorServiceClient(e *rpc.Endpoint) *PointAggregatorServiceClient {
	if e == nil {
		panic("Endpoint cannot be nil")
	}
	return &PointAggregatorServiceClient{e: e}
}

// ServiceFullName returns the full name of the PointAggregatorService.
func (*PointAggregatorServiceClient) ServiceFullName() string {
	return "horus.pb.PointAggregatorService"
}

// ServiceId returns the service ID of the PointAggregatorService (6).
func (*PointAggregatorServiceClient) ServiceId() rpc.ServiceId {
	return 6
}

// Endpoint returns the (non-nil) [rpc.Endpoint] that the PointAggregatorService communicates with.
func (c *PointAggregatorServiceClient) Endpoint() *rpc.Endpoint {
	return c.e
}

// Subscribes to receive future processed point clouds as they are made available.
func (c *PointAggregatorServiceClient) Subscribe(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error) {
	res := &rpc_pb.DefaultSubscribeResponse{}
	err := c.e.SendTwoWay(ctx, c.ServiceId(), 5, req, res)
	if err != nil { return nil, err }
	return res, nil
}

// Unsubscribe following a call to `Subscribe()`.
func (c *PointAggregatorServiceClient) Unsubscribe(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error) {
	res := &rpc_pb.DefaultUnsubscribeResponse{}
	err := c.e.SendTwoWay(ctx, c.ServiceId(), 6, req, res)
	if err != nil { return nil, err }
	return res, nil
}

// PointAggregatorSubscriberServiceClient provides access to a remote PointAggregatorSubscriberService:
// Service exposed by clients of the `PointAggregatorService` that wish to receive updates when new processed points are received.
type PointAggregatorSubscriberServiceClient struct {
	e *rpc.Endpoint
}

// NewPointAggregatorSubscriberServiceClient creates a [PointAggregatorSubscriberServiceClient] which communicates with the
// remote service via e. e cannot be nil.
func NewPointAggregatorSubscriberServiceClient(e *rpc.Endpoint) *PointAggregatorSubscriberServiceClient {
	if e == nil {
		panic("Endpoint cannot be nil")
	}
	return &PointAggregatorSubscriberServiceClient{e: e}
}

// ServiceFullName returns the full name of the PointAggregatorSubscriberService.
func (*PointAggregatorSubscriberServiceClient) ServiceFullName() string {
	return "horus.pb.PointAggregatorSubscriberService"
}

// ServiceId returns the service ID of the PointAggregatorSubscriberService (5).
func (*PointAggregatorSubscriberServiceClient) ServiceId() rpc.ServiceId {
	return 5
}

// Endpoint returns the (non-nil) [rpc.Endpoint] that the PointAggregatorSubscriberService communicates with.
func (c *PointAggregatorSubscriberServiceClient) Endpoint() *rpc.Endpoint {
	return c.e
}

// Notify new processed points input.
func (c *PointAggregatorSubscriberServiceClient) BroadcastProcessedPoints(ctx context.Context, req *point_message_pb.AggregatedPointEvents) error {
	return c.e.SendOneWay(ctx, c.ServiceId(), 2, req)
}

// Notify new occupancy grid input.
func (c *PointAggregatorSubscriberServiceClient) BroadcastOccupancyGrid(ctx context.Context, req *messages_pb.OccupancyGridEvent) error {
	return c.e.SendOneWay(ctx, c.ServiceId(), 3, req)
}

// PointAggregatorServiceHandler defines how to handle requests sent to a PointAggregatorService:
// Service for receiving pre-processed point clouds.
type PointAggregatorServiceHandler struct {
	// Subscribes to receive future processed point clouds as they are made available.
	Subscribe func(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error)
	// Unsubscribe following a call to `Subscribe()`.
	Unsubscribe func(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error)
}

// ServiceId returns the service ID of the PointAggregatorService (6).
func (*PointAggregatorServiceHandler) ServiceId() rpc.ServiceId {
	return 6
}

// Handle processes a request sent to the PointAggregatorService and returns its response.
func (h *PointAggregatorServiceHandler) Handle(ctx context.Context, method rpc.MethodId, bytes []byte) (proto.Message, error) {
	switch method {
	case 5:
		req := &rpc_pb.DefaultSubscribeRequest{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.Subscribe == nil {
			return nil, nil
		}
		return h.Subscribe(ctx, req)
	case 6:
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

// PointAggregatorServiceInterface defines the methods that a PointAggregatorService must implement:
// Service for receiving pre-processed point clouds.
type PointAggregatorServiceInterface interface {
	// Subscribes to receive future processed point clouds as they are made available.
	Subscribe(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error)
	// Unsubscribe following a call to `Subscribe()`.
	Unsubscribe(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error)
}

// PointAggregatorServiceInterfaceToHandler converts a [PointAggregatorServiceInterface] to a [rpc.Handler].
func PointAggregatorServiceInterfaceToHandler(h PointAggregatorServiceInterface) rpc.Handler {
	return &PointAggregatorServiceHandler{
		Subscribe: h.Subscribe,
		Unsubscribe: h.Unsubscribe,
	}
}

// PointAggregatorSubscriberServiceHandler defines how to handle requests sent to a PointAggregatorSubscriberService:
// Service exposed by clients of the `PointAggregatorService` that wish to receive updates when new processed points are received.
type PointAggregatorSubscriberServiceHandler struct {
	// Notify new processed points input.
	BroadcastProcessedPoints func(ctx context.Context, req *point_message_pb.AggregatedPointEvents) error
	// Notify new occupancy grid input.
	BroadcastOccupancyGrid func(ctx context.Context, req *messages_pb.OccupancyGridEvent) error
}

// ServiceId returns the service ID of the PointAggregatorSubscriberService (5).
func (*PointAggregatorSubscriberServiceHandler) ServiceId() rpc.ServiceId {
	return 5
}

// Handle processes a request sent to the PointAggregatorSubscriberService and returns its response.
func (h *PointAggregatorSubscriberServiceHandler) Handle(ctx context.Context, method rpc.MethodId, bytes []byte) (proto.Message, error) {
	switch method {
	case 2:
		req := &point_message_pb.AggregatedPointEvents{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.BroadcastProcessedPoints == nil {
			return nil, nil
		}
		return nil, h.BroadcastProcessedPoints(ctx, req)
	case 3:
		req := &messages_pb.OccupancyGridEvent{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.BroadcastOccupancyGrid == nil {
			return nil, nil
		}
		return nil, h.BroadcastOccupancyGrid(ctx, req)
	default:
		return nil, fmt.Errorf("method not found: %d", method)
	}
}

// PointAggregatorSubscriberServiceInterface defines the methods that a PointAggregatorSubscriberService must implement:
// Service exposed by clients of the `PointAggregatorService` that wish to receive updates when new processed points are received.
type PointAggregatorSubscriberServiceInterface interface {
	// Notify new processed points input.
	BroadcastProcessedPoints(ctx context.Context, req *point_message_pb.AggregatedPointEvents) error
	// Notify new occupancy grid input.
	BroadcastOccupancyGrid(ctx context.Context, req *messages_pb.OccupancyGridEvent) error
}

// PointAggregatorSubscriberServiceInterfaceToHandler converts a [PointAggregatorSubscriberServiceInterface] to a [rpc.Handler].
func PointAggregatorSubscriberServiceInterfaceToHandler(h PointAggregatorSubscriberServiceInterface) rpc.Handler {
	return &PointAggregatorSubscriberServiceHandler{
		BroadcastProcessedPoints: h.BroadcastProcessedPoints,
		BroadcastOccupancyGrid: h.BroadcastOccupancyGrid,
	}
}
