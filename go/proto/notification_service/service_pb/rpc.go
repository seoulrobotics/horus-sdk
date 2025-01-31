package service_pb

import (
	"context"
	"fmt"

	"github.com/seoulrobotics/horus-sdk/go/internal/rpc"

	"github.com/seoulrobotics/horus-sdk/go/proto/rpc_pb"
	"google.golang.org/protobuf/proto"
)

// NotificationServiceClient provides access to a remote NotificationService:
// Service which receives notifications from services, storing and forwarding them.
type NotificationServiceClient struct {
	e *rpc.Endpoint
}

// NewNotificationServiceClient creates a [NotificationServiceClient] which communicates with the
// remote service via e. e cannot be nil.
func NewNotificationServiceClient(e *rpc.Endpoint) *NotificationServiceClient {
	if e == nil {
		panic("Endpoint cannot be nil")
	}
	return &NotificationServiceClient{e: e}
}

// ServiceFullName returns the full name of the NotificationService.
func (*NotificationServiceClient) ServiceFullName() string {
	return "horus.pb.NotificationService"
}

// ServiceId returns the service ID of the NotificationService (2).
func (*NotificationServiceClient) ServiceId() rpc.ServiceId {
	return 2
}

// Endpoint returns the (non-nil) [rpc.Endpoint] that the NotificationService communicates with.
func (c *NotificationServiceClient) Endpoint() *rpc.Endpoint {
	return c.e
}

// Logs a message, storing it and broadcasting it to subscribers.
func (c *NotificationServiceClient) LogMessage(ctx context.Context, req *LogMessageRequest) error {
	return c.e.SendOneWay(ctx, c.ServiceId(), 1, req)
}

// Subscribes to new log and profiling events.
func (c *NotificationServiceClient) Subscribe(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error) {
	res := &rpc_pb.DefaultSubscribeResponse{}
	err := c.e.SendTwoWay(ctx, c.ServiceId(), 3, req, res)
	if err != nil { return nil, err }
	return res, nil
}

// Unsubscribe following a call to `Subscribe()`.
func (c *NotificationServiceClient) Unsubscribe(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error) {
	res := &rpc_pb.DefaultUnsubscribeResponse{}
	err := c.e.SendTwoWay(ctx, c.ServiceId(), 4, req, res)
	if err != nil { return nil, err }
	return res, nil
}

// NotificationListenerServiceClient provides access to a remote NotificationListenerService:
// Service which listens for notifications forwarded by the `NotificationService`.
type NotificationListenerServiceClient struct {
	e *rpc.Endpoint
}

// NewNotificationListenerServiceClient creates a [NotificationListenerServiceClient] which communicates with the
// remote service via e. e cannot be nil.
func NewNotificationListenerServiceClient(e *rpc.Endpoint) *NotificationListenerServiceClient {
	if e == nil {
		panic("Endpoint cannot be nil")
	}
	return &NotificationListenerServiceClient{e: e}
}

// ServiceFullName returns the full name of the NotificationListenerService.
func (*NotificationListenerServiceClient) ServiceFullName() string {
	return "horus.pb.NotificationListenerService"
}

// ServiceId returns the service ID of the NotificationListenerService (10).
func (*NotificationListenerServiceClient) ServiceId() rpc.ServiceId {
	return 10
}

// Endpoint returns the (non-nil) [rpc.Endpoint] that the NotificationListenerService communicates with.
func (c *NotificationListenerServiceClient) Endpoint() *rpc.Endpoint {
	return c.e
}

// Received when a log message is received by the `NotificationService`.
func (c *NotificationListenerServiceClient) NotifyLogMessage(ctx context.Context, req *LogMessageEvent) error {
	return c.e.SendOneWay(ctx, c.ServiceId(), 1, req)
}

// Received when a profiling info is received by the `NotificationService`.
func (c *NotificationListenerServiceClient) NotifyProfilingInfo(ctx context.Context, req *ProfilingInfoEvent) error {
	return c.e.SendOneWay(ctx, c.ServiceId(), 2, req)
}

// Received when a sensor info is received by the `NotificationService`.
func (c *NotificationListenerServiceClient) NotifySensorInfo(ctx context.Context, req *SensorInfoEvent) error {
	return c.e.SendOneWay(ctx, c.ServiceId(), 3, req)
}

// NotificationServiceHandler defines how to handle requests sent to a NotificationService:
// Service which receives notifications from services, storing and forwarding them.
type NotificationServiceHandler struct {
	// Logs a message, storing it and broadcasting it to subscribers.
	LogMessage func(ctx context.Context, req *LogMessageRequest) error
	// Subscribes to new log and profiling events.
	Subscribe func(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error)
	// Unsubscribe following a call to `Subscribe()`.
	Unsubscribe func(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error)
}

// ServiceId returns the service ID of the NotificationService (2).
func (*NotificationServiceHandler) ServiceId() rpc.ServiceId {
	return 2
}

// Handle processes a request sent to the NotificationService and returns its response.
func (h *NotificationServiceHandler) Handle(ctx context.Context, method rpc.MethodId, bytes []byte) (proto.Message, error) {
	switch method {
	case 1:
		req := &LogMessageRequest{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.LogMessage == nil {
			return nil, nil
		}
		return nil, h.LogMessage(ctx, req)
	case 3:
		req := &rpc_pb.DefaultSubscribeRequest{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.Subscribe == nil {
			return nil, nil
		}
		return h.Subscribe(ctx, req)
	case 4:
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

// NotificationServiceInterface defines the methods that a NotificationService must implement:
// Service which receives notifications from services, storing and forwarding them.
type NotificationServiceInterface interface {
	// Logs a message, storing it and broadcasting it to subscribers.
	LogMessage(ctx context.Context, req *LogMessageRequest) error
	// Subscribes to new log and profiling events.
	Subscribe(ctx context.Context, req *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error)
	// Unsubscribe following a call to `Subscribe()`.
	Unsubscribe(ctx context.Context, req *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error)
}

// NotificationServiceInterfaceToHandler converts a [NotificationServiceInterface] to a [rpc.Handler].
func NotificationServiceInterfaceToHandler(h NotificationServiceInterface) rpc.Handler {
	return &NotificationServiceHandler{
		LogMessage: h.LogMessage,
		Subscribe: h.Subscribe,
		Unsubscribe: h.Unsubscribe,
	}
}

// NotificationListenerServiceHandler defines how to handle requests sent to a NotificationListenerService:
// Service which listens for notifications forwarded by the `NotificationService`.
type NotificationListenerServiceHandler struct {
	// Received when a log message is received by the `NotificationService`.
	NotifyLogMessage func(ctx context.Context, req *LogMessageEvent) error
	// Received when a profiling info is received by the `NotificationService`.
	NotifyProfilingInfo func(ctx context.Context, req *ProfilingInfoEvent) error
	// Received when a sensor info is received by the `NotificationService`.
	NotifySensorInfo func(ctx context.Context, req *SensorInfoEvent) error
}

// ServiceId returns the service ID of the NotificationListenerService (10).
func (*NotificationListenerServiceHandler) ServiceId() rpc.ServiceId {
	return 10
}

// Handle processes a request sent to the NotificationListenerService and returns its response.
func (h *NotificationListenerServiceHandler) Handle(ctx context.Context, method rpc.MethodId, bytes []byte) (proto.Message, error) {
	switch method {
	case 1:
		req := &LogMessageEvent{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.NotifyLogMessage == nil {
			return nil, nil
		}
		return nil, h.NotifyLogMessage(ctx, req)
	case 2:
		req := &ProfilingInfoEvent{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.NotifyProfilingInfo == nil {
			return nil, nil
		}
		return nil, h.NotifyProfilingInfo(ctx, req)
	case 3:
		req := &SensorInfoEvent{}
		err := proto.Unmarshal(bytes, req)
		if err != nil {
			return nil, fmt.Errorf("failed to deserialize request: %v", err)
		}
		if h.NotifySensorInfo == nil {
			return nil, nil
		}
		return nil, h.NotifySensorInfo(ctx, req)
	default:
		return nil, fmt.Errorf("method not found: %d", method)
	}
}

// NotificationListenerServiceInterface defines the methods that a NotificationListenerService must implement:
// Service which listens for notifications forwarded by the `NotificationService`.
type NotificationListenerServiceInterface interface {
	// Received when a log message is received by the `NotificationService`.
	NotifyLogMessage(ctx context.Context, req *LogMessageEvent) error
	// Received when a profiling info is received by the `NotificationService`.
	NotifyProfilingInfo(ctx context.Context, req *ProfilingInfoEvent) error
	// Received when a sensor info is received by the `NotificationService`.
	NotifySensorInfo(ctx context.Context, req *SensorInfoEvent) error
}

// NotificationListenerServiceInterfaceToHandler converts a [NotificationListenerServiceInterface] to a [rpc.Handler].
func NotificationListenerServiceInterfaceToHandler(h NotificationListenerServiceInterface) rpc.Handler {
	return &NotificationListenerServiceHandler{
		NotifyLogMessage: h.NotifyLogMessage,
		NotifyProfilingInfo: h.NotifyProfilingInfo,
		NotifySensorInfo: h.NotifySensorInfo,
	}
}
