#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <ranges>
#include <span>
#include <unordered_map>

#include "barretenberg/vm2/generated/columns.hpp"
#include "barretenberg/vm2/generated/flavor_settings.hpp"

namespace bb::avm2::tracegen {

class TraceContainer {
  public:
    using FF = AvmFlavorSettings::FF;

    void set(Column col, size_t row, const FF& value);
    // Bulk setting for a given row.
    void set(size_t row, std::span<const std::pair<Column, FF>> values);

    // Visits non-zero values in a column.
    void visit_column(Column col, const std::function<void(size_t, const FF&)>& visitor) const;
    // Returns the number of rows in a column. That is, the maximum non-zero row index + 1.
    size_t get_column_size(Column col) const;

    // Free column memory.
    void clear_column(Column col);

  protected:
    // We store the trace as a sparse matrix.
    std::unordered_map<Column, /*rows*/ std::unordered_map<size_t, FF>> trace;
};

} // namespace bb::avm2::tracegen