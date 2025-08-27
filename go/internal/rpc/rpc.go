package rpc

import (
	"context"
	"errors"
	"fmt"
	"log/slog"
	"net"
	"sync/atomic"
	"time"

	"github.com/coder/websocket"
	"github.com/seoulrobotics/horus-sdk/go/proto/rpc/message_pb"
	"google.golang.org/protobuf/proto"
)

// RequestId is the unique identifier of a request.
type RequestId = uint32

const (
	minRequestId     RequestId = 1
	maxRequestId               = 32
	requestQueueSize           = maxRequestId - minRequestId + 1
)

// MethodId is the unique identifier of a method.
type MethodId = uint32

// ServiceId is the unique identifier of a service.
type ServiceId = uint32

// Message is a raw Protobuf message representing a request or response.
type Message = message_pb.RpcMessage

// Endpoint is a client-side endpoint used to send and receive RPC requests.
type Endpoint struct {
	url    string
	logger *slog.Logger

	// Stores [websocket.Conn] if connected, [error] if disconnected.
	wsOrError atomic.Pointer[any]

	handlers map[ServiceId]func(context.Context, *message_pb.RpcMessage) (*message_pb.RpcMessage, error)

	// Called on connection.
	connectedHandler func()
	// Called on disconnection. Error is nil if called in [Endpoint.Close].
	disconnectedHandler func(error)
	// Called on non-disconnection error.
	errorHandler func(error)

	// Queue of available non-zero request IDs, filled on initialization with a pool of available
	// request IDs, which are then removed and added as needed by [Endpoint.SendTwoWay].
	availableRequestIds chan RequestId
	// Channels used to receive responses to two-way RPCs. The channel at index i (e.g. 0)
	// corresponds to the response with ID i+1 (e.g. 1).
	//
	// Each channel is "obtained" when popping from [Endpoint.availableRequestIds], and made
	// "available" by adding to [Endpoint.availableRequestIds]. Nil may be sent to the channel when
	// a response cannot be received (e.g. on disconnection).
	responseChannels []chan *message_pb.RpcMessage
}

func newWebSocket(ctx context.Context, url string) (*websocket.Conn, error) {
	ws, _, err := websocket.Dial(ctx, url, &websocket.DialOptions{
		CompressionMode: websocket.CompressionDisabled,
	})
	if err != nil {
		return nil, err
	}
	ws.SetReadLimit(10 << 20) // 10 MB
	return ws, nil
}

// NewEndpoint creates a new endpoint which connects to host at port via a WebSocket connection,
// logging messages to the given (non-nil) logger.
func NewEndpoint(
	ctx context.Context,
	logger *slog.Logger,
	host string,
	port uint16,
	onConnected func(),
	onDisconnected func(error),
	onError func(error),
) (*Endpoint, error) {
	url := fmt.Sprintf("ws://%s:%v", host, port)
	ws, err := newWebSocket(ctx, url)
	var wsOrError any

	if err == nil {
		wsOrError = ws
		onConnected()
	} else {
		var netError *net.OpError
		if !errors.As(err, &netError) {
			// Connection errors will be recovered from in the background, so we only return an
			// error if it is not a network error.
			return nil, err
		}
		wsOrError = err
		onError(err)
	}

	e := &Endpoint{
		url:    url,
		logger: logger,

		wsOrError: atomic.Pointer[any]{},

		handlers: make(map[ServiceId]func(context.Context, *message_pb.RpcMessage) (*message_pb.RpcMessage, error)),

		connectedHandler:    onConnected,
		disconnectedHandler: onDisconnected,
		errorHandler:        onError,

		availableRequestIds: make(chan RequestId, requestQueueSize),
		responseChannels:    make([]chan *message_pb.RpcMessage, requestQueueSize),
	}
	e.wsOrError.Store(&wsOrError)

	// Fill available request IDs and create response channels.
	for i := minRequestId; i <= maxRequestId; i++ {
		e.availableRequestIds <- i
		// Note: we must shift the index here since we don't use request ID 0.
		e.responseChannels[i-minRequestId] = make(chan *message_pb.RpcMessage)
	}

	go e.runReceiveLoop(context.Background())

	return e, nil
}

// Close closes the endpoint.
func (e *Endpoint) Close() error {
	var closeErr any = &websocket.CloseError{Code: websocket.StatusNormalClosure}
	ws := *e.wsOrError.Swap(&closeErr)

	e.cancelPending()

	if ws, ok := ws.(*websocket.Conn); ok {
		// We were still connected.
		e.disconnectedHandler(nil)

		return ws.Close(websocket.StatusNormalClosure, "")
	}
	return nil
}

// SetHandler sets the handler for a service.
func (e *Endpoint) SetHandler(handler Handler) {
	e.handlers[handler.ServiceId()] = func(ctx context.Context, request *message_pb.RpcMessage) (*message_pb.RpcMessage, error) {
		response, err := handler.Handle(ctx, request.GetMethodId(), request.GetMessageBytes())
		if err != nil {
			return nil, err
		}

		if response == nil {
			// No response needed.
			return nil, nil
		}

		// Serialize response.
		responseBytes, err := proto.Marshal(response)
		if err != nil {
			return nil, err
		}

		responseMessage := message_pb.RpcMessage_builder{
			Version:      message_pb.RpcMessage_VERSION_ONE,
			RequestId:    request.GetRequestId(),
			MessageBytes: responseBytes,
		}.Build()
		return responseMessage, nil
	}
}

// RemoveHandler removes a handler added with [Endpoint.SetHandler].
func (e *Endpoint) RemoveHandler(service ServiceId) {
	delete(e.handlers, service)
}

// SendOneWay sends a one-way request to the given service/method pair.
//
// This function should not be called directly; it is intended to be used by generated code.
func (e *Endpoint) SendOneWay(ctx context.Context, service ServiceId, method MethodId, request proto.Message) error {
	ws, err := e.getWebSocket()
	if err != nil {
		return fmt.Errorf("cannot send request to disconnected endpoint: %v", err)
	}

	// Serialize request.
	requestBytes, err := proto.Marshal(request)
	if err != nil {
		return fmt.Errorf("failed to serialize request: %v", err)
	}

	// Serialize request message.
	messageBytes, err := proto.Marshal(message_pb.RpcMessage_builder{
		Version:      message_pb.RpcMessage_VERSION_ONE,
		ServiceId:    service,
		MethodId:     method,
		MessageBytes: requestBytes,
	}.Build())
	if err != nil {
		return fmt.Errorf("failed to serialize request message: %v", err)
	}

	// Send message.
	err = ws.Write(ctx, websocket.MessageBinary, messageBytes)
	if err != nil {
		return fmt.Errorf("failed to send message: %v", err)
	}

	return nil
}

// SendOneWay sends a two-way request to the given service/method pair, and awaits a response.
//
// This function should not be called directly; it is intended to be used by generated code.
func (e *Endpoint) SendTwoWay(ctx context.Context, service ServiceId, method MethodId, request proto.Message, response proto.Message) error {
	// Serialize request.
	requestBytes, err := proto.Marshal(request)
	if err != nil {
		return fmt.Errorf("failed to serialize request: %v", err)
	}

	// Make sure we're connected.
	ws, err := e.getWebSocket()
	if err != nil {
		return fmt.Errorf("cannot send request to disconnected endpoint: %v", err)
	}

	// Get request ID and unbuffered channel for response.
	reqId := <-e.availableRequestIds
	responseChannel := e.responseChannels[reqId-minRequestId]
	defer func() {
		// Make sure to return the request ID to the pool when we're done here.
		e.availableRequestIds <- reqId
	}()

	// Serialize request message.
	messageBytes, err := proto.Marshal(message_pb.RpcMessage_builder{
		Version:      message_pb.RpcMessage_VERSION_ONE,
		ServiceId:    service,
		MethodId:     method,
		RequestId:    reqId,
		MessageBytes: requestBytes,
	}.Build())
	if err != nil {
		return fmt.Errorf("failed to serialize request message: %v", err)
	}

	// Send message.
	err = ws.Write(ctx, websocket.MessageBinary, messageBytes)
	if err != nil {
		return fmt.Errorf("failed to send message: %v", err)
	}

	// Wait for response. The channel is closed on disconnection, and receives nil from cancelPending().
	responseMessage, ok := <-responseChannel
	if !ok || responseMessage == nil {
		return fmt.Errorf("connection closed")
	}

	// Deserialize and validate response.
	if responseMessage.GetError() != "" {
		return fmt.Errorf("remote error: %v", responseMessage.GetError())
	}

	err = proto.Unmarshal(responseMessage.GetMessageBytes(), response)
	if err != nil {
		return fmt.Errorf("failed to deserialize response: %v", err)
	}

	return nil
}

// runReceiveLoop runs the receive loop, handling incoming messages and logging errors via [Endpoint.logger].
func (e *Endpoint) runReceiveLoop(ctx context.Context) {
	for {
		e.cancelPending()

		// Connect / reconnect.
		sleepTime := 50 * time.Millisecond
		maxSleepTime := 5 * time.Second

		ws, err := e.getWebSocket()

		for ws == nil {
			if websocket.CloseStatus(err) == websocket.StatusNormalClosure {
				// We closed the connection.
				return
			}

			ws, err = newWebSocket(ctx, e.url)
			if err == nil {
				e.setWebSocket(ws)
				go e.connectedHandler()

				break
			}
			go e.errorHandler(err)

			e.logger.InfoContext(ctx, "failed to connect to endpoint", e.endpointAttr(), slog.Duration("retryIn", sleepTime))

			time.Sleep(sleepTime)

			if sleepTime < maxSleepTime {
				sleepTime *= 2
			}
		}

		// Process messages.
		for {
			// Read message.
			typ, bytes, err := ws.Read(ctx)

			// Handle errors.
			if err != nil {
				if websocket.CloseStatus(err) == websocket.StatusNormalClosure {
					// We closed the connection.
					return
				}
				e.setWebSocketError(err)
				go e.disconnectedHandler(err)
				ws.CloseNow()

				e.logger.WarnContext(ctx, "WebSocket read failed", e.endpointAttr(), slog.String("error", err.Error()))

				// Reconnect.
				break
			}

			if typ != websocket.MessageBinary {
				e.logger.WarnContext(ctx, "cannot process received text message", e.endpointAttr())
				continue
			}

			// Handle message.
			e.handleMessage(ctx, ws, bytes)
		}
	}
}

// handleMessage handles a received message, processing errors.
func (e *Endpoint) handleMessage(ctx context.Context, ws *websocket.Conn, bytes []byte) {
	// Parse message.
	message := message_pb.RpcMessage{}
	err := proto.UnmarshalOptions{DiscardUnknown: true}.Unmarshal(bytes, &message)

	if err != nil {
		e.logger.WarnContext(ctx, "cannot deserialize receive RpcMessage", e.endpointAttr(), slog.String("error", err.Error()))
		return
	}

	if message.GetCancel() {
		// Ignore cancellations.
		return
	}

	if message.GetMethodId() == 0 {
		// Handle response.
		reqId := message.GetRequestId()
		reqIndex := reqId - 1

		if reqIndex >= RequestId(len(e.responseChannels)) {
			e.logger.WarnContext(ctx, "unknown request response received", e.endpointAttr(), slog.Uint64("requestId", uint64(reqId)))
			return
		}

		// Resume SendTwoWay() by sending to its channel.
		e.responseChannels[reqIndex] <- &message

		return
	}

	// Handle request.
	handler, ok := e.handlers[message.GetServiceId()]

	if !ok {
		e.logger.WarnContext(ctx, "cannot process request", e.endpointAttr(), slog.Uint64("serviceId", uint64(message.GetServiceId())))
		return
	}

	responseMessage, err := handler(ctx, &message)
	if err != nil {
		if message.GetRequestId() == 0 || message.GetMethodId() == 0 {
			// Cannot respond with error.
			e.logger.WarnContext(ctx, "service handler failed", e.endpointAttr(), slog.Uint64("serviceId", uint64(message.GetServiceId())), slog.String("error", err.Error()))
			return
		}
		responseMessage = message_pb.RpcMessage_builder{
			Version:   message_pb.RpcMessage_VERSION_ONE,
			RequestId: message.GetRequestId(),
			Error:     err.Error(),
		}.Build()
	}
	if responseMessage == nil {
		return
	}

	// Respond.
	responseBytes, err := proto.Marshal(responseMessage)
	if err != nil {
		e.logger.WarnContext(ctx, "failed to serialize response message", e.endpointAttr(), slog.String("error", err.Error()))
		return
	}

	err = ws.Write(ctx, websocket.MessageBinary, responseBytes)
	if err != nil {
		e.logger.WarnContext(ctx, "failed to send response message", e.endpointAttr(), slog.String("error", err.Error()))
		return
	}
}

// getWebSocket atomically returns the current WebSocket connection if connected, and the current
// error otherwise.
func (e *Endpoint) getWebSocket() (*websocket.Conn, error) {
	value := *e.wsOrError.Load()
	if err, ok := value.(error); ok {
		return nil, err
	}
	return value.(*websocket.Conn), nil
}

// setWebSocket atomically sets the current WebSocket connection if connected.
func (e *Endpoint) setWebSocket(ws *websocket.Conn) {
	var value any = ws
	e.wsOrError.Store(&value)
}

// setWebSocket atomically sets the current WebSocket error if disconnected.
func (e *Endpoint) setWebSocketError(err error) {
	var value any = err
	e.wsOrError.Store(&value)
}

// cancelPending closes pending response channels.
func (e *Endpoint) cancelPending() {
	for _, request := range e.responseChannels {
		select {
		case request <- nil:
			// SendTwoWay() was waiting, and will report an error.
		default:
			// No one is waiting.
		}
	}
}

// endpointAttr returns an attribute for logging the endpoint.
func (e *Endpoint) endpointAttr() slog.Attr {
	return slog.String("endpoint", e.url)
}

// Handler defines how to handle a request send to a specific service.
type Handler interface {
	ServiceId() ServiceId
	Handle(ctx context.Context, method MethodId, bytes []byte) (proto.Message, error)
}

// NewHandler creates a [Handler] from a [ServiceId] and function pair. handle cannot be nil.
func NewHandler(
	service ServiceId,
	handle func(ctx context.Context, method MethodId, bytes []byte) (proto.Message, error),
) Handler {
	if service == 0 {
		panic("service must not be 0")
	}
	if handle == nil {
		panic("handle must not be nil")
	}
	return &handlerFromFunction{service: service, handle: handle}
}

type handlerFromFunction struct {
	service ServiceId
	handle  func(ctx context.Context, method MethodId, bytes []byte) (proto.Message, error)
}

func (h handlerFromFunction) ServiceId() ServiceId {
	return h.service
}

func (h handlerFromFunction) Handle(ctx context.Context, method MethodId, bytes []byte) (proto.Message, error) {
	return h.handle(ctx, method, bytes)
}
