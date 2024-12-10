#include "barretenberg/honk/proof_system/types/proof.hpp"
#include "barretenberg/vm2/generated/prover.hpp"

namespace bb::avm2::constraining {

// This class is a wrapper around the generated Avm2Prover.
// NOTE: It can be unified with the generated prover once the transition is done.
class AvmCustomProver : public AvmProver {
  public:
    using Proof = HonkProof;

    explicit AvmCustomProver(std::shared_ptr<ProvingKey> input_key, std::shared_ptr<PCSCommitmentKey> commitment_key)
        : AvmProver(std::move(input_key), std::move(commitment_key))
    {}
};

} // namespace bb::avm2::constraining