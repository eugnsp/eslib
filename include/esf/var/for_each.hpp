#pragma once
#include <esf/mesh/tags.hpp>
#include <esf/type_traits.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace esf
{
namespace internal
{
template<std::size_t... is, class Fn>
void for_each_variable_impl(std::index_sequence<is...>, Fn fn)
{
	(static_cast<void>(fn(Var_index<is>{})), ...);
}

template<class Var_list, std::size_t i, class Element_tag, class Fn>
void for_each_variable_and_element_impl([[maybe_unused]] Fn fn)
{
	if constexpr (Var_list::template Nth<i>::Element::has_dofs(Element_tag{}))
		fn(Var_index<i>{}, Element_tag{});
}

template<class Var_list, std::size_t i, class... Element_tags, class Fn>
void for_each_variable_and_element_impl2(Fn fn)
{
	(for_each_variable_and_element_impl<Var_list, i, Element_tags>(fn), ...);

	if constexpr (i + 1 < Var_list::size)
		for_each_variable_and_element_impl2<Var_list, i + 1, Element_tags...>(fn);
}
} // namespace internal

template<class Var_list, class Fn>
void for_each_variable(Fn fn)
{
	internal::for_each_variable_impl(std::make_index_sequence<Var_list::size>{}, fn);
}

template<class Var_list, class Fn>
void for_each_variable_and_element(Fn fn)
{
	constexpr auto space_dim = Var_list::space_dim;
	static_assert(space_dim == 1 || space_dim == 2);

	if constexpr (space_dim == 1)
		internal::for_each_variable_and_element_impl2<Var_list, 0, Vertex_tag, Edge_tag>(fn);

	if constexpr (space_dim == 2)
		internal::for_each_variable_and_element_impl2<Var_list, 0, Vertex_tag, Edge_tag, Face_tag>(
			fn);
}
} // namespace esf
