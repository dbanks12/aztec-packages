#pragma once
#include "aztec3/circuits/apps/oracle_wrapper.hpp"
#include "aztec3/circuits/recursion/aggregator.hpp"
#include "aztec3/oracle/oracle.hpp"
#include "aztec3/utils/dummy_circuit_builder.hpp"
#include "aztec3/utils/types/circuit_types.hpp"
#include "aztec3/utils/types/convert.hpp"
#include "aztec3/utils/types/native_types.hpp"

namespace aztec3::circuits::kernel::public_kernel {

using Builder = proof_system::UltraCircuitBuilder;

using Aggregator = aztec3::circuits::recursion::Aggregator;

// Generic:
using CT = aztec3::utils::types::CircuitTypes<Builder>;
using NT = aztec3::utils::types::NativeTypes;
using aztec3::utils::types::to_ct;
using CircuitErrorCode = aztec3::utils::CircuitErrorCode;

using DB = oracle::FakeDB;
using oracle::NativeOracle;
using OracleWrapper = aztec3::circuits::apps::OracleWrapperInterface<Builder>;

// Used when calling library functions like `push_array` which have their own generic error code.
// So we pad this in front of the error message to identify where the error originally came from.
const std::string PUBLIC_KERNEL_CIRCUIT_ERROR_MESSAGE_BEGINNING = "public_kernel_circuit: ";

}  // namespace aztec3::circuits::kernel::public_kernel