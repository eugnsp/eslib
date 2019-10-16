#pragma once
#include <esf/dof/dof_mapper_base.hpp>
#include <esf/types.hpp>

#include <array>

namespace esf::internal
{
template<class Var_list>
class Dof_mapper<1, Var_list> : public Dof_mapper_base<Var_list>
{
private:
	using Base = Dof_mapper_base<Var_list>;
	using Mesh = typename Base::Mesh;
	using Edge_view = typename Mesh::Edge_view;

	using Vertex_indices = typename Edge_view::Vertex_indices;

	static constexpr std::size_t n_vars = Var_list::size;

public:
	template<std::size_t vi>
	using Var_dofs = typename Base::template Var_dofs<vi>;

	using Vars_dofs = typename Base::Vars_dofs;

	template<std::size_t vi>
	using Var_vertex_dofs = typename Base::template Var_vertex_dofs<vi>;

	template<std::size_t vi>
	using Var_edge_dofs = typename Base::template Var_edge_dofs<vi>;

public:
	template<std::size_t var = 0>
	Var_dofs<var> dofs(const Edge_view& cell) const
	{
		Var_dofs<var> dofs;
		dofs_impl<var>(cell, dofs);
		return dofs;
	}

	Vars_dofs all_dofs(const Edge_view& cell) const
	{
		Vars_dofs dofs_list;
		dofs2(cell, dofs_list);
		return dofs_list;
	}

	template<std::size_t var = 0>
	void vertex_dofs(Vertex_index vertex, Var_vertex_dofs<var>& dofs) const
	{
		var_vertex_dofs<var>(vertex, dofs);
	}

	template<std::size_t var = 0>
	Var_vertex_dofs<var> vertex_dofs(Vertex_index vertex) const
	{
		Var_vertex_dofs<var> dofs;
		var_vertex_dofs<var>(vertex,  dofs);
		return dofs;
	}

private:
	template<std::size_t var>
	void dofs_impl(const Edge_view& edge, Var_dofs<var>& dofs) const
	{
		Vertex_indices vertices;
		edge.get_indices(vertices);

		var_dofs_impl<var>(vertices, *edge, dofs);
	}

	void dofs2(const Edge_view& cell, Vars_dofs& dofs) const
	{
		dofs_impl3(cell, dofs, std::make_index_sequence<n_vars>{});
	}

	template<std::size_t var>
	void var_vertex_dofs(Vertex_index vertex, Var_vertex_dofs<var>& dofs) const
	{
		static_assert(Base::template Var<var>::Element::has_vertex_dofs);

		const Dof_index& first_dof = this->indices_.at(vertex, Var_index<var>{});
		for (Local_index i = 0; i < dofs.size(); ++i)
			dofs[i] = first_dof + i;

		// TODO : implements in terms of assign_dofs
	}

	template<std::size_t... vars>
	void dofs_impl3(
		const typename Mesh::Edge_view& edge, Vars_dofs& dofs, std::index_sequence<vars...>) const
	{
		Vertex_indices vertices;
		edge.get_indices(vertices);

		(var_dofs_impl<vars>(vertices, *edge, std::get<vars>(dofs)), ...);
	}

	template<std::size_t var>
	void var_dofs_impl([[maybe_unused]] const Vertex_indices& vertices,
		[[maybe_unused]] esf::Edge_index edge, Var_dofs<var>& dofs) const
	{
		using Element = typename Base::template Var<var>::Element;

		// TODO : replace indexing with iterator (when iterators are ready in esl::)
		std::size_t i = 0;
		if constexpr (Element::has_vertex_dofs)
			for (const auto vertex : vertices)
				assign_dofs<var>(vertex, dofs, i);

		if constexpr (Element::has_edge_dofs)
			assign_dofs<var>(edge, dofs, i);

		assert(i == dofs.size());
	}

	template<std::size_t var, class Element_index>
	void assign_dofs(Element_index element, Var_dofs<var>& dofs, std::size_t& i) const
	{
		using Element = typename Base::template Var<var>::Element;

		const esf::Dof_index& first_dof = this->indices_.at(element, esf::Var_index<var>{});

		constexpr auto n = Element::n_dofs(esf::internal::Element_tag_by_index<Element_index>{});
		for (esf::Local_index k = 0; k < n; ++k)
			dofs[i++] = first_dof + k;
	}
};
} // namespace esf::internal
