#pragma once
#include <esu/numeric/even_odd.hpp>

#include <cassert>
#include <cmath>
#include <type_traits>

namespace esu
{
// Squares the argument
template<typename T>
constexpr T sq(T x)
{
	return x * x;
}

// Cubes the argument
template<typename T>
constexpr T cube(T x)
{
	return x * x * x;
}

// Returns the non-negative n-th power of the argument
template<typename T, typename Integral>
constexpr T int_pow(T x, Integral n)
{
	static_assert(std::is_integral_v<Integral>);
	assert(n >= 0);

	auto result = static_cast<T>(1);
	while (n)
	{
		if (is_odd(n))
			result *= x;
		n >>= 1;
		x *= x;
	}

	return result;
}

template<typename T>
auto hypot(T x, T y)
{
	return std::sqrt(sq(x) + sq(y));
}

template<typename T>
auto hypot(T x, T y, T z)
{
	return std::sqrt(sq(x) + sq(y) + sq(z));
}

template<typename T>
auto cathetus(T hypot, T x)
{
	return std::sqrt(sq(hypot) - sq(x));
}

template<typename T>
auto cathetus(T hypot, T x, T y)
{
	return std::sqrt(sq(hypot) - sq(x) - sq(y));
}
} // namespace esu
