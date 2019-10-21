#pragma once
#include <esf/dof/dof_index.hpp>
#include <esf/dof/function.hpp>
#include <esf/dof/mesh_vars_map.hpp>
#include <esf/type_traits.hpp>
#include <esf/var.hpp>

#include <esl/dense.hpp>

#include <cassert>
#include <cstddef>
#include <utility>

template<class>
class TD;

namespace esf::internal
{
template<class Var_list>
class Dof_mapper_base
{
private:
	template<std::size_t size>
	using Dof_index_vector = esl::Vector<Dof_index, size>;

protected:
	template<std::size_t var_idx>
	using Var = typename Var_list::template Nth<var_idx>;

	using Mesh = esf::Mesh<Var_list::space_dim>;

public:
	template<std::size_t var_idx, class Mesh_element_tag>
	using Var_dofs = Dof_index_vector<Var<var_idx>::Element::dofs(Mesh_element_tag{})>;

	template<std::size_t var_idx, class Mesh_element_tag>
	using Var_total_dofs = Dof_index_vector<Var<var_idx>::Element::total_dofs(Mesh_element_tag{})>;

public:
	template<class System>
	void init(const System& system)
	{
		indices_.init_storage(system.mesh());

		compute_n_dofs(system);
		mark_const_dofs(system);
		assign_indices(system);

		assert(indices_.all_of([](auto index) { return index.is_valid(); }));
	}

	template<class Symmetry_tag, class System>
	static auto sparsity_pattern(const System& system)
		-> esl::Sparsity_pattern<Symmetry_tag>
	{
		return esf::internal::sparsity_pattern<Symmetry_tag>(system);
	}

	///////////////////////////////////////////////////////////////////////
	/** Capacity */

	esf::Index n_dofs() const
	{
		return n_dofs_;
	}

	esf::Index n_free_dofs() const
	{
		return n_free_dofs_;
	}

	std::size_t memory_size() const
	{
		return indices_.memory_size();
	}

private:
	////////////////////////////////////////////////////////////////////////
	/** Initialization */

	template<class System>
	void compute_n_dofs(const System& system)
	{
		n_dofs_ = 0;
		for_each_variable_and_element<Var_list>([this, &system](auto var, auto element_tag) {
			const auto n_dofs = system.variable(var).n_dofs(element_tag);
			const auto n_elements = *system.mesh().n_elements(element_tag);
			n_dofs_ += n_dofs * n_elements;
		});
	}

	template<class System>
	void mark_const_dofs(const System& system)
	{
		n_free_dofs_ = n_dofs_;

		for_each_variable<Var_list>([this, &system](auto var) {
			const auto& v = system.variable(var);
			v.for_each_strong_bnd_cond([this, &v, &var](const auto& bc) {
				using Element = typename Var_by_var_index<Var_list, decltype(var)>::Element;

				if constexpr (Element::has_vertex_dofs)
					for (Vertex_index vertex : bc.vertices())
					{
						Dof_index& dof = indices_.at(vertex, var);
						assert(dof.is_free);

						n_free_dofs_ -= v.n_dofs(Vertex_tag{});
						dof.is_free = false;
					}

				if constexpr (Var_list::space_dim == 2 && Element::has_edge_dofs)
					for (Halfedge_index halfedge : bc.halfedges())
					{
						Dof_index& dof = indices_.at(edge(halfedge), var);
						assert(dof.is_free);

						n_free_dofs_ -= v.n_dofs(Edge_tag{});
						dof.is_free = false;
					}
			});
		});
	}

	template<class System>
	void assign_indices(const System& system)
	{
		Index free_index = 0;
		Index const_index = n_free_dofs_;

		for_each_variable_and_element<Var_list>(
			[this, &system, &free_index, &const_index](auto var, auto element_tag) {
				using Element_index = esf::internal::Element_index_by_tag<decltype(element_tag)>;

				for (Element_index ei{}; ei < system.mesh().n_elements(element_tag); ++ei)
				{
					Dof_index& dof = indices_.at(ei, var);

					auto& index = dof.is_free ? free_index : const_index;
					dof.index = index;
					index += system.variable(var).n_dofs(element_tag);
				}
			});
	}

protected:
	Mesh_var_map<Mesh, Var_list, Dof_index> indices_;

	Index n_dofs_ = 0;
	Index n_free_dofs_ = 0;
};
} // namespace esf::internal
