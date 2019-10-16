#include <esl/dense/type_traits.hpp>
#include <esl/sparse/solver/feast_interval_solver_base.hpp>

#include <mkl_solvers_ee.h>
#include <mkl_types.h>

#include <complex>
#include <string>

namespace esl::internal
{
Feast_interval_solver_base::Feast_interval_solver_base()
{
	::feastinit(params_);

#ifndef NDEBUG
	params_.set_matrix_checker();
	params_.set_status_printer();
#endif
}

#define ESL_IMPL_MKL_FEAST_RCI(func, T)                                                                              \
	Feast_interval_solver_base::Status Feast_interval_solver_base::mkl_feast_rci(Job& job, MKL_UINT n,                 \
		Add_complex<T>& ze, T* work1, Add_complex<T>* work2, T* work3, T* work4, Remove_complex<T>& eps,               \
		MKL_UINT& n_loops, std::pair<Remove_complex<T>, Remove_complex<T>> eigen_values_interval,                      \
		MKL_UINT n_eigen_values0, Remove_complex<T>* eigen_values, T* eigen_vectors, MKL_UINT& n_eigen_values,         \
		Remove_complex<T>* residues)                                                                                   \
	{                                                                                                                  \
		using TM = internal::Mkl_type<T>;                                                                              \
		using CM = internal::Mkl_type<Add_complex<T>>;                                                                 \
                                                                                                                       \
		auto m_job = reinterpret_cast<MKL_INT*>(&job);                                                                 \
		auto m_n = reinterpret_cast<const MKL_INT*>(&n);                                                               \
		auto m_ze = reinterpret_cast<CM*>(&ze);                                                                        \
		auto m_work1 = reinterpret_cast<TM*>(work1);                                                                   \
		auto m_work2 = reinterpret_cast<CM*>(work2);                                                                   \
		auto m_work3 = reinterpret_cast<TM*>(work3);                                                                   \
		auto m_work4 = reinterpret_cast<TM*>(work4);                                                                   \
		auto m_n_loops = reinterpret_cast<MKL_INT*>(&n_loops);                                                         \
		auto m_n_eigen_values0 = reinterpret_cast<MKL_INT*>(&n_eigen_values0);                                         \
		auto m_n_eigen_values = reinterpret_cast<MKL_INT*>(&n_eigen_values);                                           \
		auto m_eigen_vectors = reinterpret_cast<TM*>(eigen_vectors);                                                   \
                                                                                                                       \
		MKL_INT info = 0;                                                                                              \
		::func(m_job, m_n, m_ze, m_work1, m_work2, m_work3, m_work4, params_, &eps, m_n_loops,                         \
			&eigen_values_interval.first, &eigen_values_interval.second, m_n_eigen_values0, eigen_values,              \
			m_eigen_vectors, m_n_eigen_values, residues, &info);                                                       \
                                                                                                                       \
		return static_cast<Status>(info);                                                                              \
	}

ESL_IMPL_MKL_FEAST_RCI(sfeast_srci, float)
ESL_IMPL_MKL_FEAST_RCI(dfeast_srci, double)
ESL_IMPL_MKL_FEAST_RCI(cfeast_hrci, std::complex<float>)
ESL_IMPL_MKL_FEAST_RCI(zfeast_hrci, std::complex<double>)

std::string Feast_interval_solver_base::error_string(Status status)
{
	std::string str = "FEAST error " + std::to_string(static_cast<MKL_INT>(status)) + ": ";
	switch (status)
	{
	case Status::ERR_SYSTEM_SIZE:
		return str + "Problem with size of the system n (n <= 0)";
	case Status::ERR_INIT_SUBSPACE:
		return str + "Problem with size of initial subspace m0 (m0 <= 0 or m0 > n)";
	case Status::ERR_E_MIN_MAX:
		return str + "Problem with emin, emax (emin >= emax)";
	case Status::SUCCESS_SUBSPACE:
		return str + "Successful return if only the computed subspace after call with fpm[13] = 1";
	case Status::WARN_SUBSPACE_TOO_SMALL:
		return str + "Size of the subspace m0 is too small(m0 < m)";
	case Status::WARN_NO_CONVERGENCE:
		return str + "No Convergence (number of iteration loops > fpm[3])";
	case Status::WARN_NO_EIGENVALUES:
		return str + "No eigenvalues found in the search interval";
	case Status::SUCCESS:
		return str + "No error";
	case Status::ERR_ALLOC:
		return str + "Internal error for allocation memory";
	case Status::ERR_LINEAR_SOLVER:
		return str + "Internal error of the inner system solver (not enough memory for inner linear "
					 "system solver or inconsistent input)";
	case Status::ERR_EIGENVALUE_SOLVER:
		return str + "Internal error of the reduced eigenvalue solver (matrix B may not be positive "
					 "definite)";
	case Status::ERR_B_NOT_POS_DEFINIT:
		return str + "Matrix B is not positive definite";
	}

	if (static_cast<MKL_INT>(status) >= 101)
		return str + "Problem with the value " + std::to_string(static_cast<MKL_INT>(status) - 101) +
			   " of the input Extended Eigensolver parameter (fpm)";

	if (static_cast<MKL_INT>(status) <= -101)
		return str + "Problem with the argument " + std::to_string(-(static_cast<MKL_INT>(status) + 101)) +
			   " of the Extended Eigensolver interface";

	return str + "Unknown error";
}
} // namespace esl::internal
