#pragma once

#include <list>

#include "barretenberg/vm2/simulation/events/events_container.hpp"
#include "barretenberg/vm2/simulation/lib/avm_inputs.hpp"

namespace bb::avm {

class AvmSimulationHelper {
  public:
    AvmSimulationHelper(const simulation::ExecutionHints& inputs)
        : inputs(inputs)
    {}

    // Full simulation with event collection.
    simulation::EventsContainer simulate();

    // Fast simulation without event collection.
    void simulate_fast();

  private:
    simulation::ExecutionHints inputs;
};

} // namespace bb::avm