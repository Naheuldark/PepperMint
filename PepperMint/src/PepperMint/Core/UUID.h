#pragma once

namespace PepperMint {

class UUID {
  public:
    UUID();
    UUID(uint64_t iUUID);
    UUID(const UUID&) = default;

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
