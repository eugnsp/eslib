#include <esl/sparse/solver/pardiso_solver_base.hpp>

#include <mkl_types.h>

namespace esl::internal
{
Pardiso_solver_base::Params::operator MKL_INT*()
{
	return params_;
}

void Pardiso_solver_base::Params::set_matrix_checker(bool enable)
{
	params_[26] = enable;
}

void Pardiso_solver_base::Params::set_zero_based_indexing()
{
	params_[34] = 1;
}
} // namespace esl::internal
