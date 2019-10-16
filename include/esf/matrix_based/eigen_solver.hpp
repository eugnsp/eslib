#pragma once
#include <esf/matrix_based/eigen_solution_view.hpp>

#include <esl/dense.hpp>

#include <cstddef>
#include <memory>
#include <random>
#include <utility>

#include <esl/sparse.hpp>

namespace esf
{
template<class System_, class Eigen_solver>
class Matrix_based_eigen_solver
{
public:
	using System = System_;
	using Mesh = typename System::Mesh;

	using Value = typename Eigen_solver::Sparse_matrix::Value;
	using Solution_view = Eigen_solution_view<System, Value>;

public:
	// template<std::size_t var>
	// using Solution_view_t = Solution_view<System, var>;

	// template<std::size_t var, class Mesh_el_tag>
	// using Solution_view_t2 = Solution_view2<System, var, Mesh_el_tag>;

	// template<std::size_t var>
	// using Solution_view = Eigen_solution_view<System, var>;

	// template<class Solver, std::size_t var>
	// friend class Solution_view;


public:
	Matrix_based_eigen_solver(const Mesh& mesh) : eigen_solver_(matrix_a_, matrix_b_), system_(mesh)
	{}

	template<class... Args>
	void init(Args&&... args)
	{
		system_.init(std::forward<Args>(args)...);

		const auto nf = system_.n_free_dofs();

		matrix_a_.resize(nf, nf);
		matrix_b_.resize(nf, nf);

		std::random_device rand_dev;
		rand_gen_.seed(rand_dev());
	}

	void solve()
	{
		before_solve();

		//		set_bnd_values();
		assemble();
		after_assemble();

		const auto grow_factor = 1.2;

		const auto size = matrix_a_.rows();
		auto dim = static_cast<std::size_t>(5 + grow_factor * eigen_space_dim());
		const auto old_dim = eigen_values_.size();

		eigen_values_.resize(dim);
		eigen_vectors_.resize(size, dim);

		auto range = eigen_values_range();

		while (true)
		{
			const auto has_initial_guess = (old_dim > 0);
			if (has_initial_guess > 0)
			{
				// Randomize the rest of eigenvectors
				eigen_vectors_.cols_view(old_dim, dim - old_dim) =
					esl::Random_matrix(size, dim - old_dim, std::uniform_real_distribution{}, rand_gen_);
			}

			if (eigen_solver_.solve(eigen_vectors_, eigen_values_, range, has_initial_guess))
				break;

			dim += 5;
			dim *= grow_factor;
		}

		const auto new_dim = eigen_solver_.n_eigen_values();
		eigen_values_.resize(new_dim);
		eigen_vectors_.resize(size, new_dim);

		after_solve();
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

	//template<std::size_t var = 0>
	Solution_view solution_view() const
	{
		return {system(), eigen_values_, eigen_vectors_};
	}

	// 	template<std::size_t var>
	// Solution_view_t<var> solution_view() const
	// {
	// 	debug_check_var_index<var>();
	// 	return {*this};
	// }

	// template<std::size_t var, class Mesh_el_tag>
	// auto solution_view2() const
	// {
	// 	debug_check_var_index<var>();
	// 	return Solution_view_t2<var, Mesh_el_tag>{*this};
	// }

	std::size_t memory_size() const
	{
		return eigen_values_.memory_size() + eigen_vectors_.memory_size() + matrix_a_.memory_size() +
			   matrix_b_.memory_size();
	}

protected:
	// virtual void set_bnd_values() = 0;

	virtual std::pair<double, double> eigen_values_range() const = 0;

	virtual std::size_t eigen_space_dim() const = 0;

	virtual void before_solve()
	{}

	virtual void after_solve()
	{}

	virtual void after_assemble()
	{}

	virtual void assemble() = 0;

protected:
	esl::Vector_xd eigen_values_;
	esl::Matrix_xd eigen_vectors_;

	typename Eigen_solver::Sparse_matrix matrix_a_;
	typename Eigen_solver::Sparse_matrix matrix_b_;

	Eigen_solver eigen_solver_;
	System system_;

private:
	std::mt19937 rand_gen_;
};
} // namespace esf
