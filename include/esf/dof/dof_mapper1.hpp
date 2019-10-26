#pragma once
#include <esf/dof/dof_mapper_base.hpp>
#include <esf/index.hpp>
#include <esf/tags.hpp>
#include <esf/type_traits.hpp>

#include <cassert>
#include <cstddef>

namespace esf::internal
{
template<class Var_list>
class Dof_mapper<Dim1, Var_list> : public Dof_mapper_base<Var_list>
{
private:
	using Base = Dof_mapper_base<Var_list>;
	using Mesh = typename Base::Mesh;
	using Edge_view = typename Mesh::Edge_view;

	using Vertex_indices = typename Edge_view::Vertex_indices;

	static constexpr std::size_t n_vars = Var_list::size;

public:
	template<std::size_t var_idx, class Mesh_element_tag>
	using Var_dofs = typename Base::template Var_dofs<var_idx, Mesh_element_tag>;

	template<std::size_t var_idx, class Mesh_element_tag = Cell_tag>
    using Var_total_dofs = typename Base::template Var_total_dofs<var_idx, Mesh_element_tag>;

public:
	template<std::size_t var_idx = 0>
	auto dofs(const Edge_view& edge, std::size_t var_dim, Var_index<var_idx> = {}) const
		-> Var_total_dofs<var_idx>
	{
		static_assert(var_idx < n_vars);

		Vertex_indices vertices;
		edge.get_indices(vertices);
		return dofs_impl<var_idx>(vertices, *edge, var_dim);
	}

    template<std::size_t var_idx = 0, class Mesh_element_index,
             class Mesh_element_tag = internal::Element_tag_by_index<Mesh_element_index>>
    auto dofs(Mesh_element_index mesh_element, std::size_t var_dim,
			  Var_index<var_idx> var_index = {}) const
        -> Var_dofs<var_idx, Mesh_element_tag>
    {
        static_assert(var_idx < n_vars);
        static_assert(Base::template Var<var_idx>::Element::has_dofs(Mesh_element_tag{}));

        Var_dofs<var_idx, Mesh_element_tag> dofs;
        const auto& first_dof = this->indices_.at(mesh_element, var_index);

        for (Index i = 0; i < dofs.size(); ++i)
            dofs[i] = first_dof + i * var_dim;

        return dofs;
    }

	// Vars_dofs all_dofs(const Edge_view& edge) const
	// {
	// 	Vars_dofs dofs_list;
	// 	dofs2(edge, dofs_list);
	// 	return dofs_list;
	// }

	// template<std::size_t var = 0>
	// void vertex_dofs(Vertex_index vertex, Var_vertex_dofs<var>& dofs) const
	// {
	// 	var_vertex_dofs<var>(vertex, dofs);
	// }

	// template<std::size_t var = 0>
	// Var_vertex_dofs<var> vertex_dofs(Vertex_index vertex) const
	// {
	// 	Var_vertex_dofs<var> dofs;
	// 	var_vertex_dofs<var>(vertex, dofs);
	// 	return dofs;
	// }

private:
	// void dofs2(const Edge_view& cell, Vars_dofs& dofs) const
	// {
	// 	dofs_impl3(cell, dofs, std::make_index_sequence<n_vars>{});
	// }

	// template<std::size_t var>
	// void var_vertex_dofs(Vertex_index vertex, Var_vertex_dofs<var>& dofs) const
	// {
	// 	static_assert(Base::template Var<var>::Element::has_vertex_dofs);

	// 	const Dof_index& first_dof = this->indices_.at(vertex, Var_index<var>{});
	// 	for (std::size_t i = 0; i < dofs.size(); ++i)
	// 		dofs[i] = first_dof + i;

	// 	// TODO : implements in terms of assign_dofs
	// }

	// template<std::size_t... vars>
	// void dofs_impl3(const typename Mesh::Edge_view& edge, Vars_dofs& dofs,
	// 				std::index_sequence<vars...>) const
	// {
	// 	Vertex_indices vertices;
	// 	edge.get_indices(vertices);

	// 	(var_dofs_impl<vars>(vertices, *edge, std::get<vars>(dofs)), ...);
	// }

	template<std::size_t var_idx>
	auto dofs_impl([[maybe_unused]] const Vertex_indices& vertices,
				   [[maybe_unused]] esf::Edge_index edge, std::size_t var_dim) const
		-> Var_total_dofs<var_idx>
	{
		using Element = typename Var_type<Var_list, var_idx>::Element;

		Var_total_dofs<var_idx> dofs;
		auto dof = dofs.data(); // TODO : replace with .begin()

        const auto assign =	[this, &dof, var_dim]<class Mesh_element_index>(
			                	Mesh_element_index mesh_element_index)
		{
            const auto first = this->indices_.at(mesh_element_index, esf::Var_index<var_idx>{});
            constexpr auto n = Element::dofs(internal::Element_tag_by_index<Mesh_element_index>{});
            for (Index i = 0; i < n; ++i)
                *dof++ = first + i * var_dim;
        };

		if constexpr (Element::has_vertex_dofs)
			for (const auto vertex : vertices)
				assign(vertex);

		if constexpr (Element::has_edge_dofs)
			assign(edge);

		assert(dof == dofs.data() + dofs.size());
	}
};
} // namespace esf::internal
