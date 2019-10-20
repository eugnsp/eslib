#pragma once
#include <esf/type_traits.hpp>

#include <esu/type_traits.hpp>

#include <cstddef>
#include <tuple>

namespace esf
{
// A variables list class used to represent a collection of variables as a single type
template<class... Variables>
struct Var_list
{
	static_assert(esu::all_same<Variables::space_dim...>);

	// Returns the number of variables in the list
	static constexpr std::size_t size = sizeof...(Variables);

	// Returns the space dimension of variables
	static constexpr auto space_dim = esu::Head<Variables...>::space_dim;

	// Returns the type of a variable in the list with the given index
	template<std::size_t i>
	using Nth = esu::Nth<i, Variables...>;

	template<template<class Variable> class Fn>
	using Tuple_map = std::tuple<Fn<Variables>...>;

	// Returns the list of variables as an `std::tuple`
	using Tuple = Tuple_map<esu::Type_identity>;

	template<class Fn>
	static void for_each(Fn fn)
	{
		[&fn]<std::size_t... is>(std::index_sequence<is...>)
		{
			(static_cast<void>(fn(Var_index<is>{})), ...);
		}
		(std::make_index_sequence<size>{});
	}
};

namespace internal
{
template<class Var_list, std::size_t i>
using Nth_variable = typename Var_list::template Nth<i>;
}
} // namespace esf
