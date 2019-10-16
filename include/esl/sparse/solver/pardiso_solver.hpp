#pragma once
#include <esl/dense/type_traits.hpp>
#include <esl/sparse/solver/pardiso_solver_base.hpp>
#include <esl/sparse/type_traits.hpp>

#include <mkl_pardiso.h>
#include <mkl_types.h>

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <string>

#include <iostream>

namespace esl
{
template<class Sparse_matrix_, bool is_positive_definite_ = false>
class Pardiso_solver : private internal::Pardiso_solver_base
{
public:
	using Sparse_matrix = Sparse_matrix_;

private:
	using Value = typename Sparse_matrix::Value;
	using Symmetry_tag = typename Sparse_matrix_::Symmetry_tag;

	static_assert(internal::is_fd_or_cfd<Value>);

public:
	Pardiso_solver(const Sparse_matrix& matrix) : matrix_(matrix)
	{
		constexpr auto matrix_type = static_cast<MKL_INT>(pardiso_matrix_type());
		::pardisoinit(handle_, &matrix_type, params_);

		params_.set_zero_based_indexing();
#ifndef NDEBUG
		params_.set_matrix_checker();
#endif
	}

	~Pardiso_solver()
	{
		call_pardiso(Phase::RELEASE_ALL);
	}

	void analyze()
	{
		call_pardiso(Phase::ANALYZE);
	}

	void analyze_factorize()
	{
		call_pardiso(Phase::ANALYZE_FACTORIZE);
	}

	template<class Dense_matrix>
	void analyze_factorize_solve(const Dense_matrix& rhs, Dense_matrix& solution)
	{
		assert(rhs.rows() == solution.rows() && rhs.cols() == solution.cols());

		const auto n_rhs = is_col_major<Dense_matrix> ? rhs.cols() : rhs.rows();
		call_pardiso(Phase::ANALYZE_FACTORIZE_SOLVE, rhs.data(), solution.data(), n_rhs);
	}

	void factorize()
	{
		call_pardiso(Phase::FACTORIZE);
	}

	template<class Dense_matrix>
	void factorize_solve(const Dense_matrix& rhs, Dense_matrix& solution)
	{
		assert(rhs.rows() == solution.rows() && rhs.cols() == solution.cols());

		const auto n_rhs = is_col_major<Dense_matrix> ? rhs.cols() : rhs.rows();
		call_pardiso(Phase::FACTORIZE_SOLVE, rhs.data(), solution.data(), n_rhs);
	}

	template<class Dense_matrix>
	void solve(const Dense_matrix& rhs, Dense_matrix& solution)
	{
		assert(rhs.rows() == solution.rows() && rhs.cols() == solution.cols());

		const auto n_rhs = is_col_major<Dense_matrix> ? rhs.cols() : rhs.rows();
		call_pardiso(Phase::SOLVE, rhs.data(), solution.data(), n_rhs);
	}

private:
	void call_pardiso(
		Phase phase, const Value* const rhs = nullptr, Value* const solution = nullptr, const std::size_t n_rhss = 1)
	{
		constexpr auto matrix_type = static_cast<MKL_INT>(pardiso_matrix_type());
		const auto n_rhs = static_cast<MKL_INT>(n_rhss);
		const MKL_INT max_factors = 1;
		const MKL_INT matrix_number = 1;
		const MKL_INT n_equations = matrix_.cols();

		MKL_INT error = 0;
		::pardiso(handle_, &max_factors, &matrix_number, &matrix_type, reinterpret_cast<const MKL_INT*>(&phase),
			&n_equations, matrix_.values(), reinterpret_cast<const MKL_INT*>(matrix_.row_indices()),
			reinterpret_cast<const MKL_INT*>(matrix_.col_indices()), nullptr, &n_rhs, params_, &message_level_,
			const_cast<Value*>(rhs), solution, &error);

		if (error)
			throw std::runtime_error(pardiso_error_string(error));
	}

	static constexpr Matrix_type pardiso_matrix_type()
	{
		if constexpr (!internal::is_cfd<Value>)
		{
			if constexpr (internal::is_symmetric<Sparse_matrix>)
				return is_positive_definite_ ? Matrix_type::REAL_SYMMETRIC_POS_DEFINITE
											 : Matrix_type::REAL_SYMMETRIC_INDEFINITE;
			else if constexpr (internal::is_structural_symmetric<Sparse_matrix>)
				return Matrix_type::REAL_STRUCTURAL_SYMMETRIC;
			else
				return Matrix_type::REAL_GENERAL;
		}
		else
		{
			if constexpr (internal::is_symmetric<Sparse_matrix>)
				return Matrix_type::COMPLEX_SYMMETRIC;
			else if constexpr (internal::is_structural_symmetric<Sparse_matrix>)
				return Matrix_type::COMPLEX_STRUCTURAL_SYMMETRIC;
			else
				return Matrix_type::COMPLEX_GENERAL;
		}

		// if constexpr (std::is_same_v<Symmetry_tag, Symmetric_upper>)
		// {
		// 	if constexpr (is_complex)
		// 		return Matrix_type::COMPLEX_SYMMETRIC;
		// 	else
		// 		return is_positive_definite_ ? Matrix_type::REAL_SYMMETRIC_POS_DEFINITE
		// 									 : Matrix_type::REAL_SYMMETRIC_INDEFINITE;
		// }

		// if constexpr (std::is_same_v<Symmetry_tag, Structural_symmetric>)
		// 	return is_complex ? Matrix_type::COMPLEX_STRUCTURAL_SYMMETRIC
		// 					  : Matrix_type::REAL_STRUCTURAL_SYMMETRIC;

		// if constexpr (std::is_same_v<T_Structure_tag, Hermitian>)
		// {
		// 	static_assert(is_complex, "Hermitian tag should be used with complex matrices only");
		// 	return t_is_positive_definite ? Pardiso_matrix_type::COMPLEX_HERMITIAN_POS_DEFINITE
		// 								  : Pardiso_matrix_type::COMPLEX_HERMITIAN_INDEFINITE;
		// }

		// return is_complex ? Matrix_type::COMPLEX_GENERAL : Matrix_type::REAL_GENERAL;
	}

private:
	void* handle_[64];
	const Sparse_matrix& matrix_;

	MKL_INT message_level_ = 0;
	Params params_;
};
} // namespace esl
