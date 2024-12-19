#pragma once

#include <cassert>

#include "barretenberg/vm2/tracegen/lib/lookup_builder.hpp"

namespace bb::avm2::tracegen {

class LookupIntoBitwise : public BaseLookupTraceBuilder<4> {
  public:
    LookupIntoBitwise(Column src_selector_column, Column counts_column, std::array<Column, 4> src_columns)
        : BaseLookupTraceBuilder<4>(src_selector_column,
                                    Column::precomputed_sel_bitwise,
                                    counts_column,
                                    Column::lookup_dummy_inv,
                                    src_columns,
                                    { Column::precomputed_bitwise_op_id,
                                      Column::precomputed_bitwise_input_a,
                                      Column::precomputed_bitwise_input_b,
                                      Column::precomputed_bitwise_output })
    {}

  private:
    // This is an efficient implementation of indexing into the precomputed table.
    uint32_t find_in_dst(std::span<const FF*> tup) const override
    {
        assert(tuple_size == tup.size());
        auto op_id = static_cast<uint32_t>(*tup[0]);
        auto a = static_cast<uint32_t>(*tup[1]);
        auto b = static_cast<uint32_t>(*tup[2]);
        return (op_id << 16) + (a << 8) + b;
    }
};

} // namespace bb::avm2::tracegen