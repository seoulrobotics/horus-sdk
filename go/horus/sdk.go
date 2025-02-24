// Package horus provides an [Sdk] used to access Horus services.
package horus

import (
	"context"
	"log/slog"
	"sync"
	"sync/atomic"

	"github.com/seoulrobotics/horus-sdk/go/internal"
	"github.com/seoulrobotics/horus-sdk/go/internal/rpc"
	"github.com/seoulrobotics/horus-sdk/go/proto/detection_service/detection_pb"
	"github.com/seoulrobotics/horus-sdk/go/proto/detection_service/detection_service_pb"
	"github.com/seoulrobotics/horus-sdk/go/proto/point/point_message_pb"
	"github.com/seoulrobotics/horus-sdk/go/proto/point_aggregator/point_aggregator_service_pb"
	project_manager_service_pb "github.com/seoulrobotics/horus-sdk/go/proto/project_manager/service_pb"
	"github.com/seoulrobotics/horus-sdk/go/proto/rpc_pb"
	"golang.org/x/sync/errgroup"
	"google.golang.org/protobuf/proto"
)

// Sdk provides access to Horus services.
type Sdk struct {
	logger *slog.Logger

	detection       *detection_service_pb.DetectionServiceClient
	pointAggregator *point_aggregator_service_pb.PointAggregatorServiceClient
	projectManager  *project_manager_service_pb.ProjectManagerServiceClient

	detectionRcs       *rcSubscription
	pointAggregatorRcs *rcSubscription

	detectionEventHandlers handlerSet[*detection_pb.DetectionEvent]
	pointFrameHandlers     handlerSet[*point_message_pb.AggregatedPointEvents]
}

// SdkOptions specifies options for creating an [Sdk] with [NewSdk].
type SdkOptions struct {
	// The services to connect to. Defaults to [DefaultServices] if nil.
	Services *Services
	// Logger for internal events. Defaults to [slog.Default] if nil.
	Logger *slog.Logger

	// Function called when a connection is established.
	OnConnected func(ServiceInfo)
	// Function called when a connection is closed. Every [SdkOptions.OnConnected] call is always
	// followed by exactly one [SdkOptions.OnDisconnected] call. The error will be nil if the
	// disconnection is intentional (i.e. requested by [Sdk.Close]).
	OnDisconnected func(ServiceInfo, error)
	// Function called when an error is encountered _outside of a disconnection_.
	OnError func(ServiceInfo, error)
}

var defaultServices = DefaultServices()

// NewSdk creates an [Sdk].
func NewSdk(ctx context.Context, options SdkOptions) (*Sdk, error) {
	// Set up SDK.
	services := options.Services
	if services == nil {
		services = &defaultServices
	}

	logger := options.Logger
	if logger == nil {
		logger = slog.Default()
	}

	sdk := &Sdk{
		logger:                 logger,
		detectionEventHandlers: newHandlerSet[*detection_pb.DetectionEvent](),
		pointFrameHandlers:     newHandlerSet[*point_message_pb.AggregatedPointEvents](),
	}

	// Connect to services. Use an errgroup to connect concurrently.
	var g errgroup.Group

	g.Go(func() (err error) {
		sdk.detection, sdk.detectionRcs, err = newSubscribingClient(
			ctx,
			logger,
			options,
			services.Detection,
			detection_service_pb.NewDetectionServiceClient,
			&detection_service_pb.DetectionSubscriberServiceHandler{
				BroadcastDetection: sdk.detectionEventHandlers.handle,
			},
		)
		return
	})

	g.Go(func() (err error) {
		sdk.pointAggregator, sdk.pointAggregatorRcs, err = newSubscribingClient(
			ctx,
			logger,
			options,
			services.PointAggregator,
			point_aggregator_service_pb.NewPointAggregatorServiceClient,
			&point_aggregator_service_pb.PointAggregatorSubscriberServiceHandler{
				BroadcastProcessedPoints: sdk.pointFrameHandlers.handle,
			},
		)
		return
	})

	g.Go(func() (err error) {
		sdk.projectManager, err = newClient(
			ctx,
			logger,
			options,
			services.ProjectManager,
			project_manager_service_pb.NewProjectManagerServiceClient,
			&project_manager_service_pb.ProjectManagerServiceHandler{},
		)
		return
	})

	err := g.Wait()
	if err != nil {
		return nil, err
	}
	return sdk, nil
}

func (sdk *Sdk) GetHealthStatus(req GetHealthStatusRequest) (*HealthStatus, error) {
	ctx := context.Background()

	pbHealthStatus, err := sdk.projectManager.GetHealthStatus(ctx, req.toPb())
	if err != nil {
		return nil, err
	}

	healthStatus, err := newHealthStatusFromPb(pbHealthStatus)
	if err != nil {
		return nil, err
	}

	return healthStatus, nil
}

// SubscribeToObjects subscribes to object detection events and calls f for each event received.
func (sdk *Sdk) SubscribeToObjects(f func(*detection_pb.DetectionEvent)) *Subscription {
	cb := &f
	sdk.detectionEventHandlers.addHandler(cb)
	return sdk.detectionRcs.addSubscription(func() { sdk.detectionEventHandlers.removeHandler(cb) })
}

// SubscribeToPointClouds subscribes to point cloud broadcasts and calls f for each frame received.
func (sdk *Sdk) SubscribeToPointClouds(f func(*point_message_pb.PointFrame)) *Subscription {
	cbf := func(req *point_message_pb.AggregatedPointEvents) {
		for _, event := range req.GetEvents() {
			f(event.GetPointFrame())
		}
	}
	cb := &cbf
	sdk.pointFrameHandlers.addHandler(cb)
	return sdk.pointAggregatorRcs.addSubscription(func() { sdk.pointFrameHandlers.removeHandler(cb) })
}

// Close closes all current connections used by the SDK.
func (sdk *Sdk) Close() error {
	clear(sdk.detectionRcs.handlers)
	clear(sdk.pointAggregatorRcs.handlers)

	var g errgroup.Group

	g.Go(sdk.detection.Endpoint().Close)
	g.Go(sdk.pointAggregator.Endpoint().Close)

	return g.Wait()
}

// A Subscription provides the ability to unsubscribe from a subscription created via the [Sdk].
type Subscription struct {
	rcs   *rcSubscription
	close func()
}

// Close unsubscribes from the subscription.
func (s *Subscription) Close() error {
	if s.rcs != nil {
		s.rcs.removeSubscription(s)
		s.rcs = nil
		s.close()
	}
	return nil
}

// A handlerSet is a set of handlers for a given message type.
type handlerSet[M proto.Message] struct {
	handlers map[*func(M)]struct{}
}

func newHandlerSet[M proto.Message]() handlerSet[M] {
	return handlerSet[M]{handlers: make(map[*func(M)]struct{})}
}

// handle calls each handler in the set with the given message.
func (hs *handlerSet[M]) handle(_ context.Context, req M) error {
	wg := sync.WaitGroup{}
	wg.Add(len(hs.handlers))

	for f := range hs.handlers {
		go func() {
			(*f)(req)
			wg.Done()
		}()
	}

	wg.Wait()
	return nil
}

// addHandler adds a handler to the set.
func (hs *handlerSet[M]) addHandler(f *func(M)) {
	hs.handlers[f] = struct{}{}
}

// removeHandler removes a handler from the set.
func (hs *handlerSet[M]) removeHandler(f *func(M)) {
	delete(hs.handlers, f)
}

// A rpcClient is an object used to perform all the requests to an endpoint.
type rpcClient interface {
	Endpoint() *rpc.Endpoint
	ServiceFullName() string
}

// A defaultSubscribable is a service that can be subscribed to and unsubscribed from with default
// request and response types.
type defaultSubscribable interface {
	rpcClient
	Subscribe(context.Context, *rpc_pb.DefaultSubscribeRequest) (*rpc_pb.DefaultSubscribeResponse, error)
	Unsubscribe(context.Context, *rpc_pb.DefaultUnsubscribeRequest) (*rpc_pb.DefaultUnsubscribeResponse, error)
}

// An rcSubscription is a reference-counted subscription to a service.
type rcSubscription struct {
	logger      *slog.Logger
	service     defaultSubscribable
	handlers    map[*Subscription]struct{}
	hasHandlers atomic.Bool
}

// addSubscription adds a subscription to the service. If this is the first subscription, it sends
// a Subscribe() request as well. close() and removeSubscription() will be called when the
// subscription is closed.
func (s *rcSubscription) addSubscription(close func()) *Subscription {
	// Create subscription.
	subscription := &Subscription{close: close, rcs: s}
	s.handlers[subscription] = struct{}{}

	if len(s.handlers) != 1 {
		return subscription
	}
	s.hasHandlers.Store(true)

	// Subscribe.
	ctx := context.Background()
	_, err := internal.CallWithResponseError(ctx, s.service.Subscribe, nil)
	if err != nil {
		// If we failed to subscribe, the connection is probably failing, in which case we will
		// automatically reconnect / resubscribe soon.
		s.logger.WarnContext(ctx, "failed to subscribe", slog.String("service", s.service.ServiceFullName()), slog.String("error", err.Error()))
	}
	return subscription
}

// removeSubscription removes a subscription from the service. If this is the last subscription, it
// sends an Unsubscribe() request as well.
func (s *rcSubscription) removeSubscription(subscription *Subscription) {
	// Remove subscription.
	delete(s.handlers, subscription)

	if len(s.handlers) != 0 {
		return
	}
	s.hasHandlers.Store(false)

	// Unsubscribe.
	ctx := context.Background()
	_, err := internal.CallWithResponseError(ctx, s.service.Unsubscribe, nil)
	if err != nil {
		s.logger.WarnContext(ctx, "failed to unsubscribe", slog.String("service", s.service.ServiceFullName()), slog.String("error", err.Error()))
	}
}

// onConnected handles creating subscriptions when the service is connected.
func (s *rcSubscription) onConnected() {
	if !s.hasHandlers.Load() {
		return
	}

	// Subscribe.
	ctx := context.Background()
	_, err := internal.CallWithResponseError(ctx, s.service.Subscribe, nil)
	if err != nil {
		s.logger.ErrorContext(ctx, "failed to subscribe", slog.String("service", s.service.ServiceFullName()), slog.String("error", err.Error()))
	} else {
		s.logger.DebugContext(ctx, "automatically subscribed", slog.String("service", s.service.ServiceFullName()))
	}
}

// Creates a rpcClient by creating a RPC endpoint to the designated service.
func newClient[C rpcClient](
	ctx context.Context,
	logger *slog.Logger,
	options SdkOptions,
	si ServiceInfo,
	createClient func(*rpc.Endpoint) C,
	handler rpc.Handler,
) (C, error) {
	onConnected := func() {}
	if options.OnConnected != nil {
		onConnected = func() { options.OnConnected(si) }
	}

	onDisconnected := func(error) {}
	if options.OnDisconnected != nil {
		onDisconnected = func(err error) { options.OnDisconnected(si, err) }
	}

	onError := func(error) {}
	if options.OnError != nil {
		onError = func(err error) { options.OnError(si, err) }
	}

	// Set up endpoint.
	endpoint, err := rpc.NewEndpoint(
		ctx,
		logger,
		si.Host,
		si.Port,
		onConnected,
		onDisconnected,
		onError,
	)
	if err != nil {
		return *new(C), err
	}
	endpoint.SetHandler(handler)

	return createClient(endpoint), nil
}

// newSubscribingClient creates a [rpc.Endpoint] connected to si, constructs a client using createClient, and
// sets up subscriptions and the given handler for that client.
func newSubscribingClient[S defaultSubscribable](
	ctx context.Context,
	logger *slog.Logger,
	options SdkOptions,
	si ServiceInfo,
	createClient func(*rpc.Endpoint) S,
	handler rpc.Handler,
) (S, *rcSubscription, error) {

	// Set up event handlers.
	subscription := &rcSubscription{
		logger:   logger,
		handlers: make(map[*Subscription]struct{}),
	}

	onConnected := func(ServiceInfo) {
		subscription.onConnected()
	}
	if options.OnConnected != nil {
		onConnected = func(ServiceInfo) {
			subscription.onConnected()
			options.OnConnected(si)
		}
	}
	options.OnConnected = onConnected

	client, err := newClient(ctx, logger, options, si, createClient, handler)

	if err != nil {
		return *new(S), subscription, err
	}

	// subscription only accesses its service when adding subscriptions, so it is okay to only
	// assign it now.
	subscription.service = client

	return client, subscription, nil
}
