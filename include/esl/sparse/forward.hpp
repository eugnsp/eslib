#pragma once
#include <esl/dense/tags.hpp>

#include <mkl_types.h>

namespace esl
{
template<class Symmetry = Not_symmetric, typename Index = MKL_UINT>
class Csr_pattern;

template<typename Value, class Symmetry = Not_symmetric, typename Index = MKL_UINT>
class Csr_matrix;

template<class Matrix>
class Mkl_sparse_matrix;

template<class Matrix>
class Mkl_sparse_matrix_ext;

namespace internal
{
template<class Expr1, class Sparse_matrix, class Expr2>
struct Mkl_sparse_matrix_mul_op;
}
} // namespace esl
