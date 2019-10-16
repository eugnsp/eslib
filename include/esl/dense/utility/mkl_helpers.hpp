#pragma once
#include <esl/dense/tags.hpp>

#include <mkl_cblas.h>
#include <mkl_lapacke.h>

namespace esl::internal
{
//////////////////////////////////////////////////////////////////////
//> Matrix memory layouts

inline constexpr ::CBLAS_LAYOUT layout_as_mkl_enum(Col_major)
{
	return ::CBLAS_LAYOUT::CblasColMajor;
}

inline constexpr ::CBLAS_LAYOUT layout_as_mkl_enum(Row_major)
{
	return ::CBLAS_LAYOUT::CblasRowMajor;
}

inline constexpr char layout_as_mkl_char(Col_major)
{
	return 'C';
}

inline constexpr char layout_as_mkl_char(Row_major)
{
	return 'R';
}

inline constexpr int layout_as_mkl_lapack(Col_major)
{
	return LAPACK_COL_MAJOR;
}

inline constexpr int layout_as_mkl_lapack(Row_major)
{
	return LAPACK_ROW_MAJOR;
}

///////////////////////////////////////////////////////////////////////
//> Matrix operations

inline constexpr ::CBLAS_TRANSPOSE transp_op_as_mkl_enum(No_transpose)
{
	return ::CBLAS_TRANSPOSE::CblasNoTrans;
}

inline constexpr ::CBLAS_TRANSPOSE transp_op_as_mkl_enum(Transpose)
{
	return ::CBLAS_TRANSPOSE::CblasTrans;
}

inline constexpr ::CBLAS_TRANSPOSE transp_op_as_mkl_enum(Conj_transpose)
{
	return ::CBLAS_TRANSPOSE::CblasConjTrans;
}

inline constexpr char transp_op_as_mkl_char(No_transpose)
{
	return 'N';
}

inline constexpr char transp_op_as_mkl_char(Transpose)
{
	return 'T';
}

inline constexpr char transp_op_as_mkl_char(Conj_transpose)
{
	return 'C';
}

///////////////////////////////////////////////////////////////////////
//> Matrix symmetry

inline constexpr char symmetry_as_mkl_char(Symmetric_upper)
{
	return 'U';
}

inline constexpr char symmetry_as_mkl_char(Symmetric_lower)
{
	return 'L';
}
} // namespace esl::internal
