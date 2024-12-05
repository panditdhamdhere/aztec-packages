// NOTE: names are in camel-case because they matter to messagepack.
// DO NOT use camel-case outside of these structures.
#pragma once

#include <vector>

#include "barretenberg/serialize/msgpack.hpp"
#include "barretenberg/vm2/common/aztec_types.hpp"
#include "barretenberg/vm2/common/field.hpp"

namespace bb::avm::simulation {

struct PublicKeysHint {
    AffinePoint masterNullifierPublicKey;
    AffinePoint masterIncomingViewingPublicKey;
    AffinePoint masterOutgoingViewingPublicKey;
    AffinePoint masterTaggingPublicKey;

    MSGPACK_FIELDS(masterNullifierPublicKey,
                   masterIncomingViewingPublicKey,
                   masterOutgoingViewingPublicKey,
                   masterTaggingPublicKey);
};

struct ContractInstanceHint {
    AztecAddress address;
    bool exists;
    simulation::FF salt;
    AztecAddress deployer;
    ContractClassId contractClassId;
    simulation::FF initializationHash;
    PublicKeysHint publicKeys;

    MSGPACK_FIELDS(address, exists, salt, deployer, contractClassId, initializationHash, publicKeys);
};

struct ContractClassHint {
    simulation::FF artifact_hash;
    simulation::FF private_function_root;
    simulation::FF public_bytecode_commitment;
    std::vector<uint8_t> packed_bytecode;

    MSGPACK_FIELDS(artifact_hash, private_function_root, public_bytecode_commitment, packed_bytecode);
};

// Temporary.
struct ExecutionHints {
    std::vector<ContractInstanceHint> contractInstances;
    std::vector<ContractClassHint> contractClasses;

    static ExecutionHints from(const std::vector<uint8_t>& data);

    MSGPACK_FIELDS(contractInstances, contractClasses);
};

} // namespace bb::avm::simulation