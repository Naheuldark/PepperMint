#pragma once

namespace PepperMint {

// "UUID" (universally unique identifier) or GUID is (usually) a 128-bit integer used to "uniquely" identify information. In Hazel, even though we use
// the term GUID and UUID, at the moment we're simply using a randomly generated 64-bit integer, as the possibility of a clash is low enough for now.
// This may change in the future.

class UUID {
  public:
    UUID();
    UUID(uint64_t iUUID);
    UUID(const UUID& iOther);

    ~UUID() = default;

    operator uint64_t() { return _uuid; }
    operator const uint64_t() const { return _uuid; }

  private:
    uint64_t _uuid;
};
}

namespace std {
template <typename T>
struct hash;

template <>
struct hash<PepperMint::UUID> {
    size_t operator()(const PepperMint::UUID& iUUID) const { return (uint64_t)iUUID; }
};
}
