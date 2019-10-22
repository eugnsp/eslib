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
	static_assert(esu::are_same<typename Variables::Space_dim...>);

	using Space_dim = typename esu::Head<Variables...>::Space_dim;
	static constexpr std::size_t size = sizeof...(Variables);

	// Returns the type of a variable in the list with the given index
	template<std::size_t i>
	using Nth = esu::Nth<i, Variables...>;

	template<template<class Variable> class Fn>
	using Tuple_map = std::tuple<Fn<Variables>...>;

	// Returns the list of variables as an `std::tuple`
	using Tuple = Tuple_map<esu::Type_identity>;
};

namespace internal
{
template<class Var_list, std::size_t i>
using Nth_variable = typename Var_list::template Nth<i>;
}
} // namespace esf
