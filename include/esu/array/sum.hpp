#pragma once
#include <array>
#include <cstddef>
#include <type_traits>

namespace esu
{
namespace internal
{
template<typename T, typename S, std::size_t size, std::size_t... indices>
constexpr T array_sum_impl(
	const std::array<S, size>& array, const T& init, std::index_sequence<indices...>)
{
	return (init + ... + array[indices]);
}
} // namespace internal

// Returns the sum of the given value (init) and elements in the (array)
template<typename T, typename S, std::size_t size>
constexpr T array_sum(const std::array<S, size>& array, T init)
{
	return internal::array_sum_impl(array, init, std::make_index_sequence<size>{});
}

// Returns the sum of the given value (init) and first (n) elements in the (array)
template<std::size_t n, typename T, typename S, std::size_t size>
constexpr T array_sum_n(const std::array<S, size>& array, T init)
{
	static_assert(n <= size);

	return internal::array_sum_impl(array, init, std::make_index_sequence<n>{});
}
} // namespace esu
