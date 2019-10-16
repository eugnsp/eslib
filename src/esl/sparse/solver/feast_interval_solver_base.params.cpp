#include <esl/sparse/solver/feast_interval_solver_base.hpp>

#include <mkl_types.h>

#include <cstddef>

namespace esl::internal
{
Feast_interval_solver_base::Params::operator MKL_INT*()
{
	return params_;
}

void Feast_interval_solver_base::Params::set_matrix_checker(bool flag)
{
	params_[26] = params_[27] = flag;
}

void Feast_interval_solver_base::Params::set_status_printer(bool flag)
{
	params_[0] = flag;
}

void Feast_interval_solver_base::Params::set_user_init_guess(bool flag)
{
	params_[4] = flag;
}

std::size_t Feast_interval_solver_base::Params::q_start_col() const
{
	return static_cast<std::size_t>(params_[23] - 1);
}

// Returns the number of columns of Q for RCI routines
std::size_t Feast_interval_solver_base::Params::q_cols() const
{
	return static_cast<std::size_t>(params_[24]);
}
} // namespace esl::internal
