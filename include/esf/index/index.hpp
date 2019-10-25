#pragma once
#include <cstdint>
#include <utility>

namespace esf
{
using Index = std::uint32_t;
inline constexpr auto invalid_index = static_cast<Index>(-1);
} // namespace esf
