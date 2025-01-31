// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.36.1
// 	protoc        v5.28.3
// source: horus/pb/config/metadata.proto

package metadata_pb

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

// A vector with 2 `float` fields.
type Vector2F struct {
	state         protoimpl.MessageState `protogen:"opaque.v1"`
	xxx_hidden_X  float32                `protobuf:"fixed32,1,opt,name=x,proto3" json:"x,omitempty"`
	xxx_hidden_Y  float32                `protobuf:"fixed32,2,opt,name=y,proto3" json:"y,omitempty"`
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *Vector2F) Reset() {
	*x = Vector2F{}
	mi := &file_horus_pb_config_metadata_proto_msgTypes[0]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *Vector2F) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Vector2F) ProtoMessage() {}

func (x *Vector2F) ProtoReflect() protoreflect.Message {
	mi := &file_horus_pb_config_metadata_proto_msgTypes[0]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

func (x *Vector2F) GetX() float32 {
	if x != nil {
		return x.xxx_hidden_X
	}
	return 0
}

func (x *Vector2F) GetY() float32 {
	if x != nil {
		return x.xxx_hidden_Y
	}
	return 0
}

func (x *Vector2F) SetX(v float32) {
	x.xxx_hidden_X = v
}

func (x *Vector2F) SetY(v float32) {
	x.xxx_hidden_Y = v
}

type Vector2F_builder struct {
	_ [0]func() // Prevents comparability and use of unkeyed literals for the builder.

	X float32
	Y float32
}

func (b0 Vector2F_builder) Build() *Vector2F {
	m0 := &Vector2F{}
	b, x := &b0, m0
	_, _ = b, x
	x.xxx_hidden_X = b.X
	x.xxx_hidden_Y = b.Y
	return m0
}

// A vector with 3 `float` fields.
type Vector3F struct {
	state         protoimpl.MessageState `protogen:"opaque.v1"`
	xxx_hidden_X  float32                `protobuf:"fixed32,1,opt,name=x,proto3" json:"x,omitempty"`
	xxx_hidden_Y  float32                `protobuf:"fixed32,2,opt,name=y,proto3" json:"y,omitempty"`
	xxx_hidden_Z  float32                `protobuf:"fixed32,3,opt,name=z,proto3" json:"z,omitempty"`
	unknownFields protoimpl.UnknownFields
	sizeCache     protoimpl.SizeCache
}

func (x *Vector3F) Reset() {
	*x = Vector3F{}
	mi := &file_horus_pb_config_metadata_proto_msgTypes[1]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *Vector3F) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Vector3F) ProtoMessage() {}

func (x *Vector3F) ProtoReflect() protoreflect.Message {
	mi := &file_horus_pb_config_metadata_proto_msgTypes[1]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

func (x *Vector3F) GetX() float32 {
	if x != nil {
		return x.xxx_hidden_X
	}
	return 0
}

func (x *Vector3F) GetY() float32 {
	if x != nil {
		return x.xxx_hidden_Y
	}
	return 0
}

func (x *Vector3F) GetZ() float32 {
	if x != nil {
		return x.xxx_hidden_Z
	}
	return 0
}

func (x *Vector3F) SetX(v float32) {
	x.xxx_hidden_X = v
}

func (x *Vector3F) SetY(v float32) {
	x.xxx_hidden_Y = v
}

func (x *Vector3F) SetZ(v float32) {
	x.xxx_hidden_Z = v
}

type Vector3F_builder struct {
	_ [0]func() // Prevents comparability and use of unkeyed literals for the builder.

	X float32
	Y float32
	Z float32
}

func (b0 Vector3F_builder) Build() *Vector3F {
	m0 := &Vector3F{}
	b, x := &b0, m0
	_, _ = b, x
	x.xxx_hidden_X = b.X
	x.xxx_hidden_Y = b.Y
	x.xxx_hidden_Z = b.Z
	return m0
}

// A vector with two `double` fields representing a range (where the `start`
// must be lower than or equal to the `end`).
type Range struct {
	state            protoimpl.MessageState `protogen:"opaque.v1"`
	xxx_hidden_Start float64                `protobuf:"fixed64,1,opt,name=start,proto3" json:"start,omitempty"`
	xxx_hidden_End   float64                `protobuf:"fixed64,2,opt,name=end,proto3" json:"end,omitempty"`
	unknownFields    protoimpl.UnknownFields
	sizeCache        protoimpl.SizeCache
}

func (x *Range) Reset() {
	*x = Range{}
	mi := &file_horus_pb_config_metadata_proto_msgTypes[2]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *Range) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Range) ProtoMessage() {}

func (x *Range) ProtoReflect() protoreflect.Message {
	mi := &file_horus_pb_config_metadata_proto_msgTypes[2]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

func (x *Range) GetStart() float64 {
	if x != nil {
		return x.xxx_hidden_Start
	}
	return 0
}

func (x *Range) GetEnd() float64 {
	if x != nil {
		return x.xxx_hidden_End
	}
	return 0
}

func (x *Range) SetStart(v float64) {
	x.xxx_hidden_Start = v
}

func (x *Range) SetEnd(v float64) {
	x.xxx_hidden_End = v
}

type Range_builder struct {
	_ [0]func() // Prevents comparability and use of unkeyed literals for the builder.

	Start float64
	End   float64
}

func (b0 Range_builder) Build() *Range {
	m0 := &Range{}
	b, x := &b0, m0
	_, _ = b, x
	x.xxx_hidden_Start = b.Start
	x.xxx_hidden_End = b.End
	return m0
}

// Contains a list of `uint32` values. Used when a list of repeated `uint32`
// values should be `optional`.
type UInt32List struct {
	state             protoimpl.MessageState `protogen:"opaque.v1"`
	xxx_hidden_Values []uint32               `protobuf:"varint,1,rep,packed,name=values,proto3" json:"values,omitempty"`
	unknownFields     protoimpl.UnknownFields
	sizeCache         protoimpl.SizeCache
}

func (x *UInt32List) Reset() {
	*x = UInt32List{}
	mi := &file_horus_pb_config_metadata_proto_msgTypes[3]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *UInt32List) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*UInt32List) ProtoMessage() {}

func (x *UInt32List) ProtoReflect() protoreflect.Message {
	mi := &file_horus_pb_config_metadata_proto_msgTypes[3]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

func (x *UInt32List) GetValues() []uint32 {
	if x != nil {
		return x.xxx_hidden_Values
	}
	return nil
}

func (x *UInt32List) SetValues(v []uint32) {
	x.xxx_hidden_Values = v
}

type UInt32List_builder struct {
	_ [0]func() // Prevents comparability and use of unkeyed literals for the builder.

	Values []uint32
}

func (b0 UInt32List_builder) Build() *UInt32List {
	m0 := &UInt32List{}
	b, x := &b0, m0
	_, _ = b, x
	x.xxx_hidden_Values = b.Values
	return m0
}

// A timestamp.
//
// We do not use google.protobuf.Timestamp as it is defined in libprotobuf, but
// not libprotobuf-lite, and generating metadata for such messages would lead to
// binary conflicts.
type Timestamp struct {
	state              protoimpl.MessageState `protogen:"opaque.v1"`
	xxx_hidden_Seconds int64                  `protobuf:"varint,1,opt,name=seconds,proto3" json:"seconds,omitempty"`
	xxx_hidden_Nanos   int32                  `protobuf:"varint,2,opt,name=nanos,proto3" json:"nanos,omitempty"`
	unknownFields      protoimpl.UnknownFields
	sizeCache          protoimpl.SizeCache
}

func (x *Timestamp) Reset() {
	*x = Timestamp{}
	mi := &file_horus_pb_config_metadata_proto_msgTypes[4]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *Timestamp) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Timestamp) ProtoMessage() {}

func (x *Timestamp) ProtoReflect() protoreflect.Message {
	mi := &file_horus_pb_config_metadata_proto_msgTypes[4]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

func (x *Timestamp) GetSeconds() int64 {
	if x != nil {
		return x.xxx_hidden_Seconds
	}
	return 0
}

func (x *Timestamp) GetNanos() int32 {
	if x != nil {
		return x.xxx_hidden_Nanos
	}
	return 0
}

func (x *Timestamp) SetSeconds(v int64) {
	x.xxx_hidden_Seconds = v
}

func (x *Timestamp) SetNanos(v int32) {
	x.xxx_hidden_Nanos = v
}

type Timestamp_builder struct {
	_ [0]func() // Prevents comparability and use of unkeyed literals for the builder.

	// Number of seconds between the represented time and the epoch
	// (1970-01-01 00:00) in UTC.
	Seconds int64
	// Non-negative fractions of a second at nanosecond resolution.
	Nanos int32
}

func (b0 Timestamp_builder) Build() *Timestamp {
	m0 := &Timestamp{}
	b, x := &b0, m0
	_, _ = b, x
	x.xxx_hidden_Seconds = b.Seconds
	x.xxx_hidden_Nanos = b.Nanos
	return m0
}

// A duration.
//
// We do not use google.protobuf.Duration as it is defined in libprotobuf, but
// not libprotobuf-lite, and generating metadata for such messages would lead to
// binary conflicts.
type Duration struct {
	state              protoimpl.MessageState `protogen:"opaque.v1"`
	xxx_hidden_Seconds int64                  `protobuf:"varint,1,opt,name=seconds,proto3" json:"seconds,omitempty"`
	xxx_hidden_Nanos   int32                  `protobuf:"varint,2,opt,name=nanos,proto3" json:"nanos,omitempty"`
	unknownFields      protoimpl.UnknownFields
	sizeCache          protoimpl.SizeCache
}

func (x *Duration) Reset() {
	*x = Duration{}
	mi := &file_horus_pb_config_metadata_proto_msgTypes[5]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *Duration) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*Duration) ProtoMessage() {}

func (x *Duration) ProtoReflect() protoreflect.Message {
	mi := &file_horus_pb_config_metadata_proto_msgTypes[5]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

func (x *Duration) GetSeconds() int64 {
	if x != nil {
		return x.xxx_hidden_Seconds
	}
	return 0
}

func (x *Duration) GetNanos() int32 {
	if x != nil {
		return x.xxx_hidden_Nanos
	}
	return 0
}

func (x *Duration) SetSeconds(v int64) {
	x.xxx_hidden_Seconds = v
}

func (x *Duration) SetNanos(v int32) {
	x.xxx_hidden_Nanos = v
}

type Duration_builder struct {
	_ [0]func() // Prevents comparability and use of unkeyed literals for the builder.

	Seconds int64
	Nanos   int32
}

func (b0 Duration_builder) Build() *Duration {
	m0 := &Duration{}
	b, x := &b0, m0
	_, _ = b, x
	x.xxx_hidden_Seconds = b.Seconds
	x.xxx_hidden_Nanos = b.Nanos
	return m0
}

// Matrix types
// ------------
// TODO(HRS-495) Reuse Matrixf for representing affine transform.
type AffineTransform3F struct {
	state           protoimpl.MessageState `protogen:"opaque.v1"`
	xxx_hidden_Data []float32              `protobuf:"fixed32,1,rep,packed,name=data,proto3" json:"data,omitempty"`
	unknownFields   protoimpl.UnknownFields
	sizeCache       protoimpl.SizeCache
}

func (x *AffineTransform3F) Reset() {
	*x = AffineTransform3F{}
	mi := &file_horus_pb_config_metadata_proto_msgTypes[6]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *AffineTransform3F) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*AffineTransform3F) ProtoMessage() {}

func (x *AffineTransform3F) ProtoReflect() protoreflect.Message {
	mi := &file_horus_pb_config_metadata_proto_msgTypes[6]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

func (x *AffineTransform3F) GetData() []float32 {
	if x != nil {
		return x.xxx_hidden_Data
	}
	return nil
}

func (x *AffineTransform3F) SetData(v []float32) {
	x.xxx_hidden_Data = v
}

type AffineTransform3F_builder struct {
	_ [0]func() // Prevents comparability and use of unkeyed literals for the builder.

	Data []float32
}

func (b0 AffineTransform3F_builder) Build() *AffineTransform3F {
	m0 := &AffineTransform3F{}
	b, x := &b0, m0
	_, _ = b, x
	x.xxx_hidden_Data = b.Data
	return m0
}

var File_horus_pb_config_metadata_proto protoreflect.FileDescriptor

var file_horus_pb_config_metadata_proto_rawDesc = []byte{
	0x0a, 0x1e, 0x68, 0x6f, 0x72, 0x75, 0x73, 0x2f, 0x70, 0x62, 0x2f, 0x63, 0x6f, 0x6e, 0x66, 0x69,
	0x67, 0x2f, 0x6d, 0x65, 0x74, 0x61, 0x64, 0x61, 0x74, 0x61, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f,
	0x12, 0x08, 0x68, 0x6f, 0x72, 0x75, 0x73, 0x2e, 0x70, 0x62, 0x22, 0x26, 0x0a, 0x08, 0x56, 0x65,
	0x63, 0x74, 0x6f, 0x72, 0x32, 0x66, 0x12, 0x0c, 0x0a, 0x01, 0x78, 0x18, 0x01, 0x20, 0x01, 0x28,
	0x02, 0x52, 0x01, 0x78, 0x12, 0x0c, 0x0a, 0x01, 0x79, 0x18, 0x02, 0x20, 0x01, 0x28, 0x02, 0x52,
	0x01, 0x79, 0x22, 0x34, 0x0a, 0x08, 0x56, 0x65, 0x63, 0x74, 0x6f, 0x72, 0x33, 0x66, 0x12, 0x0c,
	0x0a, 0x01, 0x78, 0x18, 0x01, 0x20, 0x01, 0x28, 0x02, 0x52, 0x01, 0x78, 0x12, 0x0c, 0x0a, 0x01,
	0x79, 0x18, 0x02, 0x20, 0x01, 0x28, 0x02, 0x52, 0x01, 0x79, 0x12, 0x0c, 0x0a, 0x01, 0x7a, 0x18,
	0x03, 0x20, 0x01, 0x28, 0x02, 0x52, 0x01, 0x7a, 0x22, 0x2f, 0x0a, 0x05, 0x52, 0x61, 0x6e, 0x67,
	0x65, 0x12, 0x14, 0x0a, 0x05, 0x73, 0x74, 0x61, 0x72, 0x74, 0x18, 0x01, 0x20, 0x01, 0x28, 0x01,
	0x52, 0x05, 0x73, 0x74, 0x61, 0x72, 0x74, 0x12, 0x10, 0x0a, 0x03, 0x65, 0x6e, 0x64, 0x18, 0x02,
	0x20, 0x01, 0x28, 0x01, 0x52, 0x03, 0x65, 0x6e, 0x64, 0x22, 0x24, 0x0a, 0x0a, 0x55, 0x49, 0x6e,
	0x74, 0x33, 0x32, 0x4c, 0x69, 0x73, 0x74, 0x12, 0x16, 0x0a, 0x06, 0x76, 0x61, 0x6c, 0x75, 0x65,
	0x73, 0x18, 0x01, 0x20, 0x03, 0x28, 0x0d, 0x52, 0x06, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x73, 0x22,
	0x3b, 0x0a, 0x09, 0x54, 0x69, 0x6d, 0x65, 0x73, 0x74, 0x61, 0x6d, 0x70, 0x12, 0x18, 0x0a, 0x07,
	0x73, 0x65, 0x63, 0x6f, 0x6e, 0x64, 0x73, 0x18, 0x01, 0x20, 0x01, 0x28, 0x03, 0x52, 0x07, 0x73,
	0x65, 0x63, 0x6f, 0x6e, 0x64, 0x73, 0x12, 0x14, 0x0a, 0x05, 0x6e, 0x61, 0x6e, 0x6f, 0x73, 0x18,
	0x02, 0x20, 0x01, 0x28, 0x05, 0x52, 0x05, 0x6e, 0x61, 0x6e, 0x6f, 0x73, 0x22, 0x3a, 0x0a, 0x08,
	0x44, 0x75, 0x72, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x12, 0x18, 0x0a, 0x07, 0x73, 0x65, 0x63, 0x6f,
	0x6e, 0x64, 0x73, 0x18, 0x01, 0x20, 0x01, 0x28, 0x03, 0x52, 0x07, 0x73, 0x65, 0x63, 0x6f, 0x6e,
	0x64, 0x73, 0x12, 0x14, 0x0a, 0x05, 0x6e, 0x61, 0x6e, 0x6f, 0x73, 0x18, 0x02, 0x20, 0x01, 0x28,
	0x05, 0x52, 0x05, 0x6e, 0x61, 0x6e, 0x6f, 0x73, 0x22, 0x27, 0x0a, 0x11, 0x41, 0x66, 0x66, 0x69,
	0x6e, 0x65, 0x54, 0x72, 0x61, 0x6e, 0x73, 0x66, 0x6f, 0x72, 0x6d, 0x33, 0x66, 0x12, 0x12, 0x0a,
	0x04, 0x64, 0x61, 0x74, 0x61, 0x18, 0x01, 0x20, 0x03, 0x28, 0x02, 0x52, 0x04, 0x64, 0x61, 0x74,
	0x61, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var file_horus_pb_config_metadata_proto_msgTypes = make([]protoimpl.MessageInfo, 7)
var file_horus_pb_config_metadata_proto_goTypes = []any{
	(*Vector2F)(nil),          // 0: horus.pb.Vector2f
	(*Vector3F)(nil),          // 1: horus.pb.Vector3f
	(*Range)(nil),             // 2: horus.pb.Range
	(*UInt32List)(nil),        // 3: horus.pb.UInt32List
	(*Timestamp)(nil),         // 4: horus.pb.Timestamp
	(*Duration)(nil),          // 5: horus.pb.Duration
	(*AffineTransform3F)(nil), // 6: horus.pb.AffineTransform3f
}
var file_horus_pb_config_metadata_proto_depIdxs = []int32{
	0, // [0:0] is the sub-list for method output_type
	0, // [0:0] is the sub-list for method input_type
	0, // [0:0] is the sub-list for extension type_name
	0, // [0:0] is the sub-list for extension extendee
	0, // [0:0] is the sub-list for field type_name
}

func init() { file_horus_pb_config_metadata_proto_init() }
func file_horus_pb_config_metadata_proto_init() {
	if File_horus_pb_config_metadata_proto != nil {
		return
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_horus_pb_config_metadata_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   7,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_horus_pb_config_metadata_proto_goTypes,
		DependencyIndexes: file_horus_pb_config_metadata_proto_depIdxs,
		MessageInfos:      file_horus_pb_config_metadata_proto_msgTypes,
	}.Build()
	File_horus_pb_config_metadata_proto = out.File
	file_horus_pb_config_metadata_proto_rawDesc = nil
	file_horus_pb_config_metadata_proto_goTypes = nil
	file_horus_pb_config_metadata_proto_depIdxs = nil
}
