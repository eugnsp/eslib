#pragma once
#include <cassert>
#include <climits>
#include <cstddef>
#include <type_traits>

namespace esu
{
// Returns the number of consecutive 0 bits in the value of non-zero x,
// starting from the most significant bit
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T> &&
												 !std::is_same_v<T, bool>>>
inline std::size_t count_left_zeros_nz(T x)
{
	assert(x > 0);

	if constexpr (std::is_same_v<T, unsigned long long>)
		return static_cast<std::size_t>(__builtin_clzll(x));
	else if constexpr (std::is_same_v<T, unsigned long>)
		return static_cast<std::size_t>(__builtin_clzl(x));
	else if constexpr (sizeof(T) <= sizeof(unsigned int))
		return static_cast<std::size_t>(
			__builtin_clz(x) - CHAR_BIT * (sizeof(unsigned int) - sizeof(T)));
}

// Returns the number of consecutive 0 bits in the value of x,
// starting from the most significant bit
template<typename T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T> &&
												 !std::is_same_v<T, bool>>>
std::size_t count_left_zeros(T x)
{
	return x > 0 ? count_left_zeros_nz(x) : CHAR_BIT * sizeof(T);
}
} // namespace esu
