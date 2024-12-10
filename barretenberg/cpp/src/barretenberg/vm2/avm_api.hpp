#pragma once

#include <tuple>

#include "barretenberg/vm2/proving_helper.hpp"
#include "barretenberg/vm2/simulation/lib/avm_inputs.hpp"

namespace bb::avm2 {

class AvmAPI {
  public:
    using AvmProof = AvmProvingHelper::Proof;
    using AvmVerificationKey = std::vector<uint8_t>;
    using Inputs = simulation::AvmInputs;

    AvmAPI() = default;

    std::tuple<AvmProof, AvmVerificationKey> prove(const Inputs& inputs);
};

} // namespace bb::avm2