# Horus Protobuf

The Horus SDK does not depend on `libprotobuf` or `libprotobuf-lite`. Instead,
custom Protobuf code generation and types are used for several reasons:

1. `libprotobuf-lite` is large, and not Autosar compliant.

2. `protoc`-generated types may be recursive, so messages are all
   heap-allocated.

3. We would like to use custom types in some cases (e.g. `Vector3f`), which
   requires many helper functions to transform Protobuf types to native C++
   types. Our implementation of Protobuf uses `PbTraits<T>` for serialization,
   allowing custom types to be used in Protobuf messages.

Horus message types may not be recursive, which allows them to be stored inline
with no allocation.

Additionally, all Horus messages use Copy-on-Write (often referred to as CoW) to
avoid copies. Constructing a Horus message is straightforward and uses regular
C++ types such as `std::vector`. However, when a message is received, it can be
efficiently shared with code which does not mutate it using CoW semantics. The
message will be deserialized lazily, with no copy or allocation taking place (as
long as it isn't mutated).

Generated code follows the naming and usage conventions of `protoc --cpp_out`;
using `mutable_` or `set_` on a field will mark it as set. There is one
distinction, however: unlike `protoc --cpp_out`, generated fields here are
implicitly `optional`, i.e. they all have `has_` methods and are always
serialized if they were set, even if they are set to their default value.
