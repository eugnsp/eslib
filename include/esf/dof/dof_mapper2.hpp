#pragma once
#include <esf/dof/dof_index.hpp>
#include <esf/dof/dof_mapper_base.hpp>
#include <esf/type_traits.hpp>
#include <esf/types.hpp>
#include <esf/util/algorithm.hpp>
#include <esf/util/mesh_vars_map.hpp>

#include <esu/type_traits.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>

namespace esf::internal
{
template<class Var_list>
class Dof_mapper<2, Var_list> : public Dof_mapper_base<Var_list>
{
private:
	using Base = internal::Dof_mapper_base<Var_list>;
	using Mesh = typename Base::Mesh;
	using Edge_view = typename Mesh::Edge_view;
	using Face_view = typename Mesh::Face_view;

	using Vertex_indices = typename Face_view::Vertex_indices;
	using Halfedge_indices = typename Face_view::Halfedge_indices;

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
	// 	template<std::size_t var>
	// 	Var_edge_dofs<var> dofs(const typename Mesh::Halfedge_view& edge) const
	// 	{
	// 		Var_edge_dofs<var> dofs;
	// 		var_edge_dofs<var>(*edge, dofs);
	// 		return dofs;
	// 	}

	template<std::size_t var = 0>
	Var_dofs<var> dofs(const Face_view& cell, Halfedge_index first_halfedge) const
	{
		Var_dofs<var> dofs;
		dofs_impl<var>(cell, first_halfedge, dofs);
		return dofs;
	}

	template<std::size_t var = 0>
	Var_dofs<var> dofs(const Face_view& cell) const
	{
		Var_dofs<var> dofs;
		dofs_impl<var>(cell, dofs);
		return dofs;
	}

	Vars_dofs all_dofs(const Face_view& cell) const
	{
		Vars_dofs dofs_list;
		dofs2(cell, dofs_list);
		return dofs_list;
	}

	template<std::size_t var = 0>
	void vertex_dofs(Vertex_index vertex, Var_vertex_dofs<var>& dofs_list) const
	{
		var_vertex_dofs<var>(vertex, dofs_list);
	}

	template<std::size_t var = 0>
	void edge_dofs(Edge_index edge, Var_edge_dofs<var>& dofs_list) const
	{
		var_edge_dofs<var>(edge, dofs_list);
	}

private:
	template<std::size_t var>
	void dofs_impl(const Face_view& face, Var_dofs<var>& dofs) const
	{
		Vertex_indices vertices;
		Halfedge_indices halfedges;
		face.get_indices(vertices, halfedges);

		var_dofs_impl<var>(vertices, halfedges, *face, dofs);
	}

	template<std::size_t var>
	void dofs_impl(const Face_view& face, Halfedge_index first_halfedge, Var_dofs<var>& dofs) const
	{
		Vertex_indices vertices;
		Halfedge_indices halfedges;
		face.get_indices(first_halfedge, vertices, halfedges);

		var_dofs_impl<var>(vertices, halfedges, *face, dofs);
	}

	void dofs2(const Face_view& face, Vars_dofs& dofs) const
	{
		dofs_impl3(face, dofs, std::make_index_sequence<n_vars>{});
	}

	template<std::size_t var, class Dofs>
	void var_vertex_dofs(Vertex_index vertex, Dofs& dofs) const
	{
		static_assert(Base::template Var<var>::Element::has_vertex_dofs);

		const Dof_index& first_dof = this->indices_.at(vertex, Var_index<var>{});
		for (Local_index i = 0; i < dofs.size(); ++i)
			dofs[i] = first_dof + i;
	}

	template<std::size_t var, class Dofs>
	void var_edge_dofs(Edge_index edge, Dofs& dofs) const
	{
		static_assert(Base::template Var<var>::Element::has_edge_dofs);

		const Dof_index& first_dof = this->indices_.at(edge, Var_index<var>{});
		for (std::size_t i = 0; i < dofs.size(); ++i)
			dofs[i] = first_dof + i;
	}

	template<std::size_t... vars>
	void dofs_impl3(const Face_view& cell, Vars_dofs& dofs, std::index_sequence<vars...>) const
	{
		Vertex_indices vertices;
		Halfedge_indices halfedges;
		cell.get_indices(vertices, halfedges);

		(var_dofs_impl<vars>(vertices, halfedges, *cell, std::get<vars>(dofs)), ...);
	}

	template<std::size_t var>
	void var_dofs_impl([[maybe_unused]] const Vertex_indices& vertices,
		[[maybe_unused]] const Halfedge_indices& halfedges, [[maybe_unused]] Face_index face,
		Var_dofs<var>& dofs) const
	{
		using Element = typename Base::template Var<var>::Element;

		// TODO : replace indexing with iterator (when iterators are ready in esl::)
		std::size_t i = 0;

		if constexpr (Element::has_vertex_dofs)
			for (const auto vertex : vertices)
				assign_dofs<var>(vertex, dofs, i);

		if constexpr (Element::has_edge_dofs)
			for (const auto halfedge : halfedges)
				assign_dofs<var>(edge(halfedge), dofs, i, is_first_halfedge(halfedge));

		if constexpr (Element::has_face_dofs)
			assign_dofs<var>(face, dofs, i);

		assert(i == dofs.size());
	}

	template<std::size_t var, class Element_index>
	void assign_dofs(
		Element_index element, Var_dofs<var>& dofs, std::size_t& i, bool reversed = false) const
	{
		using Element = typename Base::template Var<var>::Element;

		const esf::Dof_index& first_dof = this->indices_.at(element, esf::Var_index<var>{});
		constexpr auto n = Element::n_dofs(esf::internal::Element_tag_by_index<Element_index>{});

		for (esf::Local_index k = 0; k < n; ++k)
			dofs[i++] = first_dof + (reversed ? n - k - 1 : k);
	}
};
} // namespace esf::internal
