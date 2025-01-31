// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.36.1
// 	protoc        v5.28.3
// source: horus/pb/rpc/message.proto

package message_pb

import (
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

type RpcMessage_Version int32

const (
	RpcMessage_VERSION_UNSPECIFIED RpcMessage_Version = 0
	RpcMessage_VERSION_ONE         RpcMessage_Version = 1
)

// Enum value maps for RpcMessage_Version.
var (
	RpcMessage_Version_name = map[int32]string{
		0: "VERSION_UNSPECIFIED",
		1: "VERSION_ONE",
	}
	RpcMessage_Version_value = map[string]int32{
		"VERSION_UNSPECIFIED": 0,
		"VERSION_ONE":         1,
	}
)

func (x RpcMessage_Version) Enum() *RpcMessage_Version {
	p := new(RpcMessage_Version)
	*p = x
	return p
}

func (x RpcMessage_Version) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (RpcMessage_Version) Descriptor() protoreflect.EnumDescriptor {
	return file_horus_pb_rpc_message_proto_enumTypes[0].Descriptor()
}

func (RpcMessage_Version) Type() protoreflect.EnumType {
	return &file_horus_pb_rpc_message_proto_enumTypes[0]
}

func (x RpcMessage_Version) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// A message used to implement RPCs, sent from the client to the server or
// vice-versa.
type RpcMessage struct {
	state                   protoimpl.MessageState `protogen:"opaque.v1"`
	xxx_hidden_Version      RpcMessage_Version     `protobuf:"varint,1,opt,name=version,proto3,enum=horus.pb.RpcMessage_Version" json:"version,omitempty"`
	xxx_hidden_ServiceId    uint32                 `protobuf:"varint,2,opt,name=service_id,json=serviceId,proto3" json:"service_id,omitempty"`
	xxx_hidden_MessageBytes []byte                 `protobuf:"bytes,3,opt,name=message_bytes,json=messageBytes,proto3" json:"message_bytes,omitempty"`
	xxx_hidden_RequestId    uint32                 `protobuf:"varint,4,opt,name=request_id,json=requestId,proto3" json:"request_id,omitempty"`
	xxx_hidden_MethodId     uint32                 `protobuf:"varint,5,opt,name=method_id,json=methodId,proto3" json:"method_id,omitempty"`
	xxx_hidden_Error        string                 `protobuf:"bytes,6,opt,name=error,proto3" json:"error,omitempty"`
	xxx_hidden_Cancel       bool                   `protobuf:"varint,7,opt,name=cancel,proto3" json:"cancel,omitempty"`
	unknownFields           protoimpl.UnknownFields
	sizeCache               protoimpl.SizeCache
}

func (x *RpcMessage) Reset() {
	*x = RpcMessage{}
	mi := &file_horus_pb_rpc_message_proto_msgTypes[0]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *RpcMessage) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*RpcMessage) ProtoMessage() {}

func (x *RpcMessage) ProtoReflect() protoreflect.Message {
	mi := &file_horus_pb_rpc_message_proto_msgTypes[0]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

func (x *RpcMessage) GetVersion() RpcMessage_Version {
	if x != nil {
		return x.xxx_hidden_Version
	}
	return RpcMessage_VERSION_UNSPECIFIED
}

func (x *RpcMessage) GetServiceId() uint32 {
	if x != nil {
		return x.xxx_hidden_ServiceId
	}
	return 0
}

func (x *RpcMessage) GetMessageBytes() []byte {
	if x != nil {
		return x.xxx_hidden_MessageBytes
	}
	return nil
}

func (x *RpcMessage) GetRequestId() uint32 {
	if x != nil {
		return x.xxx_hidden_RequestId
	}
	return 0
}

func (x *RpcMessage) GetMethodId() uint32 {
	if x != nil {
		return x.xxx_hidden_MethodId
	}
	return 0
}

func (x *RpcMessage) GetError() string {
	if x != nil {
		return x.xxx_hidden_Error
	}
	return ""
}

func (x *RpcMessage) GetCancel() bool {
	if x != nil {
		return x.xxx_hidden_Cancel
	}
	return false
}

func (x *RpcMessage) SetVersion(v RpcMessage_Version) {
	x.xxx_hidden_Version = v
}

func (x *RpcMessage) SetServiceId(v uint32) {
	x.xxx_hidden_ServiceId = v
}

func (x *RpcMessage) SetMessageBytes(v []byte) {
	if v == nil {
		v = []byte{}
	}
	x.xxx_hidden_MessageBytes = v
}

func (x *RpcMessage) SetRequestId(v uint32) {
	x.xxx_hidden_RequestId = v
}

func (x *RpcMessage) SetMethodId(v uint32) {
	x.xxx_hidden_MethodId = v
}

func (x *RpcMessage) SetError(v string) {
	x.xxx_hidden_Error = v
}

func (x *RpcMessage) SetCancel(v bool) {
	x.xxx_hidden_Cancel = v
}

type RpcMessage_builder struct {
	_ [0]func() // Prevents comparability and use of unkeyed literals for the builder.

	// The version of the message.
	Version RpcMessage_Version
	// The ID of the service being talked to.
	ServiceId uint32
	// The bytes making up the request or response. The type is determined by the
	// receiver.
	MessageBytes []byte
	// The ID of the request (may also be set for responses to specify which
	// request is being replied to). Specify 0 to indicate this request expects no
	// response.
	RequestId uint32
	// The ID of the method being called. Only set in requests.
	MethodId uint32
	// An error encountered while processing a request which expects a response.
	// This field will be set when the handler of a request fails to provide a
	// response for any reason. Such errors are **internal errors**, and should
	// never occur. However, RPC endpoints need a mechanism to stop awaiting
	// pending requests in case of error, which this field provides.
	Error string
	// Whether this message is a cancellation request, in which case
	// `request_id` must be set and non-zero. The response to the relevant
	// request will be ignored, but **must nonetheless be sent to the client**.
	//
	// A response must still be provided following a cancellation request
	// because it may already be on its way to the client. If the client
	// immediately made the `request_id` available for new requests without
	// waiting for a response, it would be possible for the following to happen:
	//
	//  1. Alice sends a request Foo with ID #1 to Bob.
	//  2. Bob computes a response and enqueues it.
	//  3. Alice sends a cancellation request for #1, making #1 available for
	//     re-use.
	//  4. Alice sends a new request Bar with the same reused ID #1 to Bob.
	//  5. Bob responds to Foo.
	//  6. Alice receives a FooResponse while it expects a BarResponse.
	Cancel bool
}

func (b0 RpcMessage_builder) Build() *RpcMessage {
	m0 := &RpcMessage{}
	b, x := &b0, m0
	_, _ = b, x
	x.xxx_hidden_Version = b.Version
	x.xxx_hidden_ServiceId = b.ServiceId
	x.xxx_hidden_MessageBytes = b.MessageBytes
	x.xxx_hidden_RequestId = b.RequestId
	x.xxx_hidden_MethodId = b.MethodId
	x.xxx_hidden_Error = b.Error
	x.xxx_hidden_Cancel = b.Cancel
	return m0
}

var File_horus_pb_rpc_message_proto protoreflect.FileDescriptor

var file_horus_pb_rpc_message_proto_rawDesc = []byte{
	0x0a, 0x1a, 0x68, 0x6f, 0x72, 0x75, 0x73, 0x2f, 0x70, 0x62, 0x2f, 0x72, 0x70, 0x63, 0x2f, 0x6d,
	0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x08, 0x68, 0x6f,
	0x72, 0x75, 0x73, 0x2e, 0x70, 0x62, 0x22, 0xa7, 0x02, 0x0a, 0x0a, 0x52, 0x70, 0x63, 0x4d, 0x65,
	0x73, 0x73, 0x61, 0x67, 0x65, 0x12, 0x36, 0x0a, 0x07, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e,
	0x18, 0x01, 0x20, 0x01, 0x28, 0x0e, 0x32, 0x1c, 0x2e, 0x68, 0x6f, 0x72, 0x75, 0x73, 0x2e, 0x70,
	0x62, 0x2e, 0x52, 0x70, 0x63, 0x4d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x2e, 0x56, 0x65, 0x72,
	0x73, 0x69, 0x6f, 0x6e, 0x52, 0x07, 0x76, 0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x12, 0x1d, 0x0a,
	0x0a, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x5f, 0x69, 0x64, 0x18, 0x02, 0x20, 0x01, 0x28,
	0x0d, 0x52, 0x09, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x49, 0x64, 0x12, 0x23, 0x0a, 0x0d,
	0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x5f, 0x62, 0x79, 0x74, 0x65, 0x73, 0x18, 0x03, 0x20,
	0x01, 0x28, 0x0c, 0x52, 0x0c, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65, 0x42, 0x79, 0x74, 0x65,
	0x73, 0x12, 0x1d, 0x0a, 0x0a, 0x72, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x5f, 0x69, 0x64, 0x18,
	0x04, 0x20, 0x01, 0x28, 0x0d, 0x52, 0x09, 0x72, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x49, 0x64,
	0x12, 0x1b, 0x0a, 0x09, 0x6d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x5f, 0x69, 0x64, 0x18, 0x05, 0x20,
	0x01, 0x28, 0x0d, 0x52, 0x08, 0x6d, 0x65, 0x74, 0x68, 0x6f, 0x64, 0x49, 0x64, 0x12, 0x14, 0x0a,
	0x05, 0x65, 0x72, 0x72, 0x6f, 0x72, 0x18, 0x06, 0x20, 0x01, 0x28, 0x09, 0x52, 0x05, 0x65, 0x72,
	0x72, 0x6f, 0x72, 0x12, 0x16, 0x0a, 0x06, 0x63, 0x61, 0x6e, 0x63, 0x65, 0x6c, 0x18, 0x07, 0x20,
	0x01, 0x28, 0x08, 0x52, 0x06, 0x63, 0x61, 0x6e, 0x63, 0x65, 0x6c, 0x22, 0x33, 0x0a, 0x07, 0x56,
	0x65, 0x72, 0x73, 0x69, 0x6f, 0x6e, 0x12, 0x17, 0x0a, 0x13, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4f,
	0x4e, 0x5f, 0x55, 0x4e, 0x53, 0x50, 0x45, 0x43, 0x49, 0x46, 0x49, 0x45, 0x44, 0x10, 0x00, 0x12,
	0x0f, 0x0a, 0x0b, 0x56, 0x45, 0x52, 0x53, 0x49, 0x4f, 0x4e, 0x5f, 0x4f, 0x4e, 0x45, 0x10, 0x01,
	0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var file_horus_pb_rpc_message_proto_enumTypes = make([]protoimpl.EnumInfo, 1)
var file_horus_pb_rpc_message_proto_msgTypes = make([]protoimpl.MessageInfo, 1)
var file_horus_pb_rpc_message_proto_goTypes = []any{
	(RpcMessage_Version)(0), // 0: horus.pb.RpcMessage.Version
	(*RpcMessage)(nil),      // 1: horus.pb.RpcMessage
}
var file_horus_pb_rpc_message_proto_depIdxs = []int32{
	0, // 0: horus.pb.RpcMessage.version:type_name -> horus.pb.RpcMessage.Version
	1, // [1:1] is the sub-list for method output_type
	1, // [1:1] is the sub-list for method input_type
	1, // [1:1] is the sub-list for extension type_name
	1, // [1:1] is the sub-list for extension extendee
	0, // [0:1] is the sub-list for field type_name
}

func init() { file_horus_pb_rpc_message_proto_init() }
func file_horus_pb_rpc_message_proto_init() {
	if File_horus_pb_rpc_message_proto != nil {
		return
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_horus_pb_rpc_message_proto_rawDesc,
			NumEnums:      1,
			NumMessages:   1,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_horus_pb_rpc_message_proto_goTypes,
		DependencyIndexes: file_horus_pb_rpc_message_proto_depIdxs,
		EnumInfos:         file_horus_pb_rpc_message_proto_enumTypes,
		MessageInfos:      file_horus_pb_rpc_message_proto_msgTypes,
	}.Build()
	File_horus_pb_rpc_message_proto = out.File
	file_horus_pb_rpc_message_proto_rawDesc = nil
	file_horus_pb_rpc_message_proto_goTypes = nil
	file_horus_pb_rpc_message_proto_depIdxs = nil
}
