#pragma once
#include <mkl_types.h>

#include <string>

namespace esl::internal
{
struct Pardiso_solver_base
{
	class Params
	{
	public:
		operator MKL_INT*();

		void set_matrix_checker(bool = true);
		void set_zero_based_indexing();

	private:
		MKL_INT params_[64];
	};

	enum class Matrix_type : MKL_INT
	{
		REAL_STRUCTURAL_SYMMETRIC = 1,
		REAL_SYMMETRIC_POS_DEFINITE = 2,
		REAL_SYMMETRIC_INDEFINITE = -2,
		COMPLEX_STRUCTURAL_SYMMETRIC = 3,
		COMPLEX_HERMITIAN_POS_DEFINITE = 4,
		COMPLEX_HERMITIAN_INDEFINITE = -4,
		COMPLEX_SYMMETRIC = 6,
		REAL_GENERAL = 11,
		COMPLEX_GENERAL = 13,
	};

	enum class Phase : MKL_INT
	{
		ANALYZE = 11,
		ANALYZE_FACTORIZE = 12,
		ANALYZE_FACTORIZE_SOLVE = 13,
		FACTORIZE = 22,
		FACTORIZE_SOLVE = 23,
		SOLVE = 33,
		RELEASE_ALL = -1
	};

	static std::string pardiso_error_string(MKL_INT);
};
} // namespace esl::internal
