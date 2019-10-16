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
constexpr void tuple_for_each(Fn&& fn, Tuple&& tuple, std::index_sequence<indices...>)
{
	(static_cast<void>(fn(std::get<indices>(std::forward<Tuple>(tuple)))), ...);
}
} // namespace internal

// Applies a function object to first, second, etc. elements of a list tuples
template<class Fn, class... Tuples>
constexpr void tuple_for_each(Fn&& fn, Tuples&&... tuples)
{
	if constexpr (sizeof...(Tuples) == 1)
		return internal::tuple_for_each(
			std::forward<Fn>(fn), std::forward<Tuples>(tuples)...,
			internal::index_sequence_for_tuple<Tuples...>);
	else
		return tuple_for_each(
			internal::forward_with_apply(std::forward<Fn>(fn)),
			tuple_forward_as_zipped(std::forward<Tuples>(tuples)...));
}
} // namespace esu
