#include "barretenberg/vm2/proving_helper.hpp"

#include "barretenberg/common/thread.hpp"
#include "barretenberg/vm/stats.hpp"
#include "barretenberg/vm2/constraining/prover.hpp"
#include <memory>

using bb::avm2::constraining::AvmCustomProver;

namespace bb::avm2 {
namespace {

constexpr size_t circuit_subgroup_size = 1 << 21; // TODO: factor out.

// TODO: This doesn't need to be a shared_ptr, but BB requires it.
std::shared_ptr<AvmCustomProver::ProvingKey> create_proving_key(AvmCustomProver::ProverPolynomials& polynomials)
{
    auto proving_key = std::make_shared<AvmCustomProver::ProvingKey>(circuit_subgroup_size, 0);

    for (auto [key_poly, prover_poly] : zip_view(proving_key->get_all(), polynomials.get_unshifted())) {
        ASSERT(flavor_get_label(*proving_key, key_poly) == flavor_get_label(polynomials, prover_poly));
        key_poly = std::move(prover_poly);
    }

    proving_key->commitment_key = std::make_shared<AvmCustomProver::PCSCommitmentKey>(circuit_subgroup_size);

    return proving_key;
}

AvmCustomProver::ProverPolynomials compute_polynomials(tracegen::TraceContainer& trace)
{
    AvmCustomProver::ProverPolynomials polys;

    // catch-all with fully formed polynomials
    AVM_TRACK_TIME("proving/init_polys_unshifted", ({
                       auto unshifted = polys.get_unshifted();

                       // FIXME: We don't support handling of derived polynomials.
                       // Derived polynomials will be empty.
                       bb::parallel_for(unshifted.size(), [&](size_t i) {
                           // WARNING! Column-Polynomials order matters!
                           Column col = static_cast<Column>(i);
                           const auto num_rows = trace.get_column_size(col);

                           unshifted[i] = AvmCustomProver::Polynomial::create_non_parallel_zero_init(
                               num_rows, circuit_subgroup_size);
                       });
                   }));

    AVM_TRACK_TIME("proving/set_polys_unshifted", ({
                       auto unshifted = polys.get_unshifted();

                       // FIXME: We don't support handling of derived polynomials.
                       // TODO: We are now visiting per-column. Profile if per-row is better.
                       // This would need changes to the trace container.
                       bb::parallel_for(unshifted.size(), [&](size_t i) {
                           // WARNING! Column-Polynomials order matters!
                           auto& poly = unshifted[i];
                           Column col = static_cast<Column>(i);

                           trace.visit_column(col, [&](size_t row, const AvmCustomProver::FF& value) {
                               // We use `at` because we are sure the row exists and the value is non-zero.
                               poly.at(row) = value;
                           });
                           // We free columns as we go.
                           // TODO: If we merge the init with the setting, this would be even more memory efficient.
                           trace.clear_column(col);
                       });
                   }));

    AVM_TRACK_TIME("proving/set_polys_shifted", ({
                       for (auto [shifted, to_be_shifted] : zip_view(polys.get_shifted(), polys.get_to_be_shifted())) {
                           shifted = to_be_shifted.shifted();
                       }
                   }));

    return polys;
}

} // namespace

AvmProvingHelper::Proof AvmProvingHelper::prove(tracegen::TraceContainer&& trace)
{
    auto polynomials = AVM_TRACK_TIME_V("proving/create_prover:compute_polynomials", compute_polynomials(trace));
    const auto proving_key = AVM_TRACK_TIME_V("proving/create_prover:proving_key", create_proving_key(polynomials));
    auto prover = AVM_TRACK_TIME_V("proving/create_prover:construct_prover",
                                   AvmCustomProver(proving_key, proving_key->commitment_key));

    return AVM_TRACK_TIME_V("proving/construct_proof", prover.construct_proof());
}

} // namespace bb::avm2