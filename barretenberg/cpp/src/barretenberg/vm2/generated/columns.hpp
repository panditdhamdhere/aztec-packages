#pragma once

#include <array>
#include <optional>

namespace bb::avm {

// The entities that will be used in the flavor.
// clang-format off
#define PRECOMPUTED_ENTITIES execution_zero
#define WIRE_ENTITIES execution_input, alu_dst_addr, alu_ia, alu_ia_addr, alu_ib, alu_ib_addr, alu_ic, alu_op, alu_sel_op_add, execution_clk, execution_last, execution_pc, execution_selector, lookup_dummy_counts
#define DERIVED_WITNESS_ENTITIES lookup_dummy_inv
#define SHIFTED_ENTITIES execution_pc_shift
#define TO_BE_SHIFTED(e) e.execution_pc
#define ALL_ENTITIES PRECOMPUTED_ENTITIES, WIRE_ENTITIES, DERIVED_WITNESS_ENTITIES, SHIFTED_ENTITIES

#define TO_BE_SHIFTED_COLUMNS Column::execution_pc
#define SHIFTED_COLUMNS ColumnAndShifts::execution_pc_shift

#define ALL_ENTITIES PRECOMPUTED_ENTITIES, WIRE_ENTITIES, DERIVED_WITNESS_ENTITIES, SHIFTED_ENTITIES
#define UNSHIFTED_ENTITIES PRECOMPUTED_ENTITIES, WIRE_ENTITIES, DERIVED_WITNESS_ENTITIES
// clang-format on

// All columns minus shifts.
enum class Column { UNSHIFTED_ENTITIES };

// C++ doesn't allow enum extension, so we'll have to cast.
enum class ColumnAndShifts {
    ALL_ENTITIES,
    // Sentinel.
    NUM_COLUMNS,
};

constexpr auto TO_BE_SHIFTED_COLUMNS_ARRAY = []() { return std::array{ TO_BE_SHIFTED_COLUMNS }; }();
constexpr auto SHIFTED_COLUMNS_ARRAY = []() { return std::array{ SHIFTED_COLUMNS }; }();
static_assert(TO_BE_SHIFTED_COLUMNS_ARRAY.size() == SHIFTED_COLUMNS_ARRAY.size());

} // namespace bb::avm