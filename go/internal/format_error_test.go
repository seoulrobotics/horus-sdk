package internal

import (
	"testing"

	"github.com/seoulrobotics/horus-sdk/go/proto/logs/logs_pb"
	"github.com/seoulrobotics/horus-sdk/go/proto/rpc_pb"
)

func TestFormatResponseError(t *testing.T) {
	tests := []struct {
		name     string
		response *rpc_pb.DefaultSubscribeResponse
		expected string
	}{
		{
			name:     "NoError",
			response: &rpc_pb.DefaultSubscribeResponse{},
			expected: "",
		},
		{
			name: "Error",
			response: rpc_pb.DefaultSubscribeResponse_builder{
				ConnectionError: logs_pb.RpcConnectionError_builder{
					TargetService: "FooService",
					TargetUri:     "localhost:1234",
					Details:       "no good",
				}.Build(),
			}.Build(),
			expected: "Cannot initiate connection to FooService at localhost:1234: no good",
		},
	}

	for _, test := range tests {
		t.Run(test.name, func(t *testing.T) {
			got := FormatResponseError(test.response)
			err := ResponseError(test.response)

			if test.expected == "" {
				if got != "" {
					t.Errorf("Expected no error, got \"%v\"", got)
				}
				if err != nil {
					t.Errorf("Expected no error, got \"%v\"", err)
				}
			} else {
				if got != test.expected {
					t.Errorf("Expected error \"%v\", got \"%v\"", test.expected, got)
				}
				if err == nil {
					t.Errorf("Expected error, got none")
				}
			}
		})
	}
}
