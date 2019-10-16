#pragma once
#include <type_traits>

namespace esu
{
template<typename...>
struct Always_false : std::false_type
{};

template<typename... Ts>
inline constexpr bool always_false = Always_false<Ts...>::value;
} // namespace esu
