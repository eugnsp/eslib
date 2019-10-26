#pragma once
#include <esf/solution_view.hpp>
#include <esf/system.hpp>
#include <esf/type_traits.hpp>

#include <esl/dense.hpp>

#include <cstddef>

namespace esf
{
template<class System_, class Linear_solver>
class Matrix_based_solver_base
{
private:
	using Value  = typename Linear_solver::Sparse_matrix::Value;

public:
	using System = System_;
	using Mesh   = typename System::Mesh;

	template<std::size_t var>
	using Solution_view = esf::Solution_view<System, var, Value>;

public:
	Matrix_based_solver_base(const Mesh& mesh)
		: linear_solver_(matrix_), system_(mesh)
	{}

	System& system()
	{
		return system_;
	}

	const System& system() const
	{
		return system_;
	}

	const Mesh& mesh() const
	{
		return system_.mesh();
	}

	const esl::Vector_x<Value>& solution() const
	{
		return solution_;
	}

	void set_solution(const esl::Vector_x<Value>& solution)
	{
		solution_ = solution;
	}

	template<std::size_t var = 0>
	Solution_view<var> solution_view() const
	{
		return {system_, solution_};
	}

	template<std::size_t var = 0>
	Solution_view<var> solution_view(const esl::Vector_x<Value>& solution) const
	{
		return {system_, solution};
	}

	std::size_t memory_size() const
	{
		return solution_.memory_size() +
			   rhs_.memory_size() +
			   matrix_.memory_size() +
			   system_.memory_size();
	}

protected:
	template<class... Args>
	void init(Args&&... args)
	{
		system_.init(std::forward<Args>(args)...);

		const auto n  = system_.n_dofs();
		const auto nf = system_.n_free_dofs();

		matrix_.resize(nf, nf);
		solution_.resize(n);
		rhs_.resize(nf);
	}

	void set_bnd_values()
	{
		system_.for_each_variable(
			[this]<std::size_t vi, class Var>(Var_index<vi> var_index, const Var& var)
		{
			using Element = typename Var::Element;

			var.for_each_strong_bnd_cond([this, var_index](const auto& bnd_cond)
			{
				if constexpr (Element::has_vertex_dofs)
					for (auto& vertex : bnd_cond.vertices())
					{
						const auto vertex_dofs = dofs(system_, vertex, var_index);
						for (std::size_t i = 0; i < vertex_dofs.size(); ++i)
						{
							assert(!vertex_dofs[i].is_free);
							solution_[vertex_dofs[i].index] = bnd_cond.value(vertex, i);
						}
					}

				if constexpr (internal::is_dim2<System> && Element::has_edge_dofs)
					for (auto& halfedge : bnd_cond.halfedges())
					{
						const auto halfedge_dofs = dofs(system_, halfedge, var_index);
						for (std::size_t i = 0; i < halfedge_dofs.size(); ++i)
						{
							assert(!halfedge_dofs[i].is_free);
							solution_[halfedge_dofs[i].index] = bnd_cond.value(halfedge, i);
						}
					}
			});
		});
	}

protected:
	esl::Vector_x<Value> solution_;
	esl::Vector_x<Value> rhs_;
	typename Linear_solver::Sparse_matrix matrix_;

	Linear_solver linear_solver_;
	System system_;
};
} // namespace esf
