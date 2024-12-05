#pragma once

#include <tuple>

#include "barretenberg/honk/proof_system/types/proof.hpp"
#include "barretenberg/vm2/simulation/lib/avm_inputs.hpp"

namespace bb::avm {

class AvmAPI {
  public:
    using AvmProof = bb::HonkProof;
    using AvmVerificationKey = int;
    using Inputs = simulation::ExecutionHints;

    AvmAPI() = default;

    std::tuple<AvmProof, AvmVerificationKey> prove(const Inputs& inputs);
};

} // namespace bb::avm