#pragma once
#include <esl/dense/type_traits.hpp>

#include <mkl_types.h>

#include <complex>
#include <cstddef>
#include <string>
#include <utility>

namespace esl::internal
{
class Feast_interval_solver_base
{
protected:
	class Params
	{
	public:
		operator MKL_INT*();

		void set_matrix_checker(bool = true);
		void set_status_printer(bool = true);
		void set_user_init_guess(bool = true);

		// Returns the index of the first column of Q for RCI routines
		std::size_t q_start_col() const;

		// Returns the number of columns of Q for RCI routines
		std::size_t q_cols() const;

	private:
		MKL_INT params_[128];
	};

	enum class Status : MKL_INT
	{
		ERR_SYSTEM_SIZE = 202,
		ERR_INIT_SUBSPACE = 201,
		ERR_E_MIN_MAX = 200,
		SUCCESS_SUBSPACE = 4,
		WARN_SUBSPACE_TOO_SMALL = 3,
		WARN_NO_CONVERGENCE = 2,
		WARN_NO_EIGENVALUES = 1,
		SUCCESS = 0,
		ERR_ALLOC = -1,
		ERR_LINEAR_SOLVER = -2,
		ERR_EIGENVALUE_SOLVER = -3,
		ERR_B_NOT_POS_DEFINIT = -4
	};

	enum class Job : MKL_INT
	{
		DONE = 0,
		INIT = -1,
		FACTORIZE_Z = 10,
		SOLVE_Z = 11,
		FACTORIZE_ZH = 20,
		SOLVE_ZH = 21,
		MULTIPLY_A = 30,
		MULTIPLY_B = 40,
		RERUN = -2
	};

protected:
	Feast_interval_solver_base();

#define ESL_MKL_FEAST_RCI(T)                                                                                         \
	Status mkl_feast_rci(Job&, MKL_UINT n, Add_complex<T>& ze, T* work1, Add_complex<T>* work2, T* work3, T* work4,    \
		Remove_complex<T>& eps, MKL_UINT& n_loops,                                                                     \
		std::pair<Remove_complex<T>, Remove_complex<T>> eigen_values_interval, MKL_UINT n_eigen_values0,               \
		Remove_complex<T>* eigen_values, T* eigen_vectors, MKL_UINT& n_eigen_values, Remove_complex<T>* residues);

	ESL_MKL_FEAST_RCI(float)
	ESL_MKL_FEAST_RCI(double)
	ESL_MKL_FEAST_RCI(std::complex<float>)
	ESL_MKL_FEAST_RCI(std::complex<double>)

	static std::string error_string(Status);

protected:
	Params params_;
};
} // namespace esl::internal
