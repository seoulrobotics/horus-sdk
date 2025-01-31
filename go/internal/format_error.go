package internal

import (
	"context"
	"fmt"

	"google.golang.org/protobuf/proto"
	"google.golang.org/protobuf/reflect/protoreflect"
)

// ResponseWithError is a Protobuf message which contains an error `oneof`.
type ResponseWithError[E ~int32] interface {
	proto.Message

	WhichError() E
}

// FormatResponseError formats the error message of a response.
func FormatResponseError[E ~int32](response ResponseWithError[E]) string {
	if response.WhichError() == 0 {
		return ""
	}

	oneofDescriptor := response.ProtoReflect().Descriptor().Oneofs().ByName("error")
	dataDescriptor := oneofDescriptor.Fields().ByNumber(protoreflect.FieldNumber(response.WhichError()))
	data := response.ProtoReflect().Get(dataDescriptor).Message()

	return FormatAnyLogMessage(data.Interface())
}

// ResponseError returns an error if the response contains an error.
func ResponseError[E ~int32](response ResponseWithError[E]) error {
	if response.WhichError() == 0 {
		return nil
	}
	return fmt.Errorf("%s yielded an error: %s", response.ProtoReflect().Descriptor().Name(), FormatResponseError(response))
}

// CallWithResponseError calls f with req, converting any `oneof` error to a Go error.
func CallWithResponseError[E ~int32, Req proto.Message, Res ResponseWithError[E]](ctx context.Context, f func(context.Context, Req) (Res, error), req Req) (Res, error) {
	response, err := f(ctx, req)
	if err != nil {
		return response, err
	}
	err = ResponseError(response)
	return response, err
}

// spell-checker:ignore protoreflect
