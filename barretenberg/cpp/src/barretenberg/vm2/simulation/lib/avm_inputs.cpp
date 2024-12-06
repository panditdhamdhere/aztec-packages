#include "barretenberg/vm2/simulation/lib/avm_inputs.hpp"

#include <vector>

#include "barretenberg/serialize/msgpack.hpp"

namespace bb::avm::simulation {

AvmInputs AvmInputs::from(const std::vector<uint8_t>& data)
{
    AvmInputs inputs;
    msgpack::unpack(reinterpret_cast<const char*>(data.data()), data.size()).get().convert(inputs);
    return inputs;
}

} // namespace bb::avm::simulation