#include "barretenberg/honk/proof_system/types/proof.hpp"
#include "barretenberg/vm2/generated/prover.hpp"

namespace bb::avm2::constraining {

// This class is a wrapper around the generated Avm2Prover.
// NOTE: It can be unified with the generated prover once the transition is done.
class AvmCustomProver : public Avm2Prover {
  public:
    using Proof = HonkProof;
};

} // namespace bb::avm2::constraining