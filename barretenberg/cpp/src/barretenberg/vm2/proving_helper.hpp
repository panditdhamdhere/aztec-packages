#pragma once

#include "barretenberg/honk/proof_system/types/proof.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm2 {

class AvmProvingHelper {
  public:
    AvmProvingHelper() = default;
    using Proof = bb::HonkProof;

    Proof prove(tracegen::TraceContainer&& trace);
};

} // namespace bb::avm2