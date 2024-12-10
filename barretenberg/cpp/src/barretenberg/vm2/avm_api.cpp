#include "barretenberg/vm2/avm_api.hpp"

#include "barretenberg/vm2/proving_helper.hpp"
#include "barretenberg/vm2/simulation_helper.hpp"
#include "barretenberg/vm2/tracegen_helper.hpp"

namespace bb::avm2 {

using namespace bb::avm2::simulation;

std::tuple<AvmAPI::AvmProof, AvmAPI::AvmVerificationKey> AvmAPI::prove(const AvmAPI::Inputs& inputs)
{
    // Simulate.
    AvmSimulationHelper simulation_helper(inputs);
    auto events = simulation_helper.simulate();

    // Generate trace.
    AvmTraceGenHelper tracegen_helper;
    auto trace = tracegen_helper.generate_trace(std::move(events));

    // Prove.
    AvmProvingHelper proving_helper;
    auto proof = proving_helper.prove(std::move(trace));

    // FIXME: No VK.
    return { std::move(proof), {} };
}

} // namespace bb::avm2