#pragma once

#include "barretenberg/vm2/common/field.hpp"
#include "barretenberg/vm2/generated/columns.hpp"
#include "barretenberg/vm2/tracegen/trace_container.hpp"
#include <cstddef>
#include <span>

namespace bb::avm2::tracegen {

// TODO: This builds the counts. Think if it should do more or change name.

template <size_t TUPLE_SIZE> class BaseLookupTraceBuilder {
  public:
    BaseLookupTraceBuilder(Column src_selector,
                           Column dst_selector,
                           Column counts,
                           Column inverses,
                           std::array<Column, TUPLE_SIZE> src_columns,
                           std::array<Column, TUPLE_SIZE> dst_columns)
        : src_selector(src_selector)
        , dst_selector(dst_selector)
        , counts(counts)
        , inverses(inverses)
        , src_columns(std::move(src_columns))
        , dst_columns(std::move(dst_columns))
    {}
    virtual ~BaseLookupTraceBuilder() = default;

    void process(TraceContainer& trace)
    {
        // Let "src_sel {c1, c2, ...} in dst_sel {d1, d2, ...}" be a lookup,
        // For each row that has a 1 in the src_sel, we take the values of {c1, c2, ...},
        // find a row dst_row in the target columns {d1, d2, ...} where the values match.
        // Then we increment the count in the counts column at dst_row.
        // The complexity is O(|src_selector|) * O(find_in_dst).
        trace.visit_column(src_selector, [&](uint32_t row, const FF& src_sel_value) {
            assert(src_sel_value == 1);

            auto src_values = trace.get_multiple(src_columns, row);
            uint32_t dst_row = find_in_dst(src_values); // Assumes an efficient implementation.
            trace.set(counts, dst_row, trace.get(counts, dst_row) + 1);

            // We set a dummy value in the inverse column so that the size of the column is right.
            // The correct value will be set by the prover.
            trace.set(Column::lookup_dummy_inv, row, 0xdeadbeef);
        });

        // We set a dummy value in the inverse column so that the size of the column is right.
        // The correct value will be set by the prover.
        trace.visit_column(dst_selector,
                           [&](uint32_t row, const FF&) { trace.set(Column::lookup_dummy_inv, row, 0xdeadbeef); });
    }

  protected:
    static constexpr size_t tuple_size = TUPLE_SIZE;
    Column src_selector;
    Column dst_selector;
    Column counts;
    Column inverses;

    std::array<Column, TUPLE_SIZE> src_columns;
    std::array<Column, TUPLE_SIZE> dst_columns;

    virtual uint32_t find_in_dst(std::span<const FF*> tup) const = 0;
};

} // namespace bb::avm2::tracegen