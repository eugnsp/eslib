#pragma once
#include <esf/dof/dof_mapper_base.hpp>
#include <esf/forward.hpp>
#include <esf/index.hpp>
#include <esf/tags.hpp>
#include <esf/type_traits.hpp>

#include <cassert>
#include <cstddef>

namespace esf::internal
{
template<class Var_list>
class Dof_mapper_impl<Var_list, Dim2> : public Dof_mapper_base<Var_list>
{
private:
	using Base = Dof_mapper_base<Var_list>;
	using Mesh = typename Base::Mesh;
	using Edge_view = typename Mesh::Edge_view;
	using Face_view = typename Mesh::Face_view;

	using Vertex_indices   = typename Face_view::Vertex_indices;
	using Halfedge_indices = typename Face_view::Halfedge_indices;

	static constexpr std::size_t n_vars = Var_list::size;

public:
	template<std::size_t var_idx, class Mesh_element_tag>
	using Var_dofs = typename Base::template Var_dofs<var_idx, Mesh_element_tag>;

	template<std::size_t var_idx, class Mesh_element_tag = Cell_tag>
	using Var_total_dofs = typename Base::template Var_total_dofs<var_idx, Mesh_element_tag>;

public:
	template<std::size_t var_idx = 0>
	auto dofs(const Face_view& face, Halfedge_index first_halfedge, std::size_t var_dim,
		      Var_index<var_idx> var_index = {}) const
		-> Var_total_dofs<var_idx>
	{
		static_assert(var_idx < n_vars);

		Vertex_indices vertices;
		Halfedge_indices halfedges;
		face.get_indices(first_halfedge, vertices, halfedges);
		return dofs_impl(vertices, halfedges, *face, var_index, var_dim);
	}

	template<std::size_t var_idx = 0>
	auto dofs(const Face_view& face, std::size_t var_dim, Var_index<var_idx> var_index = {}) const
		-> Var_total_dofs<var_idx>
	{
		static_assert(var_idx < n_vars);

		Vertex_indices vertices;
		Halfedge_indices halfedges;
		face.get_indices(vertices, halfedges);
		return dofs_impl(vertices, halfedges, *face, var_index, var_dim);
	}

	// Vars_dofs all_dofs(const Face_view& cell) const
	// {
	// 	Vars_dofs dofs_list;
	// 	dofs2(cell, dofs_list);
	// 	return dofs_list;
	// }

	template<std::size_t var_idx = 0, class Mesh_element_index,
		     class Mesh_element_tag = internal::Element_tag_by_index<Mesh_element_index>>
	auto dofs(Mesh_element_index mesh_element_index, std::size_t var_dim,
			  Var_index<var_idx> var_index = {}) const
		-> Var_dofs<var_idx, Mesh_element_tag>
	{
		static_assert(var_idx < n_vars);
		static_assert(Base::template Var<var_idx>::Element::has_dofs(Mesh_element_tag{}));

		Var_dofs<var_idx, Mesh_element_tag> dofs;
		auto dof = dofs.data();
		get_dofs(dof, mesh_element_index, var_index, var_dim);
		assert(dof == dofs.data() + dofs.size());

		return dofs;
	}

	template<std::size_t var_idx = 0>
	auto dofs(Halfedge_index halfedge, std::size_t var_dim, Var_index<var_idx> var_index = {}) const
		-> Var_dofs<var_idx, Edge_tag>
	{
		static_assert(var_idx < n_vars);
		static_assert(Base::template Var<var_idx>::Element::has_edge_dofs);

		Var_dofs<var_idx, Edge_tag> dofs;
		auto dof = dofs.data();
		get_dofs(dof, edge(halfedge), var_index, var_dim, !is_first_halfedge(halfedge));
		assert(dof == dofs.data() + dofs.size());

		return dofs;
	}

private:
	using Base::get_dofs;

	// void dofs2(const Face_view& face, Vars_dofs& dofs) const
	// {
	// 	dofs_impl3(face, dofs, std::make_index_sequence<n_vars>{});
	// }

	// template<std::size_t... vars>
	// void dofs_impl3(const Face_view& cell, Vars_dofs& dofs, std::index_sequence<vars...>) const
	// {
	// 	Vertex_indices vertices;
	// 	Halfedge_indices halfedges;
	// 	cell.get_indices(vertices, halfedges);

	// 	(var_dofs_impl<vars>(vertices, halfedges, *cell, std::get<vars>(dofs)), ...);
	// }

	template<std::size_t var_idx>
	auto dofs_impl([[maybe_unused]] const Vertex_indices& vertices,
				   [[maybe_unused]] const Halfedge_indices& halfedges,
				   [[maybe_unused]] Face_index face,
									Var_index<var_idx> var_index, std::size_t var_dim) const
		-> Var_total_dofs<var_idx>
	{
		using Element = typename Var_type<Var_list, var_idx>::Element;

		Var_total_dofs<var_idx> dofs;
		auto dof = dofs.data();

		if constexpr (Element::has_vertex_dofs)
			for (const auto vertex : vertices)
				get_dofs(dof, vertex, var_index, var_dim);

		if constexpr (Element::has_edge_dofs)
			for (const auto halfedge : halfedges)
				get_dofs(dof, edge(halfedge), var_index, var_dim, !is_first_halfedge(halfedge));

		if constexpr (Element::has_face_dofs)
			get_dofs(dof, face, var_index, var_dim);

		assert(dof == dofs.data() + dofs.size());
		return dofs;
	}
};
} // namespace esf::internal
