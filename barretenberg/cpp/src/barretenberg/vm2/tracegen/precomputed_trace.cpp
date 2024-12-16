#include "barretenberg/vm2/tracegen/precomputed_trace.hpp"

#include <cstddef>
#include <cstdint>
#include <ranges>
#include <stdexcept>

namespace bb::avm2::tracegen {

void PrecomputedTraceBuilder::process(TraceContainer& trace)
{
    using C = Column;

    trace.set(C::precomputed_first_row, 0, 1);
}

} // namespace bb::avm2::tracegen