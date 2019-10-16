#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/mkl_overloads.hpp>

#include <mkl_cblas.h>
#include <mkl_lapacke.h>
#include <mkl_trans.h>
#include <mkl_types.h>

#include <complex>
#include <cstddef>
#include <cstring>
#include <stdexcept>
#include <string>

namespace esl::internal
{
//////////////////////////////////////////////////////////////////////
//> BLAS level 1

#define ESL_IMPL_MKL_BLAS_AXPY_R(fn, T)                                                                                \
	void mkl_axpy(const std::size_t n, const T a, const T* const data_x, const std::size_t inc_x, T* const data_y,     \
		const std::size_t inc_y)                                                                                       \
	{                                                                                                                  \
		::fn(static_cast<MKL_INT>(n), a, data_x, static_cast<MKL_INT>(inc_x), data_y, static_cast<MKL_INT>(inc_y));    \
	}

#define ESL_IMPL_MKL_BLAS_AXPY_C(fn, T)                                                                                \
	void mkl_axpy(const std::size_t n, const T a, const T* const data_x, const std::size_t inc_x, T* const data_y,     \
		const std::size_t inc_y)                                                                                       \
	{                                                                                                                  \
		::fn(static_cast<MKL_INT>(n), &a, data_x, static_cast<MKL_INT>(inc_x), data_y, static_cast<MKL_INT>(inc_y));   \
	}

ESL_IMPL_MKL_BLAS_AXPY_R(cblas_saxpy, float)
ESL_IMPL_MKL_BLAS_AXPY_R(cblas_daxpy, double)
ESL_IMPL_MKL_BLAS_AXPY_C(cblas_caxpy, std::complex<float>)
ESL_IMPL_MKL_BLAS_AXPY_C(cblas_zaxpy, std::complex<double>)

///////////////////////////////////////////////////////////////////////

#define ESL_IMPL_MKL_BLAS_DOT(fn, T)                                                                                   \
	T mkl_dot(const std::size_t n, const T* const data_x, const std::size_t inc_x, const T* const data_y,              \
		const std::size_t inc_y)                                                                                       \
	{                                                                                                                  \
		return ::fn(                                                                                                   \
			static_cast<MKL_INT>(n), data_x, static_cast<MKL_INT>(inc_x), data_y, static_cast<MKL_INT>(inc_y));        \
	}

ESL_IMPL_MKL_BLAS_DOT(cblas_sdot, float)
ESL_IMPL_MKL_BLAS_DOT(cblas_ddot, double)

///////////////////////////////////////////////////////////////////////

#define ESL_IMPL_MKL_BLAS_NRM2(fn, T)                                                                                  \
	Remove_complex<T> mkl_nrm2(const std::size_t n, const T* const data, const std::size_t inc)                        \
	{                                                                                                                  \
		return ::fn(static_cast<MKL_INT>(n), data, static_cast<MKL_INT>(inc));                                         \
	}

ESL_IMPL_MKL_BLAS_NRM2(cblas_snrm2, float)
ESL_IMPL_MKL_BLAS_NRM2(cblas_dnrm2, double)
ESL_IMPL_MKL_BLAS_NRM2(cblas_scnrm2, std::complex<float>)
ESL_IMPL_MKL_BLAS_NRM2(cblas_dznrm2, std::complex<double>)

//////////////////////////////////////////////////////////////////////
//> BLAS levels 2 and 3

#define ESL_IMPL_MKL_GEMV_R(fn, T)                                                                                     \
	void mkl_gemv(const CBLAS_LAYOUT layout, const CBLAS_TRANSPOSE transp, const std::size_t m, const std::size_t n,   \
		const T alpha, const T* const data_a, const std::size_t lead_dim_a, const T* const data_x,                     \
		const std::size_t inc_x, const T beta, T* const data_y, const std::size_t inc_y)                               \
	{                                                                                                                  \
		::fn(layout, transp, static_cast<MKL_INT>(m), static_cast<MKL_INT>(n), alpha, data_a, lead_dim_a, data_x,      \
			inc_x, beta, data_y, inc_y);                                                                               \
	}

#define ESL_IMPL_MKL_GEMV_C(fn, T)                                                                                     \
	void mkl_gemv(const CBLAS_LAYOUT layout, const CBLAS_TRANSPOSE transp, const std::size_t m, const std::size_t n,   \
		const T alpha, const T* const data_a, const std::size_t lead_dim_a, const T* const data_x,                     \
		const std::size_t inc_x, const T beta, T* const data_y, const std::size_t inc_y)                               \
	{                                                                                                                  \
		::fn(layout, transp, static_cast<MKL_INT>(m), static_cast<MKL_INT>(n), &alpha, data_a, lead_dim_a, data_x,     \
			inc_x, &beta, data_y, inc_y);                                                                              \
	}

ESL_IMPL_MKL_GEMV_R(cblas_sgemv, float)
ESL_IMPL_MKL_GEMV_R(cblas_dgemv, double)
ESL_IMPL_MKL_GEMV_C(cblas_cgemv, std::complex<float>)
ESL_IMPL_MKL_GEMV_C(cblas_zgemv, std::complex<double>)

//////////////////////////////////////////////////////////////////////

#define ESL_IMPL_MKL_GEMM_R(fn, T)                                                                                     \
	void mkl_gemm(const CBLAS_LAYOUT layout, const CBLAS_TRANSPOSE transp_a, const CBLAS_TRANSPOSE transp_b,           \
		const std::size_t m, const std::size_t n, const std::size_t k, const T alpha, const T* const data_a,           \
		const std::size_t lead_dim_a, const T* const data_b, const std::size_t lead_dim_b, const T beta,               \
		T* const data_c, const std::size_t lead_dim_c)                                                                 \
	{                                                                                                                  \
		::fn(layout, transp_a, transp_b, static_cast<MKL_INT>(m), static_cast<MKL_INT>(n), static_cast<MKL_INT>(k),    \
			alpha, data_a, lead_dim_a, data_b, lead_dim_b, beta, data_c, lead_dim_c);                                  \
	}

#define ESL_IMPL_MKL_GEMM_C(fn, T)                                                                                     \
	void mkl_gemm(const CBLAS_LAYOUT layout, const CBLAS_TRANSPOSE transp_a, const CBLAS_TRANSPOSE transp_b,           \
		const std::size_t m, const std::size_t n, const std::size_t k, const T alpha, const T* const data_a,           \
		const std::size_t lead_dim_a, const T* const data_b, const std::size_t lead_dim_b, const T beta,               \
		T* const data_c, const std::size_t lead_dim_c)                                                                 \
	{                                                                                                                  \
		::fn(layout, transp_a, transp_b, static_cast<MKL_INT>(m), static_cast<MKL_INT>(n), static_cast<MKL_INT>(k),    \
			&alpha, data_a, lead_dim_a, data_b, lead_dim_b, &beta, data_c, lead_dim_c);                                \
	}

ESL_IMPL_MKL_GEMM_R(cblas_sgemm, float)
ESL_IMPL_MKL_GEMM_R(cblas_dgemm, double)
ESL_IMPL_MKL_GEMM_C(cblas_cgemm3m, std::complex<float>)
ESL_IMPL_MKL_GEMM_C(cblas_zgemm3m, std::complex<double>)

///////////////////////////////////////////////////////////////////////
//> BLAS-like extensions

#define ESL_IMPL_MKL_OMATCOPY(fn, T)                                                                                   \
	void mkl_omatcopy(const char layout, const char trans, const std::size_t rows, const std::size_t cols,             \
		const T alpha, const T* const data_a, const std::size_t l_dim_a, T* const data_b, const std::size_t l_dim_b)   \
	{                                                                                                                  \
		Mkl_type<T> alpha_m;                                                                                           \
		std::memcpy(&alpha_m, &alpha, sizeof(T));                                                                      \
		::fn(layout, trans, rows, cols, alpha_m, reinterpret_cast<const Mkl_type<T>*>(data_a), l_dim_a,                \
			reinterpret_cast<Mkl_type<T>*>(data_b), l_dim_b);                                                          \
	}

ESL_IMPL_MKL_OMATCOPY(mkl_somatcopy, float)
ESL_IMPL_MKL_OMATCOPY(mkl_domatcopy, double)
ESL_IMPL_MKL_OMATCOPY(mkl_comatcopy, std::complex<float>)
ESL_IMPL_MKL_OMATCOPY(mkl_zomatcopy, std::complex<double>)

//////////////////////////////////////////////////////////////////////
//> LAPACK

#define ESL_IMPL_MKL_LAPACK_SYTRD(fn, T)                                                                               \
	void mkl_lapack_sytrd(const int layout, const char up_lo, const std::size_t n, T* const data_a,                    \
		const std::size_t lead_dim_a, T* const data_d, T* const data_e, T* const data_tau)                             \
	{                                                                                                                  \
		const auto info = ::fn(layout, up_lo, static_cast<lapack_int>(n), data_a, static_cast<lapack_int>(lead_dim_a), \
			data_d, data_e, data_tau);                                                                                 \
		if (info != 0)                                                                                                 \
			throw std::runtime_error(                                                                                  \
				"Parameter " + std::to_string(-info) + " has an illegal value in " + std::string(__FUNCTION__));       \
	}

ESL_IMPL_MKL_LAPACK_SYTRD(LAPACKE_ssytrd, float)
ESL_IMPL_MKL_LAPACK_SYTRD(LAPACKE_dsytrd, double)

///////////////////////////////////////////////////////////////////////

#define ESL_IMPL_MKL_LAPACK_STERF(fn, T)                                                                               \
	void mkl_lapack_sterf(const std::size_t n, T* const data_d, T* const data_e)                                       \
	{                                                                                                                  \
		const auto info = ::fn(static_cast<lapack_int>(n), data_d, data_e);                                            \
		if (info > 0)                                                                                                  \
			throw std::runtime_error(std::to_string(info) + " off-diagonal elements have not converged to zero in " +  \
									 std::string(__FUNCTION__));                                                       \
		else if (info < 0)                                                                                             \
			throw std::runtime_error(                                                                                  \
				"Parameter " + std::to_string(-info) + " has an illegal value in " + std::string(__FUNCTION__));       \
	}

ESL_IMPL_MKL_LAPACK_STERF(LAPACKE_ssterf, float)
ESL_IMPL_MKL_LAPACK_STERF(LAPACKE_dsterf, double)

} // namespace esl::internal
