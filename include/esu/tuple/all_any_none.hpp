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
template<class Pred, class Tuple, std::size_t... indices>
constexpr bool tuple_all_of(Pred pred, Tuple&& tuple, std::index_sequence<indices...>)
{
	return (... && pred(std::get<indices>(std::forward<Tuple>(tuple))));
}

template<class Pred, class Tuple, std::size_t... indices>
constexpr bool tuple_any_of(Pred pred, Tuple&& tuple, std::index_sequence<indices...>)
{
	return (... || pred(std::get<indices>(std::forward<Tuple>(tuple))));
}

template<class Pred, class Tuple, std::size_t... indices>
constexpr bool tuple_none_of(Pred pred, Tuple&& tuple, std::index_sequence<indices...>)
{
	return (... && !pred(std::get<indices>(std::forward<Tuple>(tuple))));
}
} // namespace internal

// Checks whether a predicate returns `true` for all
// first, second, etc. elements of a list tuples
template<class Pred, class... Tuples>
constexpr bool tuple_all_of(Pred pred, Tuples&&... tuples)
{
	if constexpr (sizeof...(Tuples) == 0)
		return true;
	else if constexpr (sizeof...(Tuples) == 1)
		return internal::tuple_all_of(
			pred, std::forward<Tuples>(tuples)..., internal::index_sequence_for_tuple<Tuples...>);
	else
		return tuple_all_of(
			internal::forward_with_apply(pred),
			tuple_forward_as_zipped(std::forward<Tuples>(tuples)...));
}

// Checks whether a predicate returns `true` for any
// first, second, etc. elements of a list tuples
template<class Pred, class... Tuples>
constexpr bool tuple_any_of(Pred pred, Tuples&&... tuples)
{
	if constexpr (sizeof...(Tuples) == 0)
		return false;
	else if constexpr (sizeof...(Tuples) == 1)
		return internal::tuple_any_of(
			pred, std::forward<Tuples>(tuples)..., internal::index_sequence_for_tuple<Tuples...>);
	else
		return tuple_any_of(
			internal::forward_with_apply(pred),
			tuple_forward_as_zipped(std::forward<Tuples>(tuples)...));
}

// Checks whether a predicate returns `true` for none
// first, second, etc. elements of a list tuples
template<class Pred, class... Tuples>
constexpr bool tuple_none_of(Pred pred, Tuples&&... tuples)
{
	if constexpr (sizeof...(Tuples) == 0)
		return true;
	else if constexpr (sizeof...(Tuples) == 1)
		return internal::tuple_none_of(
			pred, std::forward<Tuples>(tuples)..., internal::index_sequence_for_tuple<Tuples...>);
	else
		return tuple_none_of(
			internal::forward_with_apply(pred),
			tuple_forward_as_zipped(std::forward<Tuples>(tuples)...));
}
} // namespace esu
