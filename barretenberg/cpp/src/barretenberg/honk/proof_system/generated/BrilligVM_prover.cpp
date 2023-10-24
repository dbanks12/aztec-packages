

#include "BrilligVM_prover.hpp"
#include "barretenberg/honk/pcs/claim.hpp"
#include "barretenberg/honk/pcs/commitment_key.hpp"
#include "barretenberg/honk/proof_system/lookup_library.hpp"
#include "barretenberg/honk/proof_system/permutation_library.hpp"
#include "barretenberg/honk/sumcheck/sumcheck.hpp"
#include "barretenberg/honk/utils/power_polynomial.hpp"
#include "barretenberg/polynomials/polynomial.hpp"
#include "barretenberg/polynomials/univariate.hpp" // will go away
#include "barretenberg/proof_system/relations/lookup_relation.hpp"
#include "barretenberg/proof_system/relations/permutation_relation.hpp"
#include "barretenberg/transcript/transcript_wrappers.hpp"
#include <algorithm>
#include <array>
#include <cstddef>
#include <memory>
#include <span>
#include <string>
#include <utility>
#include <vector>

namespace proof_system::honk {

/**
 * Create BrilligVMProver_ from proving key, witness and manifest.
 *
 * @param input_key Proving key.
 * @param input_manifest Input manifest
 *
 * @tparam settings Settings class.
 * */
template <typename Flavor>
BrilligVMProver_<Flavor>::BrilligVMProver_(std::shared_ptr<typename Flavor::ProvingKey> input_key,
                                           std::shared_ptr<PCSCommitmentKey> commitment_key)
    : key(input_key)
    , commitment_key(commitment_key)
{
    // TODO: take every polynomial and assign it to the key!!

    prover_polynomials.main_POSITIVE = key->main_POSITIVE;
    prover_polynomials.main_FIRST = key->main_FIRST;
    prover_polynomials.main_LAST = key->main_LAST;
    prover_polynomials.main_STEP = key->main_STEP;
    prover_polynomials.main__romgen_first_step = key->main__romgen_first_step;
    prover_polynomials.main_first_step = key->main_first_step;
    prover_polynomials.main_p_line = key->main_p_line;
    prover_polynomials.main_p_X_const = key->main_p_X_const;
    prover_polynomials.main_p_instr__jump_to_operation = key->main_p_instr__jump_to_operation;
    prover_polynomials.main_p_instr__loop = key->main_p_instr__loop;
    prover_polynomials.main_p_instr__reset = key->main_p_instr__reset;
    prover_polynomials.main_p_instr_call = key->main_p_instr_call;
    prover_polynomials.main_p_instr_call_param_l = key->main_p_instr_call_param_l;
    prover_polynomials.main_p_instr_ret = key->main_p_instr_ret;
    prover_polynomials.main_p_instr_return = key->main_p_instr_return;
    prover_polynomials.main_p_reg_write_X_r0 = key->main_p_reg_write_X_r0;
    prover_polynomials.main_p_reg_write_X_r1 = key->main_p_reg_write_X_r1;
    prover_polynomials.main_p_reg_write_X_r3 = key->main_p_reg_write_X_r3;
    prover_polynomials.main__block_enforcer_last_step = key->main__block_enforcer_last_step;
    prover_polynomials.main__linker_first_step = key->main__linker_first_step;
    prover_polynomials.main_XInv = key->main_XInv;
    prover_polynomials.main_XIsZero = key->main_XIsZero;
    prover_polynomials.main_m_addr = key->main_m_addr;
    prover_polynomials.main_m_step = key->main_m_step;
    prover_polynomials.main_m_change = key->main_m_change;
    prover_polynomials.main_m_value = key->main_m_value;
    prover_polynomials.main_m_op = key->main_m_op;
    prover_polynomials.main_m_is_write = key->main_m_is_write;
    prover_polynomials.main_m_is_read = key->main_m_is_read;
    prover_polynomials.main__operation_id = key->main__operation_id;
    prover_polynomials.main__sigma = key->main__sigma;
    prover_polynomials.main_pc = key->main_pc;
    prover_polynomials.main_X = key->main_X;
    prover_polynomials.main_Y = key->main_Y;
    prover_polynomials.main_Z = key->main_Z;
    prover_polynomials.main_jump_ptr = key->main_jump_ptr;
    prover_polynomials.main_addr = key->main_addr;
    prover_polynomials.main_tmp = key->main_tmp;
    prover_polynomials.main_reg_write_X_r0 = key->main_reg_write_X_r0;
    prover_polynomials.main_r0 = key->main_r0;
    prover_polynomials.main_reg_write_X_r1 = key->main_reg_write_X_r1;
    prover_polynomials.main_r1 = key->main_r1;
    prover_polynomials.main_r2 = key->main_r2;
    prover_polynomials.main_reg_write_X_r3 = key->main_reg_write_X_r3;
    prover_polynomials.main_r3 = key->main_r3;
    prover_polynomials.main_r4 = key->main_r4;
    prover_polynomials.main_r5 = key->main_r5;
    prover_polynomials.main_r6 = key->main_r6;
    prover_polynomials.main_r7 = key->main_r7;
    prover_polynomials.main_r8 = key->main_r8;
    prover_polynomials.main_r9 = key->main_r9;
    prover_polynomials.main_r10 = key->main_r10;
    prover_polynomials.main_r11 = key->main_r11;
    prover_polynomials.main_instr_call = key->main_instr_call;
    prover_polynomials.main_instr_call_param_l = key->main_instr_call_param_l;
    prover_polynomials.main_instr_ret = key->main_instr_ret;
    prover_polynomials.main_instr__jump_to_operation = key->main_instr__jump_to_operation;
    prover_polynomials.main_instr__reset = key->main_instr__reset;
    prover_polynomials.main_instr__loop = key->main_instr__loop;
    prover_polynomials.main_instr_return = key->main_instr_return;
    prover_polynomials.main_X_const = key->main_X_const;
    prover_polynomials.main_X_free_value = key->main_X_free_value;
    prover_polynomials.main_Y_free_value = key->main_Y_free_value;
    prover_polynomials.main_Z_free_value = key->main_Z_free_value;
    prover_polynomials.main__operation_id_no_change = key->main__operation_id_no_change;

    prover_polynomials.main_r7 = key->main_r7;
    prover_polynomials.main_r7_shift = key->main_r7.shifted();

    prover_polynomials.main__romgen_first_step = key->main__romgen_first_step;
    prover_polynomials.main__romgen_first_step_shift = key->main__romgen_first_step.shifted();

    prover_polynomials.main_r0 = key->main_r0;
    prover_polynomials.main_r0_shift = key->main_r0.shifted();

    prover_polynomials.main_r8 = key->main_r8;
    prover_polynomials.main_r8_shift = key->main_r8.shifted();

    prover_polynomials.main_r1 = key->main_r1;
    prover_polynomials.main_r1_shift = key->main_r1.shifted();

    prover_polynomials.main_r9 = key->main_r9;
    prover_polynomials.main_r9_shift = key->main_r9.shifted();

    prover_polynomials.main_r10 = key->main_r10;
    prover_polynomials.main_r10_shift = key->main_r10.shifted();

    prover_polynomials.main_m_is_write = key->main_m_is_write;
    prover_polynomials.main_m_is_write_shift = key->main_m_is_write.shifted();

    prover_polynomials.main_pc = key->main_pc;
    prover_polynomials.main_pc_shift = key->main_pc.shifted();

    prover_polynomials.main_tmp = key->main_tmp;
    prover_polynomials.main_tmp_shift = key->main_tmp.shifted();

    prover_polynomials.main_addr = key->main_addr;
    prover_polynomials.main_addr_shift = key->main_addr.shifted();

    prover_polynomials.main_jump_ptr = key->main_jump_ptr;
    prover_polynomials.main_jump_ptr_shift = key->main_jump_ptr.shifted();

    prover_polynomials.main_r11 = key->main_r11;
    prover_polynomials.main_r11_shift = key->main_r11.shifted();

    prover_polynomials.main_r2 = key->main_r2;
    prover_polynomials.main_r2_shift = key->main_r2.shifted();

    prover_polynomials.main_r3 = key->main_r3;
    prover_polynomials.main_r3_shift = key->main_r3.shifted();

    prover_polynomials.main_m_value = key->main_m_value;
    prover_polynomials.main_m_value_shift = key->main_m_value.shifted();

    prover_polynomials.main_r5 = key->main_r5;
    prover_polynomials.main_r5_shift = key->main_r5.shifted();

    prover_polynomials.main__operation_id = key->main__operation_id;
    prover_polynomials.main__operation_id_shift = key->main__operation_id.shifted();

    prover_polynomials.main__sigma = key->main__sigma;
    prover_polynomials.main__sigma_shift = key->main__sigma.shifted();

    prover_polynomials.main_r4 = key->main_r4;
    prover_polynomials.main_r4_shift = key->main_r4.shifted();

    prover_polynomials.main_m_addr = key->main_m_addr;
    prover_polynomials.main_m_addr_shift = key->main_m_addr.shifted();

    prover_polynomials.main_r6 = key->main_r6;
    prover_polynomials.main_r6_shift = key->main_r6.shifted();

    prover_polynomials.main_first_step = key->main_first_step;
    prover_polynomials.main_first_step_shift = key->main_first_step.shifted();

    // prover_polynomials.lookup_inverses = key->lookup_inverses;
    // key->z_perm = Polynomial(key->circuit_size);
    // prover_polynomials.z_perm = key->z_perm;
}

/**
 * @brief Commit to the wires
 *
 */
template <typename Flavor> void BrilligVMProver_<Flavor>::compute_wire_commitments()
{
    auto wire_polys = key->get_wires();
    auto labels = commitment_labels.get_wires();
    for (size_t idx = 0; idx < wire_polys.size(); ++idx) {
        transcript.send_to_verifier(labels[idx], commitment_key->commit(wire_polys[idx]));
    }
}

/**
 * @brief Add circuit size, public input size, and public inputs to transcript
 *
 */
template <typename Flavor> void BrilligVMProver_<Flavor>::execute_preamble_round()
{
    const auto circuit_size = static_cast<uint32_t>(key->circuit_size);

    transcript.send_to_verifier("circuit_size", circuit_size);
}

/**
 * @brief Compute commitments to the first three wires
 *
 */
template <typename Flavor> void BrilligVMProver_<Flavor>::execute_wire_commitments_round()
{
    auto wire_polys = key->get_wires();
    auto labels = commitment_labels.get_wires();
    for (size_t idx = 0; idx < wire_polys.size(); ++idx) {
        transcript.send_to_verifier(labels[idx], commitment_key->commit(wire_polys[idx]));
    }
}

/**
 * @brief Compute sorted witness-table accumulator
 *
 */
// template <typename Flavor> void BrilligVMProver_<Flavor>::execute_log_derivative_commitments_round()
// {
//     // Compute and add beta to relation parameters
//     auto [beta, gamma] = transcript.get_challenges("beta", "gamma");
//     // TODO(#583)(@zac-williamson): fix Transcript to be able to generate more than 2 challenges per round! oof.
//     auto beta_sqr = beta * beta;
//     relation_parameters.gamma = gamma;
//     relation_parameters.beta = beta;
//     relation_parameters.beta_sqr = beta_sqr;
//     relation_parameters.beta_cube = beta_sqr * beta;
//     relation_parameters.BrilligVM_set_permutation_delta =
//         gamma * (gamma + beta_sqr) * (gamma + beta_sqr + beta_sqr) * (gamma + beta_sqr + beta_sqr + beta_sqr);
//     relation_parameters.BrilligVM_set_permutation_delta =
//     relation_parameters.BrilligVM_set_permutation_delta.invert();
//     // Compute inverse polynomial for our logarithmic-derivative lookup method
//     lookup_library::compute_logderivative_inverse<Flavor, typename Flavor::LookupRelation>(
//         prover_polynomials, relation_parameters, key->circuit_size);
//     transcript.send_to_verifier(commitment_labels.lookup_inverses, commitment_key->commit(key->lookup_inverses));
//     prover_polynomials.lookup_inverses = key->lookup_inverses;
// }

/**
 * @brief Compute permutation and lookup grand product polynomials and commitments
 *
 */
// template <typename Flavor> void BrilligVMProver_<Flavor>::execute_grand_product_computation_round()
// {
//     // Compute permutation grand product and their commitments
//     permutation_library::compute_permutation_grand_products<Flavor>(key, prover_polynomials, relation_parameters);

//     transcript.send_to_verifier(commitment_labels.z_perm, commitment_key->commit(key->z_perm));
// }

/**
 * @brief Run Sumcheck resulting in u = (u_1,...,u_d) challenges and all evaluations at u being calculated.
 *
 */
template <typename Flavor> void BrilligVMProver_<Flavor>::execute_relation_check_rounds()
{
    using Sumcheck = sumcheck::SumcheckProver<Flavor>;

    auto sumcheck = Sumcheck(key->circuit_size, transcript);

    sumcheck_output = sumcheck.prove(prover_polynomials, relation_parameters);
}

/**
 * - Get rho challenge
 * - Compute d+1 Fold polynomials and their evaluations.
 *
 * */
template <typename Flavor> void BrilligVMProver_<Flavor>::execute_univariatization_round()
{
    const size_t NUM_POLYNOMIALS = Flavor::NUM_ALL_ENTITIES;

    // Generate batching challenge ρ and powers 1,ρ,…,ρᵐ⁻¹
    FF rho = transcript.get_challenge("rho");
    std::vector<FF> rhos = pcs::gemini::powers_of_rho(rho, NUM_POLYNOMIALS);

    // Batch the unshifted polynomials and the to-be-shifted polynomials using ρ
    Polynomial batched_poly_unshifted(key->circuit_size); // batched unshifted polynomials
    size_t poly_idx = 0;                                  // TODO(#391) zip
    for (auto& unshifted_poly : prover_polynomials.get_unshifted()) {
        batched_poly_unshifted.add_scaled(unshifted_poly, rhos[poly_idx]);
        ++poly_idx;
    }

    Polynomial batched_poly_to_be_shifted(key->circuit_size); // batched to-be-shifted polynomials
    for (auto& to_be_shifted_poly : prover_polynomials.get_to_be_shifted()) {
        batched_poly_to_be_shifted.add_scaled(to_be_shifted_poly, rhos[poly_idx]);
        ++poly_idx;
    };

    // Compute d-1 polynomials Fold^(i), i = 1, ..., d-1.
    gemini_polynomials = Gemini::compute_gemini_polynomials(
        sumcheck_output.challenge, std::move(batched_poly_unshifted), std::move(batched_poly_to_be_shifted));

    // Compute and add to trasnscript the commitments [Fold^(i)], i = 1, ..., d-1
    for (size_t l = 0; l < key->log_circuit_size - 1; ++l) {
        transcript.send_to_verifier("Gemini:FOLD_" + std::to_string(l + 1),
                                    commitment_key->commit(gemini_polynomials[l + 2]));
    }
}

/**
 * - Do Fiat-Shamir to get "r" challenge
 * - Compute remaining two partially evaluated Fold polynomials Fold_{r}^(0) and Fold_{-r}^(0).
 * - Compute and aggregate opening pairs (challenge, evaluation) for each of d Fold polynomials.
 * - Add d-many Fold evaluations a_i, i = 0, ..., d-1 to the transcript, excluding eval of Fold_{r}^(0)
 * */
template <typename Flavor> void BrilligVMProver_<Flavor>::execute_pcs_evaluation_round()
{
    const FF r_challenge = transcript.get_challenge("Gemini:r");
    gemini_output = Gemini::compute_fold_polynomial_evaluations(
        sumcheck_output.challenge, std::move(gemini_polynomials), r_challenge);

    for (size_t l = 0; l < key->log_circuit_size; ++l) {
        std::string label = "Gemini:a_" + std::to_string(l);
        const auto& evaluation = gemini_output.opening_pairs[l + 1].evaluation;
        transcript.send_to_verifier(label, evaluation);
    }
}

/**
 * - Do Fiat-Shamir to get "nu" challenge.
 * - Compute commitment [Q]_1
 * */
template <typename Flavor> void BrilligVMProver_<Flavor>::execute_shplonk_batched_quotient_round()
{
    nu_challenge = transcript.get_challenge("Shplonk:nu");

    batched_quotient_Q =
        Shplonk::compute_batched_quotient(gemini_output.opening_pairs, gemini_output.witnesses, nu_challenge);

    // commit to Q(X) and add [Q] to the transcript
    transcript.send_to_verifier("Shplonk:Q", commitment_key->commit(batched_quotient_Q));
}

/**
 * - Do Fiat-Shamir to get "z" challenge.
 * - Compute polynomial Q(X) - Q_z(X)
 * */
template <typename Flavor> void BrilligVMProver_<Flavor>::execute_shplonk_partial_evaluation_round()
{
    const FF z_challenge = transcript.get_challenge("Shplonk:z");

    shplonk_output = Shplonk::compute_partially_evaluated_batched_quotient(
        gemini_output.opening_pairs, gemini_output.witnesses, std::move(batched_quotient_Q), nu_challenge, z_challenge);
}
/**
 * - Compute final PCS opening proof:
 * - For KZG, this is the quotient commitment [W]_1
 * - For IPA, the vectors L and R
 * */
template <typename Flavor> void BrilligVMProver_<Flavor>::execute_final_pcs_round()
{
    PCS::compute_opening_proof(commitment_key, shplonk_output.opening_pair, shplonk_output.witness, transcript);
}

template <typename Flavor> plonk::proof& BrilligVMProver_<Flavor>::export_proof()
{
    proof.proof_data = transcript.proof_data;
    return proof;
}

template <typename Flavor> plonk::proof& BrilligVMProver_<Flavor>::construct_proof()
{
    // Add circuit size public input size and public inputs to transcript.
    execute_preamble_round();

    // Compute wire commitments
    execute_wire_commitments_round();

    // TODO: not implemented for codegen just yet
    // Compute sorted list accumulator and commitment
    // execute_log_derivative_commitments_round();

    // Fiat-Shamir: bbeta & gamma
    // Compute grand product(s) and commitments.
    // execute_grand_product_computation_round();

    // Fiat-Shamir: alpha
    // Run sumcheck subprotocol.
    execute_relation_check_rounds();

    // Fiat-Shamir: rho
    // Compute Fold polynomials and their commitments.
    execute_univariatization_round();

    // Fiat-Shamir: r
    // Compute Fold evaluations
    execute_pcs_evaluation_round();

    // Fiat-Shamir: nu
    // Compute Shplonk batched quotient commitment Q
    execute_shplonk_batched_quotient_round();

    // Fiat-Shamir: z
    // Compute partial evaluation Q_z
    execute_shplonk_partial_evaluation_round();

    // Fiat-Shamir: z
    // Compute PCS opening proof (either KZG quotient commitment or IPA opening proof)
    execute_final_pcs_round();

    return export_proof();
}

template class BrilligVMProver_<honk::flavor::BrilligVMFlavor>;

} // namespace proof_system::honk