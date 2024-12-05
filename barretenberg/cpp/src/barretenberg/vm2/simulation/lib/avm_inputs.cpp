#include "barretenberg/vm2/simulation/lib/avm_inputs.hpp"

#include <vector>

#include "barretenberg/serialize/msgpack.hpp"

namespace bb::avm::simulation {

ExecutionHints ExecutionHints::from(const std::vector<uint8_t>& data)
{
    ExecutionHints hints;
    msgpack::unpack(reinterpret_cast<const char*>(data.data()), data.size()).get().convert(hints);
    return hints;
}

} // namespace bb::avm::simulation