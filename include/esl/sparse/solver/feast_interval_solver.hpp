#pragma once
#include <esl/dense.hpp>
#include <esl/sparse.hpp>
#include <esl/sparse/solver/feast_interval_solver_base.hpp>
#include <esl/sparse/solver/pardiso_solver.hpp>

#include <mkl_types.h>

#include <cassert>
#include <stdexcept>
#include <utility>

namespace esl
{
template<class Sparse_matrix>
class Feast_interval_solver;

template<typename Value, class Symmetry_tag, typename Index>
class Feast_interval_solver<Csr_matrix<Value, Symmetry_tag, Index>> : private internal::Feast_interval_solver_base
{
	static_assert(internal::is_symmetric<Symmetry_tag>);

public:
	using Sparse_matrix = Csr_matrix<Value, Symmetry_tag, Index>;

private:
	using Real = internal::Remove_complex<Value>;
	using Complex = internal::Add_complex<Value>;

public:
	Feast_interval_solver(const Sparse_matrix& mat_a, const Sparse_matrix& mat_b) : mat_a_(mat_a), mat_b_(mat_b)
	{
		assert(mat_a.rows() == mat_a.cols());
		assert(mat_a.rows() == mat_b.rows() && mat_a.cols() == mat_b.cols());
	}

	bool solve(Matrix_x<Value>& vectors, Vector_x<Real>& values, std::pair<Real, Real> values_interval,
		bool user_init_guess = false)
	{
		assert(values_interval.first < values_interval.second);

		params_.set_user_init_guess(user_init_guess);

		const auto n = static_cast<MKL_UINT>(mat_a_.rows());
		/*const*/ auto m0 = static_cast<MKL_UINT>(values.size());

		MKL_UINT n_loops = 0;
		n_eigen_values_ = 0;
		Real eps = 0;
		Vector_x<Real> rel_errors(m0, 0.);

		Matrix_x<Value> work1(n, m0);
		Matrix_x<Complex> work2(n, m0), work5(n, m0);
		Vector_x<Value> work3(m0 * m0), work4(m0 * m0);

		Mkl_sparse_matrix_ext mkl_mat_a(mat_a_), mkl_mat_b(mat_b_);

		using Z = Csr_matrix<Complex, Symmetric_upper>;
		Z mat_z;
		mat_z.assign_pattern(mat_a_);

		Pardiso_solver solver(mat_z);

		auto job = Job::INIT;
		while (true)
		{
			Complex ze;

			const auto status = mkl_feast_rci(job, n, ze, work1.data(), work2.data(), work3.data(), work4.data(), eps,
				n_loops, values_interval, m0, values.data(), vectors.data(), n_eigen_values_, rel_errors.data());

			if (job == Job::DONE)
			{
				if (status == Status::WARN_SUBSPACE_TOO_SMALL)
					return false;
				else if (status == Status::SUCCESS)
					return true;
				else
					throw std::runtime_error(error_string(status));
			}

			if (status != Status::SUCCESS)
				throw std::runtime_error(error_string(status));

			if (job == Job::FACTORIZE_Z)
			{
				for (MKL_UINT i = 0; i < mat_z.nnz(); ++i)
					mat_z[i] = ze * mat_b_[i] - mat_a_[i];
				solver.analyze_factorize();
			}
			else if (job == Job::SOLVE_Z)
			{
				solver.solve(work2, work5);
				work2 = work5;
			}
			else if (job == Job::MULTIPLY_A)
			{
				const auto start_col = params_.q_start_col();
				const auto cols = params_.q_cols();
				work1.cols_view(start_col, cols) = mkl_mat_a * vectors.cols_view(start_col, cols);
			}
			else if (job == Job::MULTIPLY_B)
			{
				const auto start_col = params_.q_start_col();
				const auto cols = params_.q_cols();
				work1.cols_view(start_col, cols) = mkl_mat_b * vectors.cols_view(start_col, cols);
			}
		}
	}

	std::size_t n_eigen_values() const
	{
		return static_cast<std::size_t>(n_eigen_values_);
	}

private:
	const Sparse_matrix& mat_a_;
	const Sparse_matrix& mat_b_;

	MKL_UINT n_eigen_values_;
};
} // namespace esl
