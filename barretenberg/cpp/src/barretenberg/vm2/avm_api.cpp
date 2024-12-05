#include "barretenberg/vm2/avm_api.hpp"

#include "barretenberg/vm2/simulation_helper.hpp"
#include "barretenberg/vm2/tracegen_helper.hpp"

namespace bb::avm {

using namespace bb::avm::simulation;

std::tuple<AvmAPI::AvmProof, AvmAPI::AvmVerificationKey> AvmAPI::prove(const AvmAPI::Inputs& inputs)
{
    // Simulate.
    AvmSimulationHelper simulation_helper(inputs);
    auto events = simulation_helper.simulate();

    // Generate trace.
    AvmTraceGenHelper tracegen_helper;
    [[maybe_unused]] auto trace = tracegen_helper.generate_trace(events);

    // Prove.
    // TBD: Implement proving. This depends on the flavor.

    return { AvmProof(), 0 };
}

} // namespace bb::avm