#include "barretenberg/vm2/avm_api.hpp"

#include "barretenberg/vm/stats.hpp"
#include "barretenberg/vm2/proving_helper.hpp"
#include "barretenberg/vm2/simulation_helper.hpp"
#include "barretenberg/vm2/tracegen_helper.hpp"

namespace bb::avm2 {

using namespace bb::avm2::simulation;

std::tuple<AvmAPI::AvmProof, AvmAPI::AvmVerificationKey> AvmAPI::prove(const AvmAPI::Inputs& inputs)
{
    // Simulate.
    info("Simulating...");
    AvmSimulationHelper simulation_helper(inputs);
    auto events = AVM_TRACK_TIME_V("simulation/all", simulation_helper.simulate());

    // Generate trace.
    info("Generating trace...");
    AvmTraceGenHelper tracegen_helper;
    auto trace = AVM_TRACK_TIME_V("tracegen/all", tracegen_helper.generate_trace(std::move(events)));

    // Prove.
    info("Proving...");
    AvmProvingHelper proving_helper;
    auto proof = AVM_TRACK_TIME_V("proving/all", proving_helper.prove(std::move(trace)));

    // FIXME: No VK.
    info("Done!");
    return { std::move(proof), {} };
}

} // namespace bb::avm2