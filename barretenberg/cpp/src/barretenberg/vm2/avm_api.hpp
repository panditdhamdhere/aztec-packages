#pragma once

#include <tuple>

#include "barretenberg/vm2/proving_helper.hpp"
#include "barretenberg/vm2/simulation/lib/avm_inputs.hpp"

namespace bb::avm2 {

class AvmAPI {
  public:
    using AvmProof = AvmProvingHelper::Proof;
    using AvmVerificationKey = std::vector<uint8_t>;
    using ProvingInputs = simulation::AvmProvingInputs;
    using PublicInputs = simulation::PublicInputs;

    AvmAPI() = default;

    // NOTE: The public inputs are NOT part of the proof.
    std::pair<AvmProof, AvmVerificationKey> prove(const ProvingInputs& inputs);
    bool verify(const AvmProof& proof, const PublicInputs& pi, const AvmVerificationKey& vk_data);
};

} // namespace bb::avm2