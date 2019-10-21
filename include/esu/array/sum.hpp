#pragma once
#include <array>
#include <cstddef>
#include <type_traits>

namespace esu
{
namespace internal
{
template<typename R, typename T, std::size_t size, std::size_t... indices>
constexpr R array_sum_impl(
	const std::array<T, size>& array, const R& init, std::index_sequence<indices...>)
{
	return (init + ... + array[indices]);
}

template<typename T, std::size_t size, std::size_t... indices>
constexpr T array_sum_impl(const std::array<T, size>& array, std::index_sequence<indices...>)
{
	if constexpr (sizeof...(indices) > 0)
		return (... + array[indices]);
	else
		return T{};
}
} // namespace internal

// Returns the sum of the value `init` and elements in the `array`
template<typename R, typename T, std::size_t size>
constexpr R array_sum(const std::array<T, size>& array, R init)
{
	return internal::array_sum_impl(array, init, std::make_index_sequence<size>{});
}

// Returns the sum of elements in the (array)
template<typename T, std::size_t size>
constexpr T array_sum(const std::array<T, size>& array)
{
	return internal::array_sum_impl(array, std::make_index_sequence<size>{});
}

// Returns the sum of the value `init` and first `n` elements in the `array`
template<std::size_t n, typename R, typename T, std::size_t size>
constexpr R array_sum_n(const std::array<T, size>& array, R init)
{
	static_assert(n <= size);
	return internal::array_sum_impl(array, init, std::make_index_sequence<n>{});
}

// Returns the sum of the first `n` elements in the `array`
template<std::size_t n, typename T, std::size_t size>
constexpr T array_sum_n(const std::array<T, size>& array)
{
	static_assert(n <= size);
	return internal::array_sum_impl(array, std::make_index_sequence<n>{});
}
} // namespace esu
