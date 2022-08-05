#include "pmpch.h"

#include <random>

#include "PepperMint/Core/UUID.h"

namespace PepperMint {

static std::random_device                      sRandomDevice;
static std::mt19937_64                         sEngine(sRandomDevice());
static std::uniform_int_distribution<uint64_t> sUniformDistribution;

UUID::UUID() : _uuid(sUniformDistribution(sEngine)) {}
UUID::UUID(uint64_t iUUID) : _uuid(iUUID) {}
}
