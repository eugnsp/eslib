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

	template<class Fn>
	static void for_each_variable(Fn fn)
	{
		[&fn]<std::size_t... var_idxs>(std::index_sequence<var_idxs...>)
		{
			((void)fn(Var_index<var_idxs>{}), ...);
		}
		(std::make_index_sequence<size>{});
	}

	template<class Fn>
	static void for_each_variable_and_element(Fn fn)
	{
		for_each_variable([&fn]<std::size_t var_idx>(Var_index<var_idx> var_index)
		{
			using Element = typename Var_type<Var_list, var_idx>::Element;
			if constexpr (Element::has_vertex_dofs)
				fn(var_index, Vertex_tag{});
			if constexpr (Element::has_edge_dofs)
				fn(var_index, Edge_tag{});
			if constexpr (Element::has_face_dofs)
				fn(var_index, Face_tag{});
		});
	}
};
} // namespace esf
