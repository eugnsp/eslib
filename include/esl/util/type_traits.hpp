#pragma once
#include <type_traits>

namespace math_la::internal
{
template<bool...>
struct Bool_pack;

// Checks whether all bool values are true (the value is true if the pack is empty)
template<bool... t_values>
using All = std::is_same<Bool_pack<true, t_values...>, Bool_pack<t_values..., true>>;

template<bool... t_values>
constexpr bool all_v = All<t_values...>::value;

} // namespace math_la::internal
