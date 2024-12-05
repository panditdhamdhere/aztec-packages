#include "barretenberg/vm2/simulation/addressing.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>

#include "barretenberg/vm2/common/memory_types.hpp"
#include "barretenberg/vm2/simulation/events/addressing_event.hpp"
#include "barretenberg/vm2/simulation/events/event_emitter.hpp"
#include "barretenberg/vm2/simulation/events/memory_event.hpp"
#include "barretenberg/vm2/simulation/lib/serialization.hpp"
#include "barretenberg/vm2/simulation/memory.hpp"

namespace bb::avm::simulation {
namespace {

// These exceptions are internal to this file, to guide the processing and recovery.
struct AddressingException : public std::runtime_error {
    explicit AddressingException(AddressingEventError e)
        : std::runtime_error("Addressing error")
        , error(e)
    {}
    AddressingEventError error;
};

struct OperandResolutionException : public AddressingException {
    explicit OperandResolutionException(AddressingEventError e, size_t operand_idx)
        : AddressingException(e)
        , operand_idx(operand_idx)
    {}
    size_t operand_idx;
};

} // namespace

std::vector<Operand> Addressing::resolve(const Instruction& instruction, MemoryInterface& memory) const
{
    // We'll be filling in the event as we progress.
    AddressingEvent event;
    event.instruction = instruction;

    try {
        // Note: it's fine to query instruction info in here since it does not trigger events.
        ExecutionOpCode opcode = instruction_info_db.map_wire_opcode_to_execution_opcode(instruction.opcode);
        const InstructionSpec& spec = instruction_info_db.get(opcode);
        event.spec = &spec;
        // This represents either: (1) wrong info in the spec, or (2) a wrong witgen deserialization.
        // Therefore, it is not an error the circuit should be able to prove.
        assert(instruction.operands.size() <= spec.num_addresses);

        // We retrieve, cache and check M[0] first because this is probably what we'll do in the circuit.
        auto stack_pointer = memory.get(0);
        event.stack_pointer_tag = stack_pointer.tag;
        event.stack_pointer_val = stack_pointer.value;
        if (!memory.is_valid_address(stack_pointer)) {
            throw AddressingException(AddressingEventError::STACK_POINTER_INVALID_ADDRESS);
        }

        // First process relative addressing for all the addresses.
        event.after_relative = instruction.operands;
        for (size_t i = 0; i < spec.num_addresses; ++i) {
            if ((instruction.indirect >> i) & 1) {
                MemoryValue offset(event.after_relative[i]);
                offset += stack_pointer.value;
                event.after_relative[i] = static_cast<Operand>(offset);
                if (!memory.is_valid_address(offset)) {
                    throw OperandResolutionException(AddressingEventError::RELATIVE_COMPUTATION_OOB, i);
                }
            }
        }

        // Then indirection.
        event.resolved_operands = event.after_relative;
        for (size_t i = 0; i < spec.num_addresses; ++i) {
            if ((instruction.indirect >> (i + spec.num_addresses)) & 1) {
                MemoryValue offset(event.resolved_operands[i]);
                if (!memory.is_valid_address(offset)) {
                    throw OperandResolutionException(AddressingEventError::INDIRECT_INVALID_ADDRESS, i);
                }
                // Observe that the new address can still be invalid as an address.
                // TODO: consider the guarantees of the output of this "gadget".
                auto new_address = memory.get(static_cast<MemoryAddress>(offset));
                event.resolved_operands[i] = static_cast<Operand>(new_address.value);
            }
        }
    } catch (const OperandResolutionException& e) {
        event.error = e.error;
        event.error_operand_idx = e.operand_idx;
    } catch (const AddressingException& e) {
        event.error = e.error;
    }

    events.emit(AddressingEvent(event));
    return event.resolved_operands;
}

} // namespace bb::avm::simulation