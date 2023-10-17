#pragma once
#include "barretenberg/proof_system/arithmetization/arithmetization.hpp"
namespace arithmetization {
class ExampleRelationArithmetization : public Arithmetization<3, 0> {
  public:
    using FF = barretenberg::fr;
    struct Selectors {};
};
} // namespace arithmetization
