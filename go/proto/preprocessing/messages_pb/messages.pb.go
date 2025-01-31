// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.36.1
// 	protoc        v5.28.3
// source: horus/pb/preprocessing/messages.proto

package messages_pb

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

// / The classification of the occupancy grid cell.
// / @note The maximum number of classification enum values supported is 2^3=8.
// / @see `OccupancyGrid` message and `occupancy_map_constants` for more details.
type OccupancyClassification int32

const (
	OccupancyClassification_OCCUPANCYCLASSIFICATION_UNSPECIFIED OccupancyClassification = 0
	OccupancyClassification_FREE                                OccupancyClassification = 1
	OccupancyClassification_OCCLUDED                            OccupancyClassification = 2
	OccupancyClassification_STATIONARY_OCCUPIED                 OccupancyClassification = 3
)

// Enum value maps for OccupancyClassification.
var (
	OccupancyClassification_name = map[int32]string{
		0: "OCCUPANCYCLASSIFICATION_UNSPECIFIED",
		1: "FREE",
		2: "OCCLUDED",
		3: "STATIONARY_OCCUPIED",
	}
	OccupancyClassification_value = map[string]int32{
		"OCCUPANCYCLASSIFICATION_UNSPECIFIED": 0,
		"FREE":                                1,
		"OCCLUDED":                            2,
		"STATIONARY_OCCUPIED":                 3,
	}
)

func (x OccupancyClassification) Enum() *OccupancyClassification {
	p := new(OccupancyClassification)
	*p = x
	return p
}

func (x OccupancyClassification) String() string {
	return protoimpl.X.EnumStringOf(x.Descriptor(), protoreflect.EnumNumber(x))
}

func (OccupancyClassification) Descriptor() protoreflect.EnumDescriptor {
	return file_horus_pb_preprocessing_messages_proto_enumTypes[0].Descriptor()
}

func (OccupancyClassification) Type() protoreflect.EnumType {
	return &file_horus_pb_preprocessing_messages_proto_enumTypes[0]
}

func (x OccupancyClassification) Number() protoreflect.EnumNumber {
	return protoreflect.EnumNumber(x)
}

// / The data of the occupancy map.
// / The data is formated with a run-length encoding (RLE) in row-major order.
// / Consecutive cells of the same classification are compressed into a single
// / uint32.
type OccupancyGrid struct {
	state            protoimpl.MessageState `protogen:"opaque.v1"`
	xxx_hidden_Rows  uint32                 `protobuf:"varint,1,opt,name=rows,proto3" json:"rows,omitempty"`
	xxx_hidden_Cols  uint32                 `protobuf:"varint,2,opt,name=cols,proto3" json:"cols,omitempty"`
	xxx_hidden_Cells []uint32               `protobuf:"varint,3,rep,packed,name=cells,proto3" json:"cells,omitempty"`
	unknownFields    protoimpl.UnknownFields
	sizeCache        protoimpl.SizeCache
}

func (x *OccupancyGrid) Reset() {
	*x = OccupancyGrid{}
	mi := &file_horus_pb_preprocessing_messages_proto_msgTypes[0]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *OccupancyGrid) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*OccupancyGrid) ProtoMessage() {}

func (x *OccupancyGrid) ProtoReflect() protoreflect.Message {
	mi := &file_horus_pb_preprocessing_messages_proto_msgTypes[0]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

func (x *OccupancyGrid) GetRows() uint32 {
	if x != nil {
		return x.xxx_hidden_Rows
	}
	return 0
}

func (x *OccupancyGrid) GetCols() uint32 {
	if x != nil {
		return x.xxx_hidden_Cols
	}
	return 0
}

func (x *OccupancyGrid) GetCells() []uint32 {
	if x != nil {
		return x.xxx_hidden_Cells
	}
	return nil
}

func (x *OccupancyGrid) SetRows(v uint32) {
	x.xxx_hidden_Rows = v
}

func (x *OccupancyGrid) SetCols(v uint32) {
	x.xxx_hidden_Cols = v
}

func (x *OccupancyGrid) SetCells(v []uint32) {
	x.xxx_hidden_Cells = v
}

type OccupancyGrid_builder struct {
	_ [0]func() // Prevents comparability and use of unkeyed literals for the builder.

	Rows uint32
	Cols uint32
	// Packed representation of cells: Encodes <value, length>. The three most
	// significant bits of the uint32 encodes the value of
	// `OccupancyClassification`. The remaining 29 bits encode the run length.
	// @note The maximum number of classification enum values supported is 2^3=8.
	// @see `occupancy_map_constants` for more details.
	Cells []uint32
}

func (b0 OccupancyGrid_builder) Build() *OccupancyGrid {
	m0 := &OccupancyGrid{}
	b, x := &b0, m0
	_, _ = b, x
	x.xxx_hidden_Rows = b.Rows
	x.xxx_hidden_Cols = b.Cols
	x.xxx_hidden_Cells = b.Cells
	return m0
}

// / The occupancy grid event.
// / Contains the occupancy grid, the range of the grid and the resolution.
// /
// / Grid Coverage:
// /   The grid covers the x and y range inclusively with the specified
// /   resolution.
// /
// / Grid Dims Calculation:
// /   Computed as:
// /     `x_dims = floor((x_max - x_min) / resolution) + 1`
// /
// / Projecting a point onto the Grid:
// /   To determine the grid cell corresponding to a point in space:
// /     `grid_x = floor((point.x - x_min) / resolution)`
// /     `grid_y = floor((point.y - y_min) / resolution)`
type OccupancyGridEvent struct {
	state                 protoimpl.MessageState `protogen:"opaque.v1"`
	xxx_hidden_Grid       *OccupancyGrid         `protobuf:"bytes,1,opt,name=grid,proto3" json:"grid,omitempty"`
	xxx_hidden_XMin       float32                `protobuf:"fixed32,2,opt,name=x_min,json=xMin,proto3" json:"x_min,omitempty"`
	xxx_hidden_XMax       float32                `protobuf:"fixed32,3,opt,name=x_max,json=xMax,proto3" json:"x_max,omitempty"`
	xxx_hidden_YMin       float32                `protobuf:"fixed32,4,opt,name=y_min,json=yMin,proto3" json:"y_min,omitempty"`
	xxx_hidden_YMax       float32                `protobuf:"fixed32,5,opt,name=y_max,json=yMax,proto3" json:"y_max,omitempty"`
	xxx_hidden_Resolution float32                `protobuf:"fixed32,6,opt,name=resolution,proto3" json:"resolution,omitempty"`
	unknownFields         protoimpl.UnknownFields
	sizeCache             protoimpl.SizeCache
}

func (x *OccupancyGridEvent) Reset() {
	*x = OccupancyGridEvent{}
	mi := &file_horus_pb_preprocessing_messages_proto_msgTypes[1]
	ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
	ms.StoreMessageInfo(mi)
}

func (x *OccupancyGridEvent) String() string {
	return protoimpl.X.MessageStringOf(x)
}

func (*OccupancyGridEvent) ProtoMessage() {}

func (x *OccupancyGridEvent) ProtoReflect() protoreflect.Message {
	mi := &file_horus_pb_preprocessing_messages_proto_msgTypes[1]
	if x != nil {
		ms := protoimpl.X.MessageStateOf(protoimpl.Pointer(x))
		if ms.LoadMessageInfo() == nil {
			ms.StoreMessageInfo(mi)
		}
		return ms
	}
	return mi.MessageOf(x)
}

func (x *OccupancyGridEvent) GetGrid() *OccupancyGrid {
	if x != nil {
		return x.xxx_hidden_Grid
	}
	return nil
}

func (x *OccupancyGridEvent) GetXMin() float32 {
	if x != nil {
		return x.xxx_hidden_XMin
	}
	return 0
}

func (x *OccupancyGridEvent) GetXMax() float32 {
	if x != nil {
		return x.xxx_hidden_XMax
	}
	return 0
}

func (x *OccupancyGridEvent) GetYMin() float32 {
	if x != nil {
		return x.xxx_hidden_YMin
	}
	return 0
}

func (x *OccupancyGridEvent) GetYMax() float32 {
	if x != nil {
		return x.xxx_hidden_YMax
	}
	return 0
}

func (x *OccupancyGridEvent) GetResolution() float32 {
	if x != nil {
		return x.xxx_hidden_Resolution
	}
	return 0
}

func (x *OccupancyGridEvent) SetGrid(v *OccupancyGrid) {
	x.xxx_hidden_Grid = v
}

func (x *OccupancyGridEvent) SetXMin(v float32) {
	x.xxx_hidden_XMin = v
}

func (x *OccupancyGridEvent) SetXMax(v float32) {
	x.xxx_hidden_XMax = v
}

func (x *OccupancyGridEvent) SetYMin(v float32) {
	x.xxx_hidden_YMin = v
}

func (x *OccupancyGridEvent) SetYMax(v float32) {
	x.xxx_hidden_YMax = v
}

func (x *OccupancyGridEvent) SetResolution(v float32) {
	x.xxx_hidden_Resolution = v
}

func (x *OccupancyGridEvent) HasGrid() bool {
	if x == nil {
		return false
	}
	return x.xxx_hidden_Grid != nil
}

func (x *OccupancyGridEvent) ClearGrid() {
	x.xxx_hidden_Grid = nil
}

type OccupancyGridEvent_builder struct {
	_ [0]func() // Prevents comparability and use of unkeyed literals for the builder.

	Grid       *OccupancyGrid
	XMin       float32
	XMax       float32
	YMin       float32
	YMax       float32
	Resolution float32
}

func (b0 OccupancyGridEvent_builder) Build() *OccupancyGridEvent {
	m0 := &OccupancyGridEvent{}
	b, x := &b0, m0
	_, _ = b, x
	x.xxx_hidden_Grid = b.Grid
	x.xxx_hidden_XMin = b.XMin
	x.xxx_hidden_XMax = b.XMax
	x.xxx_hidden_YMin = b.YMin
	x.xxx_hidden_YMax = b.YMax
	x.xxx_hidden_Resolution = b.Resolution
	return m0
}

var File_horus_pb_preprocessing_messages_proto protoreflect.FileDescriptor

var file_horus_pb_preprocessing_messages_proto_rawDesc = []byte{
	0x0a, 0x25, 0x68, 0x6f, 0x72, 0x75, 0x73, 0x2f, 0x70, 0x62, 0x2f, 0x70, 0x72, 0x65, 0x70, 0x72,
	0x6f, 0x63, 0x65, 0x73, 0x73, 0x69, 0x6e, 0x67, 0x2f, 0x6d, 0x65, 0x73, 0x73, 0x61, 0x67, 0x65,
	0x73, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x08, 0x68, 0x6f, 0x72, 0x75, 0x73, 0x2e, 0x70,
	0x62, 0x22, 0x4d, 0x0a, 0x0d, 0x4f, 0x63, 0x63, 0x75, 0x70, 0x61, 0x6e, 0x63, 0x79, 0x47, 0x72,
	0x69, 0x64, 0x12, 0x12, 0x0a, 0x04, 0x72, 0x6f, 0x77, 0x73, 0x18, 0x01, 0x20, 0x01, 0x28, 0x0d,
	0x52, 0x04, 0x72, 0x6f, 0x77, 0x73, 0x12, 0x12, 0x0a, 0x04, 0x63, 0x6f, 0x6c, 0x73, 0x18, 0x02,
	0x20, 0x01, 0x28, 0x0d, 0x52, 0x04, 0x63, 0x6f, 0x6c, 0x73, 0x12, 0x14, 0x0a, 0x05, 0x63, 0x65,
	0x6c, 0x6c, 0x73, 0x18, 0x03, 0x20, 0x03, 0x28, 0x0d, 0x52, 0x05, 0x63, 0x65, 0x6c, 0x6c, 0x73,
	0x22, 0xb5, 0x01, 0x0a, 0x12, 0x4f, 0x63, 0x63, 0x75, 0x70, 0x61, 0x6e, 0x63, 0x79, 0x47, 0x72,
	0x69, 0x64, 0x45, 0x76, 0x65, 0x6e, 0x74, 0x12, 0x2b, 0x0a, 0x04, 0x67, 0x72, 0x69, 0x64, 0x18,
	0x01, 0x20, 0x01, 0x28, 0x0b, 0x32, 0x17, 0x2e, 0x68, 0x6f, 0x72, 0x75, 0x73, 0x2e, 0x70, 0x62,
	0x2e, 0x4f, 0x63, 0x63, 0x75, 0x70, 0x61, 0x6e, 0x63, 0x79, 0x47, 0x72, 0x69, 0x64, 0x52, 0x04,
	0x67, 0x72, 0x69, 0x64, 0x12, 0x13, 0x0a, 0x05, 0x78, 0x5f, 0x6d, 0x69, 0x6e, 0x18, 0x02, 0x20,
	0x01, 0x28, 0x02, 0x52, 0x04, 0x78, 0x4d, 0x69, 0x6e, 0x12, 0x13, 0x0a, 0x05, 0x78, 0x5f, 0x6d,
	0x61, 0x78, 0x18, 0x03, 0x20, 0x01, 0x28, 0x02, 0x52, 0x04, 0x78, 0x4d, 0x61, 0x78, 0x12, 0x13,
	0x0a, 0x05, 0x79, 0x5f, 0x6d, 0x69, 0x6e, 0x18, 0x04, 0x20, 0x01, 0x28, 0x02, 0x52, 0x04, 0x79,
	0x4d, 0x69, 0x6e, 0x12, 0x13, 0x0a, 0x05, 0x79, 0x5f, 0x6d, 0x61, 0x78, 0x18, 0x05, 0x20, 0x01,
	0x28, 0x02, 0x52, 0x04, 0x79, 0x4d, 0x61, 0x78, 0x12, 0x1e, 0x0a, 0x0a, 0x72, 0x65, 0x73, 0x6f,
	0x6c, 0x75, 0x74, 0x69, 0x6f, 0x6e, 0x18, 0x06, 0x20, 0x01, 0x28, 0x02, 0x52, 0x0a, 0x72, 0x65,
	0x73, 0x6f, 0x6c, 0x75, 0x74, 0x69, 0x6f, 0x6e, 0x2a, 0x73, 0x0a, 0x17, 0x4f, 0x63, 0x63, 0x75,
	0x70, 0x61, 0x6e, 0x63, 0x79, 0x43, 0x6c, 0x61, 0x73, 0x73, 0x69, 0x66, 0x69, 0x63, 0x61, 0x74,
	0x69, 0x6f, 0x6e, 0x12, 0x27, 0x0a, 0x23, 0x4f, 0x43, 0x43, 0x55, 0x50, 0x41, 0x4e, 0x43, 0x59,
	0x43, 0x4c, 0x41, 0x53, 0x53, 0x49, 0x46, 0x49, 0x43, 0x41, 0x54, 0x49, 0x4f, 0x4e, 0x5f, 0x55,
	0x4e, 0x53, 0x50, 0x45, 0x43, 0x49, 0x46, 0x49, 0x45, 0x44, 0x10, 0x00, 0x12, 0x08, 0x0a, 0x04,
	0x46, 0x52, 0x45, 0x45, 0x10, 0x01, 0x12, 0x0c, 0x0a, 0x08, 0x4f, 0x43, 0x43, 0x4c, 0x55, 0x44,
	0x45, 0x44, 0x10, 0x02, 0x12, 0x17, 0x0a, 0x13, 0x53, 0x54, 0x41, 0x54, 0x49, 0x4f, 0x4e, 0x41,
	0x52, 0x59, 0x5f, 0x4f, 0x43, 0x43, 0x55, 0x50, 0x49, 0x45, 0x44, 0x10, 0x03, 0x62, 0x06, 0x70,
	0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var file_horus_pb_preprocessing_messages_proto_enumTypes = make([]protoimpl.EnumInfo, 1)
var file_horus_pb_preprocessing_messages_proto_msgTypes = make([]protoimpl.MessageInfo, 2)
var file_horus_pb_preprocessing_messages_proto_goTypes = []any{
	(OccupancyClassification)(0), // 0: horus.pb.OccupancyClassification
	(*OccupancyGrid)(nil),        // 1: horus.pb.OccupancyGrid
	(*OccupancyGridEvent)(nil),   // 2: horus.pb.OccupancyGridEvent
}
var file_horus_pb_preprocessing_messages_proto_depIdxs = []int32{
	1, // 0: horus.pb.OccupancyGridEvent.grid:type_name -> horus.pb.OccupancyGrid
	1, // [1:1] is the sub-list for method output_type
	1, // [1:1] is the sub-list for method input_type
	1, // [1:1] is the sub-list for extension type_name
	1, // [1:1] is the sub-list for extension extendee
	0, // [0:1] is the sub-list for field type_name
}

func init() { file_horus_pb_preprocessing_messages_proto_init() }
func file_horus_pb_preprocessing_messages_proto_init() {
	if File_horus_pb_preprocessing_messages_proto != nil {
		return
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_horus_pb_preprocessing_messages_proto_rawDesc,
			NumEnums:      1,
			NumMessages:   2,
			NumExtensions: 0,
			NumServices:   0,
		},
		GoTypes:           file_horus_pb_preprocessing_messages_proto_goTypes,
		DependencyIndexes: file_horus_pb_preprocessing_messages_proto_depIdxs,
		EnumInfos:         file_horus_pb_preprocessing_messages_proto_enumTypes,
		MessageInfos:      file_horus_pb_preprocessing_messages_proto_msgTypes,
	}.Build()
	File_horus_pb_preprocessing_messages_proto = out.File
	file_horus_pb_preprocessing_messages_proto_rawDesc = nil
	file_horus_pb_preprocessing_messages_proto_goTypes = nil
	file_horus_pb_preprocessing_messages_proto_depIdxs = nil
}
