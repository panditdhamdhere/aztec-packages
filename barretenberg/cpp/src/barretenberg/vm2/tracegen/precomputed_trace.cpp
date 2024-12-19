#include "barretenberg/vm2/tracegen/precomputed_trace.hpp"
#include "barretenberg/vm2/common/constants.hpp"

#include <cstddef>
#include <cstdint>

namespace bb::avm2::tracegen {

void PrecomputedTraceBuilder::process_misc(TraceContainer& trace)
{
    using C = Column;

    // First row.
    trace.set(C::precomputed_first_row, 0, 1);

    // Clk.
    // TODO: What a waste of 64MB. Can we elegantly have a flag for this?
    trace.reserve_column(C::precomputed_clk, CIRCUIT_SUBGROUP_SIZE);
    for (uint32_t i = 0; i < CIRCUIT_SUBGROUP_SIZE; i++) {
        trace.set(C::precomputed_clk, i, i);
    }
}

void PrecomputedTraceBuilder::process_bitwise(TraceContainer& trace)
{
    using C = Column;

    constexpr auto num_rows = 256 * 256 * 3;
    trace.reserve_column(C::precomputed_sel_bitwise, num_rows);
    trace.reserve_column(C::precomputed_bitwise_input_a, num_rows);
    trace.reserve_column(C::precomputed_bitwise_input_b, num_rows);
    trace.reserve_column(C::precomputed_bitwise_output, num_rows);

    uint32_t row = 0;
    // AND
    for (size_t a = 0; a < 256; a++) {
        for (size_t b = 0; b < 256; b++) {
            trace.set(row,
                      { {
                          { C::precomputed_sel_bitwise, 1 },
                          { C::precomputed_bitwise_input_a, FF(a) },
                          { C::precomputed_bitwise_input_b, FF(b) },
                          { C::precomputed_bitwise_output, FF(a & b) },
                      } });
            row++;
        }
    }
    // OR
    for (size_t a = 0; a < 256; a++) {
        for (size_t b = 0; b < 256; b++) {
            trace.set(row,
                      { {
                          { C::precomputed_sel_bitwise, 1 },
                          { C::precomputed_bitwise_input_a, FF(a) },
                          { C::precomputed_bitwise_input_b, FF(b) },
                          { C::precomputed_bitwise_output, FF(a | b) },
                      } });
            row++;
        }
    }
    // XOR
    for (size_t a = 0; a < 256; a++) {
        for (size_t b = 0; b < 256; b++) {
            trace.set(row,
                      { {
                          { C::precomputed_sel_bitwise, 1 },
                          { C::precomputed_bitwise_input_a, FF(a) },
                          { C::precomputed_bitwise_input_b, FF(b) },
                          { C::precomputed_bitwise_output, FF(a ^ b) },
                      } });
            row++;
        }
    }
}

} // namespace bb::avm2::tracegen