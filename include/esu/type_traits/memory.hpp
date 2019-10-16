#pragma once
#include <type_traits>

namespace esu
{
template<typename T>
inline constexpr bool is_trivially_relocatable =
	std::is_trivially_move_constructible_v<T> && std::is_trivially_destructible_v<T>;

template<typename T>
inline constexpr bool is_memcpy_copyable =
	std::is_trivially_copy_constructible_v<T> && std::is_trivially_copy_assignable_v<T>;
} // namespace esu
