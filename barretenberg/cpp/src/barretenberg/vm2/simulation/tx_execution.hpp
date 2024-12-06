#pragma once

#include "barretenberg/vm2/simulation/execution.hpp"
#include "barretenberg/vm2/simulation/lib/avm_inputs.hpp"

namespace bb::avm::simulation {

// Temporary.
struct Tx {
    std::vector<PublicExecutionRequest> enqueued_calls;
};

// In charge of executing a transaction.
class TxExecution final {
  public:
    TxExecution(ExecutionInterface& call_execution)
        : call_execution(call_execution){};

    void simulate(const Tx& tx);

  private:
    ExecutionInterface& call_execution;
};

} // namespace bb::avm::simulation