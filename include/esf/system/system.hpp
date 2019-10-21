#pragma once
#include <esf/dof/dof_mapper.hpp>
#include <esf/index.hpp>
#include <esf/type_traits.hpp>
#include <esf/var.hpp>

#include <esl/dense.hpp>
#include <esu/string.hpp>
#include <esu/type_traits.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace esf
{
template<class Var_list_, template<class> class Dof_mapper_ = Dof_mapper>
class System
{
public:
	static constexpr std::size_t dim = Var_list_::space_dim;
	static constexpr std::size_t n_vars = Var_list_::size;

	using Var_list = Var_list_;
	using Mesh = esf::Mesh<dim>;
	using Dof_mapper = Dof_mapper_<Var_list>;

	template<std::size_t var>
	using Var_t = typename Var_list::template Nth<var>;

	template<std::size_t var>
	using Var_dofs = typename Dof_mapper::template Var_dofs<var>;

	using Vars_dofs = typename Dof_mapper::Vars_dofs;

	using Dofs = std::conditional_t<n_vars == 1, Var_dofs<0>, Vars_dofs>;

	template<std::size_t var>
	using Var_vertex_dofs = typename Dof_mapper::template Var_vertex_dofs<var>;

	template<std::size_t var>
	using Var_edge_dofs = typename Dof_mapper::template Var_edge_dofs<var>;

	template<std::size_t var>
	using Var_cell_dofs = typename Dof_mapper::template Var_cell_dofs<var>;

public:
	System(const Mesh& mesh) : mesh_(mesh)
	{}

	template<class... Args>
	void init(Args&&... args)
	{
		dof_mapper_.init(*this, std::forward<Args>(args)...);
	}

	//////////////////////////////////////////////////////////////////////
	/** Degrees of freedom */

	Index n_dofs() const
	{
		return dof_mapper_.n_dofs();
	}

	Index n_free_dofs() const
	{
		return dof_mapper_.n_free_dofs();
	}

	Index n_const_dofs() const
	{
		return n_dofs() - n_free_dofs();
	}

	const Dof_mapper& dof_mapper() const
	{
		return dof_mapper_;
	}

	// 	template<std::size_t var>
	// 	void vertex_dofs(mesh::Index vertex, Var_vertex_dofs<var>& dofs_list) const
	// 	{
	// 		dof_mapper_.template vertex_dofs<var>(vertex, dofs_list);
	// 	}

	//////////////////////////////////////////////////////////////////////
	//* Variables */

	template<std::size_t i = 0>
	auto& variable(Var_index<i> = {})
	{
		static_assert(i < n_vars);
		return std::get<i>(vars_);
	}

	template<std::size_t i = 0>
	auto& variable(Var_index<i> = {}) const
	{
		static_assert(i < n_vars);
		return std::get<i>(vars_);
	}

	auto& variables() const
	{
		return vars_;
	}

	template<class Fn>
	void for_each_variable(Fn fn) const
	{
		Var_list::for_each([this, &fn](auto var_index) {
			auto& var = variable(var_index);
			if constexpr (std::is_invocable_v<Fn, decltype(var)>)
				fn(var);
			else
				fn(var_index, var);
		});
	}

	///////////////////////////////////////////////////////////////////////

	const auto& mesh() const
	{
		return mesh_;
	}

	template<class Symmetry_tag, class... Args>
	decltype(auto) sparsity_pattern(Args&&... args) const
	{
		return dof_mapper_.template sparsity_pattern<Symmetry_tag>(
			*this, std::forward<Args>(args)...);
	}

	template<class Symmetry_tag, class... Args>
	decltype(auto) sparsity_pattern2(Args&&... args) const
	{
		return dof_mapper_.template sparsity_pattern2<Symmetry_tag>(
			*this, std::forward<Args>(args)...);
	}

	virtual std::string name() const
	{
		return "Unnamed system";
	}

	std::size_t memory_size() const
	{
		return dof_mapper_.memory_size();
	}

private:
	// 	template<std::size_t var = 0>
	// 	void set_constraints()
	// 	{
	// 		using Var = Var_t<var>;
	//
	// 		if constexpr (Var::has_bnd_cond)
	// 		{
	// 			const Var& v = variable<var>();
	// 			for (auto& bc : v.bnd_conds())
	// 			{
	// 				if constexpr (Var::has_dof(Vertex_tag{}))
	// 					set_constraints_at_vertices<var>(bc);
	//
	// 				if constexpr (Var::has_dof(Edge_tag{}))
	// 					set_constraints_at_edges<var>(bc);
	// 			}
	// 		}
	//
	// 		if constexpr (var + 1 < n_vars)
	// 			set_constraints<var + 1>();
	// 	}

	// 	template<std::size_t var, class Bnd_cond>
	// 	void set_constraints_at_vertices(const Bnd_cond& bc)
	// 	{
	// 		static_assert(Var_t<var>::has_dof(Vertex_tag{}), "Variable has no DoFs at vertices");
	//
	// // 		Var_vertex_dofs<var> vertex_dofs;
	// // 	//	la::Matrix_x<Index> vertex_dof_indices;		// TODO : static size
	// //
	// // 		for (auto vertex = bc->begin_vertex(); vertex != bc->end_vertex(); ++vertex)
	// // 		{
	// // 			dof_mapper_.template vertex_dofs<var>(*vertex, vertex_dofs);
	// // 			//vertex_dof_indices.resize(vertex_dofs.rows(), vertex_dofs.cols());
	// // 			for (std::size_t j = 0; j < vertex_dofs.cols(); ++j)
	// // 				for (std::size_t i = 0; i < vertex_dofs.rows(); ++i)
	// // 				{
	// // 					assert(vertex_dofs(i, j).is_free == false);
	// // //					vertex_dof_indices(i, j) = vertex_dofs(i, j).index;
	// // 				}
	// //
	// // 			//bc->set_values_at_vertex(*vertex, solution_.view(vertex_dof_indices));
	// // 			bc->set_values_at_vertex(*vertex, solution_[vertex_dofs(0, 0).index]);
	// // 		}
	// 	}
	//
	// 	template<std::size_t var, class Bnd_cond>
	// 	void set_constraints_at_edges(const Bnd_cond& bc)
	// 	{
	// 		static_assert(Var_t<var>::has_dof(Edge_tag{}), "Variable has no DoFs at edges");
	// //		throw;
	//
	// 		// TODO : Edge direction???
	// // 				for (auto edge = bc->begin_edge(); edge != bc->end_edge(); ++edge)
	// // 				{
	// // 					Dof_index& dof = indices_.at(Edge_tag{}, Var_index<t_var>{}, *edge);
	// // 					assert(!dof.is_free);
	// //
	// // 					// TODO : submatrix in v.degree index and n_dofs(Edge_tag{})
	// // 					bc->get_values_at_edge(*edge, solution_[dof.index]);
	// // 				}
	//
	// 	}

private:
	Dof_mapper dof_mapper_;
	typename Var_list::Tuple vars_;
	const Mesh& mesh_;
};

template<class Var_list, template<class> class Dof_mapper>
std::ostream& operator<<(std::ostream& out, const System<Var_list, Dof_mapper>& system)
{
	out << system.name() << '\n'
		<< "Number of variables: " << system.n_vars << '\n'
		<< "DoFs (free/constrained/total): " << system.n_free_dofs() << '/' << system.n_const_dofs()
		<< '/' << system.n_dofs() << '\n'
		<< "Memory: " << esu::size_string(system.memory_size()) << '\n';

	return out;
}
} // namespace esf
