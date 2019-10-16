#pragma once
#include <type_traits>

namespace esu
{
// Checks whether (n) is even
template<typename Integral>
constexpr bool is_even(Integral n)
{
	static_assert(std::is_integral_v<Integral>);
	return (n % 2) == 0;
}

// Checks whether (n) is odd
template<typename Integral>
constexpr bool is_odd(Integral n)
{
	return !is_even(n);
}
} // namespace esu
