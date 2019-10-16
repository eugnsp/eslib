#pragma once

#include <cstddef>
#include <type_traits>
#include <utility>

namespace esu
{
template<class T, class... Ts>
struct Are_same : std::conjunction<std::is_same<T, Ts>...>
{};

template<class T, class... Ts>
inline constexpr bool are_same = Are_same<T, Ts...>::value;
} // namespace esu
