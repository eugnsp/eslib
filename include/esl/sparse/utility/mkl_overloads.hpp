#pragma once
#include <mkl_spblas.h>
#include <mkl_types.h>

#include <complex>
#include <cstddef>

namespace esl::internal
{
#define ESL_MKL_SPARSE_CREATE_CSR(T)                                                                                 \
	::sparse_status_t mkl_sparse_create_csr(::sparse_matrix_t&, MKL_UINT n_rows, MKL_UINT n_cols,                      \
		MKL_UINT* rows_start, MKL_UINT* rows_end, MKL_UINT* col_index, T*);

ESL_MKL_SPARSE_CREATE_CSR(float)
ESL_MKL_SPARSE_CREATE_CSR(double)
ESL_MKL_SPARSE_CREATE_CSR(std::complex<float>)
ESL_MKL_SPARSE_CREATE_CSR(std::complex<double>)

#define ESL_MKL_SPARSE_SET_VALUE(T)                                                                                  \
	::sparse_status_t mkl_sparse_set_value(::sparse_matrix_t, MKL_UINT row, MKL_UINT col, T);

ESL_MKL_SPARSE_SET_VALUE(float)
ESL_MKL_SPARSE_SET_VALUE(double)
ESL_MKL_SPARSE_SET_VALUE(std::complex<float>)
ESL_MKL_SPARSE_SET_VALUE(std::complex<double>)

#define ESL_MKL_SPARSE_EXPORT_CSR(T)                                                                                 \
	::sparse_status_t mkl_sparse_export_csr(::sparse_matrix_t, ::sparse_index_base_t&, MKL_UINT& n_rows,               \
		MKL_UINT& n_cols, MKL_UINT*& rows_start, MKL_UINT*& rows_end, MKL_UINT*& col_index, T*& values);

ESL_MKL_SPARSE_EXPORT_CSR(float)
ESL_MKL_SPARSE_EXPORT_CSR(double)
ESL_MKL_SPARSE_EXPORT_CSR(std::complex<float>)
ESL_MKL_SPARSE_EXPORT_CSR(std::complex<double>)

#define ESL_MKL_SPARSE_MM(T)                                                                                         \
	::sparse_status_t mkl_sparse_mm(::sparse_operation_t operation, T alpha, ::sparse_matrix_t handle,                 \
		const ::matrix_descr& descr, ::sparse_layout_t layout, const T* x, std::size_t columns, std::size_t l_dim_x,   \
		T beta, T* y, std::size_t l_dim_y);

ESL_MKL_SPARSE_MM(float)
ESL_MKL_SPARSE_MM(double)
ESL_MKL_SPARSE_MM(std::complex<float>)
ESL_MKL_SPARSE_MM(std::complex<double>)
} // namespace esl::internal
