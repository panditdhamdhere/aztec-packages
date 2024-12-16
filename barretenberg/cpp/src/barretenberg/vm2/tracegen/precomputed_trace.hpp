#pragma once

#include "barretenberg/vm2/tracegen/trace_container.hpp"

namespace bb::avm2::tracegen {

// This fills the trace for the "general" precomputed columns.
// See precomputed.pil.
// Do not put more specific precomputed things (like powers, etc) here.
class PrecomputedTraceBuilder final {
  public:
    void process(TraceContainer& trace);
};

} // namespace bb::avm2::tracegen