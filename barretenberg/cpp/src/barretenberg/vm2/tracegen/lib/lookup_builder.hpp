#pragma once

#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/generated/columns.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"
#include <array>
#include <cstddef>
#include <span>

namespace bb::avm2::tracegen {

// TODO: This builds the counts. Think if it should do more or change name.

template <typename LookupSettings_> class BaseLookupTraceBuilder {
  public:
    virtual ~BaseLookupTraceBuilder() = default;

    void process(TraceContainer& trace)
    {
        // Let "src_sel {c1, c2, ...} in dst_sel {d1, d2, ...}" be a lookup,
        // For each row that has a 1 in the src_sel, we take the values of {c1, c2, ...},
        // find a row dst_row in the target columns {d1, d2, ...} where the values match.
        // Then we increment the count in the counts column at dst_row.
        // The complexity is O(|src_selector|) * O(find_in_dst).
        trace.visit_column(LookupSettings::SRC_SELECTOR, [&](uint32_t row, const FF& src_sel_value) {
            assert(src_sel_value == 1);

            std::array<FF, LookupSettings::LOOKUP_TUPLE_SIZE> src_values =
                trace.get_multiple(LookupSettings::SRC_COLUMNS, row);
            uint32_t dst_row = find_in_dst(src_values); // Assumes an efficient implementation.
            trace.set(LookupSettings::COUNTS, dst_row, trace.get(LookupSettings::COUNTS, dst_row) + 1);

            // We set a dummy value in the inverse column so that the size of the column is right.
            // The correct value will be set by the prover.
            trace.set(LookupSettings::INVERSES, row, 0xdeadbeef);
        });

        // We set a dummy value in the inverse column so that the size of the column is right.
        // The correct value will be set by the prover.
        trace.visit_column(LookupSettings::DST_SELECTOR,
                           [&](uint32_t row, const FF&) { trace.set(LookupSettings::INVERSES, row, 0xdeadbeef); });
    }

  protected:
    using LookupSettings = LookupSettings_;
    virtual uint32_t find_in_dst(const std::array<FF, LookupSettings::LOOKUP_TUPLE_SIZE> tup) const = 0;
};

} // namespace bb::avm2::tracegen