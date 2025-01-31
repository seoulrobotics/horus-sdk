/// @file
///
/// The `horus::pb` alias for `horus::sdk::pb` generated types.

#ifndef HORUS_PB_ALIAS_H_
#define HORUS_PB_ALIAS_H_

namespace horus {
namespace sdk {
namespace pb {

// Empty.

}  // namespace pb
}  // namespace sdk

/// Alias of `horus::sdk::pb`.
///
/// Using this alias is strongly preferred over using `horus::sdk::pb`; `horus::sdk::pb` should be
/// considered an implementation detail.
///
/// This is used internally by Horus to transition from Protobuf codegen to custom codegen. Because
/// types in the SDK and in Horus share the same name, linking the two together is impossible due to
/// conflicts. Using a nested namespace `sdk::pb` resolves this problem.
namespace pb = ::horus::sdk::pb;  // NOLINT(misc-unused-alias-decls)

}  // namespace horus

#endif  // HORUS_PB_ALIAS_H_
