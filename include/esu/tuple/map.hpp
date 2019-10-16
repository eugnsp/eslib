#pragma once
#include <esu/tuple/core.hpp>
#include <esu/tuple/forward_as_zipped.hpp>

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace esu
{
namespace internal
{
template<class Fn, class Tuple, std::size_t... indices>
constexpr auto tuple_map(Fn&& fn, Tuple&& tuple, std::index_sequence<indices...>)
{
	// See: https://stackoverflow.com/questions/51727961/what-should-tuple-map-return

	using Ret = std::tuple<
		std::invoke_result_t<Fn, decltype(std::get<indices>(std::forward<Tuple>(tuple)))>...>;

	return Ret{fn(std::get<indices>(std::forward<Tuple>(tuple)))...};
}
} // namespace internal

// Applies a function object to first, second, etc. elements
// of a list tuples and returns a tuple of results
template<class Fn, class... Tuples>
constexpr auto tuple_map(Fn&& fn, Tuples&&... tuples)
{
	static_assert(sizeof...(Tuples) > 0);

	if constexpr (sizeof...(Tuples) == 1)
		if constexpr (tuple_size<Tuples...> == 0)
			return std::make_tuple();
		else
			return internal::tuple_map(
				std::forward<Fn>(fn), std::forward<Tuples>(tuples)...,
				internal::index_sequence_for_tuple<Tuples...>);
	else
		return tuple_map(
			internal::forward_with_apply(std::forward<Fn>(fn)),
			tuple_forward_as_zipped(std::forward<Tuples>(tuples)...));
}
} // namespace esu
