#pragma once
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/sparse/csr_matrix.hpp>
#include <esl/sparse/type_traits.hpp>
#include <esl/sparse/utility/mkl_overloads.hpp>

#include <mkl_spblas.h>
#include <mkl_types.h>

#include <string>

namespace esl::internal
{
//////////////////////////////////////////////////////////////////////
//* Matrix memory layouts */

inline constexpr ::sparse_layout_t layout_as_mkl_enum_sparse(Col_major)
{
	return ::sparse_layout_t::SPARSE_LAYOUT_COLUMN_MAJOR;
}

inline constexpr ::sparse_layout_t layout_as_mkl_enum_sparse(Row_major)
{
	return ::sparse_layout_t::SPARSE_LAYOUT_ROW_MAJOR;
}

///////////////////////////////////////////////////////////////////////
//* Matrix operations */

inline constexpr ::sparse_operation_t transp_op_as_mkl_enum_sparse(No_transpose)
{
	return ::sparse_operation_t::SPARSE_OPERATION_NON_TRANSPOSE;
}

inline constexpr ::sparse_operation_t transp_op_as_mkl_enum_sparse(Transpose)
{
	return ::sparse_operation_t::SPARSE_OPERATION_TRANSPOSE;
}

inline constexpr ::sparse_operation_t transp_op_as_mkl_enum_sparse(Conj_transpose)
{
	return ::sparse_operation_t::SPARSE_OPERATION_CONJUGATE_TRANSPOSE;
}

//////////////////////////////////////////////////////////////////////

template<class Matrix>
::matrix_descr mkl_matrix_descr()
{
	::matrix_descr descr;
	descr.diag = SPARSE_DIAG_NON_UNIT;
	if constexpr (is_symmetric_upper<Matrix>)
	{
		descr.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
		descr.mode = SPARSE_FILL_MODE_UPPER;
	}
	else if constexpr (is_symmetric_lower<Matrix>)
	{
		descr.type = SPARSE_MATRIX_TYPE_SYMMETRIC;
		descr.mode = SPARSE_FILL_MODE_LOWER;
	}
	else
		descr.type = SPARSE_MATRIX_TYPE_GENERAL;

	return descr;
}

template<typename Value, class Symmetry, typename Index>
::sparse_status_t mkl_sparse_create(::sparse_matrix_t& handle, const Csr_matrix<Value, Symmetry, Index>& matrix)
{
	static_assert(sizeof(Index) == sizeof(MKL_UINT));

	const auto row_indices = reinterpret_cast<MKL_UINT*>(const_cast<Index*>(matrix.row_indices()));
	const auto col_indices = reinterpret_cast<MKL_UINT*>(const_cast<Index*>(matrix.col_indices()));
	const auto values = const_cast<Value*>(matrix.values());

	return mkl_sparse_create_csr(
		handle, matrix.rows(), matrix.cols(), row_indices, row_indices + 1, col_indices, values);
}

std::string mkl_sparse_status_string(::sparse_status_t);
} // namespace esl::internal
