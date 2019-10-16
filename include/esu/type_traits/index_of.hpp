#pragma once
#include <cstddef>
#include <type_traits>

namespace esu
{
template<typename T, typename... Ts>
struct Index_of : std::integral_constant<std::size_t, 0>
{};

template<typename T, typename... Ts>
struct Index_of<T, T, Ts...> : std::integral_constant<std::size_t, 0>
{};

template<typename T, typename Not_T, typename... Ts>
struct Index_of<T, Not_T, Ts...> :
	std::integral_constant<std::size_t, 1 + Index_of<T, Ts...>::value>
{};

template<typename T, typename... Ts>
inline constexpr std::size_t index_of = Index_of<T, Ts...>::value;
} // namespace esu
