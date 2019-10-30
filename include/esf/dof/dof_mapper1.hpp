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
class Dof_mapper_impl<Var_list, Dim1> : public Dof_mapper_base<Var_list>
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
	auto dofs(const Edge_view& edge, std::size_t var_dim, Var_index<var_idx> var_index = {}) const
		-> Var_total_dofs<var_idx>
	{
		static_assert(var_idx < n_vars);
		using Element = typename Var_type<Var_list, var_idx>::Element;

		Vertex_indices vertices;
		edge.get_indices(vertices);

		Var_total_dofs<var_idx> dofs;
		auto dof = dofs.data();

		if constexpr (Element::has_vertex_dofs)
			for (const auto vertex : vertices)
				get_dofs(dof, vertex, var_index, var_dim);

		if constexpr (Element::has_edge_dofs)
			get_dofs(dof, *edge, var_index, var_dim);

		assert(dof == dofs.data() + dofs.size());
		return dofs;
	}

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
private:
	using Base::get_dofs;

	// void dofs2(const Edge_view& cell, Vars_dofs& dofs) const
	// {
	// 	dofs_impl3(cell, dofs, std::make_index_sequence<n_vars>{});
	// }

	// template<std::size_t... vars>
	// void dofs_impl3(const typename Mesh::Edge_view& edge, Vars_dofs& dofs,
	// 				std::index_sequence<vars...>) const
	// {
	// 	Vertex_indices vertices;
	// 	edge.get_indices(vertices);

	// 	(var_dofs_impl<vars>(vertices, *edge, std::get<vars>(dofs)), ...);
	// }
};
} // namespace esf::internal
