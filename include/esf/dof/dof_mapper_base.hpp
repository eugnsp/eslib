#pragma once
#include <esf/dof/dof_index.hpp>
#include <esf/dof/function.hpp>
#include <esf/dof/mesh_vars_map.hpp>
#include <esf/type_traits.hpp>
#include <esf/var.hpp>

#include <esl/dense.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

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

	using Mesh = esf::Mesh<typename Var_list::Space_dim>;

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
	static esl::Sparsity_pattern<Symmetry_tag> sparsity_pattern(const System& system)
	{
		return esf::internal::sparsity_pattern<Symmetry_tag>(system);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////
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
	////////////////////////////////////////////////////////////////////////////////////////////////
	/** Initialization */

	template<class System>
	void compute_n_dofs(const System& system)
	{
		n_dofs_ = 0;
		const auto& mesh = system.mesh();
		system.for_each_variable_and_element(
			[this, &mesh](auto /* var_index */, auto element_tag, const auto& var)
		{
			n_dofs_ += var.dofs(element_tag) * *mesh.n_elements(element_tag);
		});
	}

	template<class System>
	void mark_const_dofs(const System& system)
	{
		n_free_dofs_ = n_dofs_;

		system.for_each_variable(
			[this]<std::size_t var_idx, class Var>(Var_index<var_idx> var_index, const Var& var)
		{
			using Element = typename Var::Element;

			var.for_each_strong_bnd_cond([this, &var, var_index](const auto& bnd_cond)
			{
				if constexpr (Element::has_vertex_dofs)
					for (Vertex_index vertex : bnd_cond.vertices())
					{
						Dof_index& dof = indices_.at(vertex, var_index);
						assert(dof.is_free);

						n_free_dofs_ -= var.dofs(Vertex_tag{});
						dof.is_free = false;
					}

				if constexpr (is_dim2<Var_list> && Element::has_edge_dofs)
					for (Halfedge_index halfedge : bnd_cond.halfedges())
					{
						Dof_index& dof = indices_.at(edge(halfedge), var_index);
						assert(dof.is_free);

						n_free_dofs_ -= var.dofs(Edge_tag{});
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

		const auto& mesh = system.mesh();
		system.for_each_variable_and_element(
			[this, &mesh, &free_index, &const_index](auto var_index, auto element_tag, auto& var)
		{
			const auto n_elements = mesh.n_elements(element_tag);
			using I = std::remove_const_t<decltype(n_elements)>;
			for (I i{}; i < n_elements; ++i)
			{
				Dof_index& dof = indices_.at(i, var_index);
				auto& index = dof.is_free ? free_index : const_index;
				dof.index = index;
				index += var.dofs(element_tag);
			}
		});
	}

protected:
	Mesh_var_map<Mesh, Var_list, Dof_index> indices_;

	Index n_dofs_ = 0;
	Index n_free_dofs_ = 0;
};
} // namespace esf::internal
