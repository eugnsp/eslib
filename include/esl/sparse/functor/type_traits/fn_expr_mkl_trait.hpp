#pragma once
#include <esl/dense/functor/type_traits/fn_expr_trait.hpp>
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/sparse/functor/fn_expr_mkl_sparse_mat_mul.hpp>

#include <esu/type_traits.hpp>

#include <type_traits>

namespace esl::internal
{
template<class Expr1, class Expr2, typename Value>
struct Fn_expr_assign_trait<Expr1, Expr2, Lvalue_block_tag<Value>,
	Binary_expr_tag<Mkl_sparse_matrix_t<Value>, Lvalue_block_tag<Value>, Fn_mul_tag>,
	std::enable_if_t<is_fd_or_cfd<Value>>>
{
	using Type = Fn_expr_mkl_sparse_mat_mul;
};
} // namespace esl::internal
