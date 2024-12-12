#pragma once

#include <array>
#include <optional>

namespace bb::avm2 {

// The entities that will be used in the flavor.
// clang-format off
#define AVM2_PRECOMPUTED_ENTITIES execution_zero
#define AVM2_WIRE_ENTITIES execution_input, alu_dst_addr, alu_ia, alu_ia_addr, alu_ib, alu_ib_addr, alu_ic, alu_op, alu_sel_op_add, execution_clk, execution_last, execution_pc, execution_sel, execution_selector, lookup_dummy_counts
#define AVM2_DERIVED_WITNESS_ENTITIES lookup_dummy_inv
#define AVM2_SHIFTED_ENTITIES execution_pc_shift
#define AVM2_TO_BE_SHIFTED(e) e.execution_pc
#define AVM2_ALL_ENTITIES AVM2_PRECOMPUTED_ENTITIES, AVM2_WIRE_ENTITIES, AVM2_DERIVED_WITNESS_ENTITIES, AVM2_SHIFTED_ENTITIES
#define AVM2_UNSHIFTED_ENTITIES AVM2_PRECOMPUTED_ENTITIES, AVM2_WIRE_ENTITIES, AVM2_DERIVED_WITNESS_ENTITIES

#define AVM2_TO_BE_SHIFTED_COLUMNS Column::execution_pc
#define AVM2_SHIFTED_COLUMNS ColumnAndShifts::execution_pc_shift
// clang-format on

// All columns minus shifts.
enum class Column { AVM2_UNSHIFTED_ENTITIES };

// C++ doesn't allow enum extension, so we'll have to cast.
enum class ColumnAndShifts {
    AVM2_ALL_ENTITIES,
    // Sentinel.
    NUM_COLUMNS,
};

constexpr auto TO_BE_SHIFTED_COLUMNS_ARRAY = []() { return std::array{ AVM2_TO_BE_SHIFTED_COLUMNS }; }();
constexpr auto SHIFTED_COLUMNS_ARRAY = []() { return std::array{ AVM2_SHIFTED_COLUMNS }; }();
static_assert(TO_BE_SHIFTED_COLUMNS_ARRAY.size() == SHIFTED_COLUMNS_ARRAY.size());

} // namespace bb::avm2