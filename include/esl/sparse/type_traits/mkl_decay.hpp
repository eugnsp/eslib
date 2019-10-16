#pragma once
#include <esl/dense/type_traits/mkl_decay.hpp>
#include <esl/sparse/forward.hpp>
#include <esl/sparse/tags.hpp>

namespace esl::internal
{
template<class Matrix>
struct Mkl_expr_decay_trait_impl<Mkl_sparse_matrix<Matrix>>
{
	using Type = Mkl_sparse_matrix_t<typename Matrix::Value>;
};
} // namespace esl::internal
