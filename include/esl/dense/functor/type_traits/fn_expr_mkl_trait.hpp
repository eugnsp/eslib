#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/functor/fn_expr_mkl_mat_copy.hpp>
#include <esl/dense/functor/fn_expr_mkl_mat_mul.hpp>
#include <esl/dense/functor/fn_expr_mkl_vec_axpy.hpp>
#include <esl/dense/functor/type_traits/fn_expr_trait.hpp>
#include <esl/dense/type_traits/ct_extent.hpp>
#include <esl/dense/type_traits/mkl_decay.hpp>
#include <esl/dense/type_traits/numeric.hpp>

#include <type_traits>

namespace esl::internal
{
//////////////////////////////////////////////////////////////////////
//* = */

// X = Y
template<class Expr1, class Expr2, typename Value>
struct Fn_expr_assign_trait<Expr1, Expr2, Lvalue_block_tag<Value>, Lvalue_block_tag<Value>,
	std::enable_if_t<is_fd_or_cfd<Value> && is_rc_dynamic<Expr1> && is_rc_dynamic<Expr2>>>
{
	using Type = Fn_expr_mkl_mat_copy<Fn_expr_mkl_mat_copy_assign_tag>;
};

// X = a * Y
template<class Expr1, class Expr2, typename Value, typename Scalar>
struct Fn_expr_assign_trait<Expr1, Expr2, Lvalue_block_tag<Value>,
	Scalar_expr_tag<Lvalue_block_tag<Value>, Scalar, Fn_mul_tag>,
	std::enable_if_t<is_fd_or_cfd<Value> && std::is_convertible_v<Scalar, Value> && is_rc_dynamic<Expr1> &&
					 is_rc_dynamic<Expr2>>>
{
	using Type = Fn_expr_mkl_mat_copy<Fn_expr_mkl_mat_copy_scale_assign_tag>;
};

// X = Y * Z
template<class Expr1, class Expr2, typename Value>
struct Fn_expr_assign_trait<Expr1, Expr2, Lvalue_block_tag<Value>,
	Binary_expr_tag<Lvalue_block_tag<Value>, Lvalue_block_tag<Value>, Fn_mul_tag>,
	std::enable_if_t<is_fd_or_cfd<Value> && is_dynamic<Expr1> && is_dynamic<Expr2>>>
{
	using Type = Fn_expr_mkl_mat_mul<Fn_expr_mkl_mat_mul_assign_tag>;
};

//////////////////////////////////////////////////////////////////////
//* += */

// X += Y
template<class Expr1, class Expr2, typename Value>
struct Fn_expr_add_assign_trait<Expr1, Expr2, Lvalue_block_tag<Value>, Lvalue_block_tag<Value>,
	std::enable_if_t<is_fd_or_cfd<Value> && is_dynamic<Expr1> && is_dynamic<Expr2> &&
					 (is_vector<Expr1> || is_vector<Expr2>)>>
{
	using Type = Fn_expr_mkl_vec_axpy<Fn_expr_mkl_vec_axpy_add_assign_tag>;
};

// X += Y * Z
template<class Expr1, class Expr2, typename Value>
struct Fn_expr_add_assign_trait<Expr1, Expr2, Lvalue_block_tag<Value>,
	Binary_expr_tag<Lvalue_block_tag<Value>, Lvalue_block_tag<Value>, Fn_mul_tag>,
	std::enable_if_t<is_fd_or_cfd<Value> && is_dynamic<Expr1> && is_dynamic<Expr2>>>
{
	using Type = Fn_expr_mkl_mat_mul<Fn_expr_mkl_mat_mul_add_assign_tag>;
};
} // namespace esl::internal
