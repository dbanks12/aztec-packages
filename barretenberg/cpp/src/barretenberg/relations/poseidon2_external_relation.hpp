#pragma once
#include "barretenberg/relations/relation_types.hpp"
namespace proof_system {

template <typename FF_> class Poseidon2ExternalRelationImpl {
  public:
    using FF = FF_;

    static constexpr std::array<size_t, 4> SUBRELATION_PARTIAL_LENGTHS{
        7, // external poseidon2 round sub-relation for first value
        7, // external poseidon2 round sub-relation for second value
        7, // external poseidon2 round sub-relation for third value
        7, // external poseidon2 round sub-relation for fourth value
    };

    /**
     * @brief Expression for the poseidon2 external gate.
     * @details This relation is defined as:
     * q_pos2 * ( (t6 - w_1_shift) + \alpha * (t5 - w_2_shift) +
     * \alpha^2 * (t7 - w_3_shift) + \alpha^3 * (t4 - w_4_shift) ) = 0 where:
     *      v1 := (w_1 + q_1)^5
     *      v2 := (w_2 + q_2)^5
     *      v3 := (w_3 + q_3)^5
     *      v4 := (w_4 + q_4)^5
     *      t0 := v1 + v2                                           (1, 1, 0, 0)
     *      t1 := v3 + v4                                           (0, 0, 1, 1)
     *      t2 := v2 + v2 + t1 = 2 * v2 + v3 + v4                   (0, 2, 1, 1)
     *      t3 := v4 + v4 + t0 = v1 + v2 + 2 * v4                   (1, 1, 0, 2)
     *      t4 := 4 * t1 + t3 = v1 + v2 + 4 * v3 + 6 * v4           (1, 1, 4, 6)
     *      t5 := 4 * t0 + t2 = 4 * v1 + 6 * v2 + v3 + v4           (4, 6, 1, 1)
     *      t6 := t3 + t5 = 5 * v1 + 7 * v2 + 1 * v3 + 3 * v4       (5, 7, 1, 3)
     *      t7 := t2 + t4                                           (1, 3, 5, 7)
     *
     * @param evals transformed to `evals + C(in(X)...)*scaling_factor`
     * @param in an std::array containing the fully extended Univariate edges.
     * @param parameters contains beta, gamma, and public_input_delta, ....
     * @param scaling_factor optional term to scale the evaluation before adding to evals.
     */
    template <typename ContainerOverSubrelations, typename AllEntities, typename Parameters>
    void static accumulate(ContainerOverSubrelations& evals,
                           const AllEntities& in,
                           const Parameters&,
                           const FF& scaling_factor)
    {
        using Accumulator = std::tuple_element_t<0, ContainerOverSubrelations>;
        using View = typename Accumulator::View;
        auto w_l = View(in.w_l);
        auto w_r = View(in.w_r);
        auto w_o = View(in.w_o);
        auto w_4 = View(in.w_4);
        auto w_l_shift = View(in.w_l_shift);
        auto w_r_shift = View(in.w_r_shift);
        auto w_o_shift = View(in.w_o_shift);
        auto w_4_shift = View(in.w_4_shift);
        auto q_l = View(in.q_l);
        auto q_r = View(in.q_r);
        auto q_o = View(in.q_o);
        auto q_4 = View(in.q_4);
        auto q_poseidon2_external = View(in.q_poseidon2_external);

        // add round constants which are loaded in selectors
        auto tmp1 = w_l + q_l;
        auto tmp2 = w_r + q_r;
        auto tmp3 = w_o + q_o;
        auto tmp4 = w_4 + q_4;

        // apply s-box round
        auto v1 = tmp1 * tmp1;
        v1 *= v1;
        v1 *= tmp1;
        auto v2 = tmp2 * tmp2;
        v2 *= v2;
        v2 *= tmp2;
        auto v3 = tmp3 * tmp3;
        v3 *= v3;
        v3 *= tmp3;
        auto v4 = tmp4 * tmp4;
        v4 *= v4;
        v4 *= tmp4;

        // matrix mul with 14 additions
        auto t0 = v1 + v2; // A + B
        auto t1 = v3 + v4; // C + D
        auto t2 = v2 + v2; // 2B
        t2 += t1;          // 2B + C + D
        auto t3 = v4 + v4; // 2D
        t3 += t0;          // 2D + A + B
        auto t4 = t1 + t1;
        t4 += t4;
        t4 += t3; // A + B + 4C + 6D
        auto t5 = t0 + t0;
        t5 += t5;
        t5 += t2;          // 4A + 6B + C + D
        auto t6 = t3 + t5; // 5A + 7B + 3C + D
        auto t7 = t2 + t4; // A + 3B + 5D + 7C

        {
            auto tmp = q_poseidon2_external * (t6 - w_l_shift);
            tmp *= scaling_factor;
            std::get<0>(evals) += tmp;
        }
        {
            auto tmp = q_poseidon2_external * (t5 - w_r_shift);
            tmp *= scaling_factor;
            std::get<1>(evals) += tmp;
        }
        {
            auto tmp = q_poseidon2_external * (t7 - w_o_shift);
            tmp *= scaling_factor;
            std::get<2>(evals) += tmp;
        }
        {
            auto tmp = q_poseidon2_external * (t4 - w_4_shift);
            tmp *= scaling_factor;
            std::get<3>(evals) += tmp;
        }
    };
};

template <typename FF> using Poseidon2ExternalRelation = Relation<Poseidon2ExternalRelationImpl<FF>>;
} // namespace proof_system
