#include <esl/dense/type_traits.hpp>
#include <esl/sparse/utility/mkl_overloads.hpp>

#include <mkl_spblas.h>
#include <mkl_types.h>

#include <complex>
#include <cstddef>
#include <cstring>

namespace esl::internal
{
#define ESL_IMPL_MKL_SPARSE_CREATE_CSR(fn, T)                                                                        \
	::sparse_status_t mkl_sparse_create_csr(::sparse_matrix_t& handle, MKL_UINT n_rows, MKL_UINT n_cols,               \
		MKL_UINT* rows_start, MKL_UINT* rows_end, MKL_UINT* col_index, T* values)                                      \
	{                                                                                                                  \
		return ::fn(&handle, ::sparse_index_base_t::SPARSE_INDEX_BASE_ZERO, static_cast<MKL_INT>(n_rows),              \
			static_cast<MKL_INT>(n_cols), reinterpret_cast<MKL_INT*>(rows_start),                                      \
			reinterpret_cast<MKL_INT*>(rows_end), reinterpret_cast<MKL_INT*>(col_index),                               \
			reinterpret_cast<Mkl_type<T>*>(values));                                                                   \
	}

ESL_IMPL_MKL_SPARSE_CREATE_CSR(mkl_sparse_s_create_csr, float)
ESL_IMPL_MKL_SPARSE_CREATE_CSR(mkl_sparse_d_create_csr, double)
ESL_IMPL_MKL_SPARSE_CREATE_CSR(mkl_sparse_c_create_csr, std::complex<float>)
ESL_IMPL_MKL_SPARSE_CREATE_CSR(mkl_sparse_z_create_csr, std::complex<double>)

#define ESL_IMPL_MKL_SPARSE_SET_VALUE(fn, T)                                                                         \
	::sparse_status_t mkl_sparse_set_value(::sparse_matrix_t handle, MKL_UINT row, MKL_UINT col, T value)              \
	{                                                                                                                  \
		Mkl_type<T> value_m;                                                                                           \
		std::memcpy(&value_m, &value, sizeof(T));                                                                      \
		return ::fn(handle, static_cast<MKL_INT>(row), static_cast<MKL_INT>(col), value_m);                            \
	}

ESL_IMPL_MKL_SPARSE_SET_VALUE(mkl_sparse_s_set_value, float)
ESL_IMPL_MKL_SPARSE_SET_VALUE(mkl_sparse_d_set_value, double)
ESL_IMPL_MKL_SPARSE_SET_VALUE(mkl_sparse_c_set_value, std::complex<float>)
ESL_IMPL_MKL_SPARSE_SET_VALUE(mkl_sparse_z_set_value, std::complex<double>)

#define ESL_IMPL_MKL_SPARSE_EXPORT_CSR(fn, T)                                                                        \
	::sparse_status_t mkl_sparse_export_csr(::sparse_matrix_t handle, ::sparse_index_base_t& indexing,                 \
		MKL_UINT& n_rows, MKL_UINT& n_cols, MKL_UINT*& rows_start, MKL_UINT*& rows_end, MKL_UINT*& col_index,          \
		T*& values)                                                                                                    \
	{                                                                                                                  \
		return ::fn(handle, &indexing, reinterpret_cast<MKL_INT*>(&n_rows), reinterpret_cast<MKL_INT*>(&n_cols),       \
			reinterpret_cast<MKL_INT**>(&rows_start), reinterpret_cast<MKL_INT**>(&rows_end),                          \
			reinterpret_cast<MKL_INT**>(&col_index), reinterpret_cast<Mkl_type<T>**>(&values));                        \
	}

ESL_IMPL_MKL_SPARSE_EXPORT_CSR(mkl_sparse_s_export_csr, float)
ESL_IMPL_MKL_SPARSE_EXPORT_CSR(mkl_sparse_d_export_csr, double)
ESL_IMPL_MKL_SPARSE_EXPORT_CSR(mkl_sparse_c_export_csr, std::complex<float>)
ESL_IMPL_MKL_SPARSE_EXPORT_CSR(mkl_sparse_z_export_csr, std::complex<double>)

#define ESL_IMPL_MKL_SPARSE_MM(fn, T)                                                                                \
	::sparse_status_t mkl_sparse_mm(::sparse_operation_t operation, T alpha, ::sparse_matrix_t handle,                 \
		const ::matrix_descr& descr, ::sparse_layout_t layout, const T* x, std::size_t columns, std::size_t l_dim_x,   \
		T beta, T* y, std::size_t l_dim_y)                                                                             \
	{                                                                                                                  \
		using TM = Mkl_type<T>;                                                                                        \
		TM alpha_m, beta_m;                                                                                            \
		std::memcpy(&alpha_m, &alpha, sizeof(T));                                                                      \
		std::memcpy(&beta_m, &beta, sizeof(T));                                                                        \
                                                                                                                       \
		return ::fn(operation, alpha_m, handle, descr, layout, reinterpret_cast<const TM*>(x),                         \
			static_cast<MKL_INT>(columns), static_cast<MKL_INT>(l_dim_x), beta_m, reinterpret_cast<TM*>(y),            \
			static_cast<MKL_INT>(l_dim_y));                                                                            \
	}

ESL_IMPL_MKL_SPARSE_MM(mkl_sparse_s_mm, float)
ESL_IMPL_MKL_SPARSE_MM(mkl_sparse_d_mm, double)
ESL_IMPL_MKL_SPARSE_MM(mkl_sparse_c_mm, std::complex<float>)
ESL_IMPL_MKL_SPARSE_MM(mkl_sparse_z_mm, std::complex<double>)
} // namespace esl::internal
