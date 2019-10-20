#pragma once
#include <cstdint>
#include <utility>

namespace esf
{
using Index = std::uint32_t;
using Local_index = unsigned int;

inline constexpr auto invalid_index = static_cast<Index>(-1);
inline constexpr auto invalid_local_index = static_cast<Local_index>(-1);

template<Local_index... indices>
using Local_index_sequence = std::integer_sequence<Local_index, indices...>;

template<Local_index n>
using Make_local_index_sequence = std::make_integer_sequence<Local_index, n>;
} // namespace esf
