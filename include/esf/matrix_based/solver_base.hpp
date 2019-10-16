#pragma once
#include <esf/matrix_based/solution.hpp>
#include <esf/matrix_based/solution_view.hpp>

#include <esl/dense.hpp>

#include <cstddef>

namespace esf
{
template<class System_, class Linear_solver>
class Matrix_based_solver_base
{
public:
	using System = System_;
	using Mesh = typename System::Mesh;

	using Value = typename Linear_solver::Sparse_matrix::Value;
	using Solution = esf::Solution<System, Value>;
	using Solution_view = esf::Solution_view<System, Value>;

public:
	Matrix_based_solver_base(const Mesh& mesh) : linear_solver_(matrix_), system_(mesh)
	{}

	template<class... Args>
	void init(Args&&... args)
	{
		system_.init(std::forward<Args>(args)...);

		const auto n = system_.n_dofs();
		const auto nf = system_.n_free_dofs();

		matrix_.resize(nf, nf);
		solution_.resize(n);
		rhs_.resize(nf);
	}

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

	Solution solution() const
	{
		return {system_, solution_};
	}

	Solution_view solution_view() const
	{
		return {system_, solution_};
	}

	std::size_t memory_size() const
	{
		return solution_.memory_size() + rhs_.memory_size() + matrix_.memory_size() + system_.memory_size();
	}

protected:
	esl::Vector_x<Value> solution_;
	esl::Vector_x<Value> rhs_;
	typename Linear_solver::Sparse_matrix matrix_;

	Linear_solver linear_solver_;
	System system_;
};
} // namespace esf
