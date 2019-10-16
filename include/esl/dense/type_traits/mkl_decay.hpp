#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/tags.hpp>

#include <cstddef>
#include <type_traits>

namespace esl::internal
{
template<class Expr>
struct Mkl_expr_decay_trait_impl
{
	using Type = Expr;
};

template<class Expr>
using Mkl_expr_decay_trait = Mkl_expr_decay_trait_impl<std::remove_const_t<Expr>>;

template<class Expr>
using Mkl_expr_decay = typename Mkl_expr_decay_trait<Expr>::Type;

template<typename Value, std::size_t rows, std::size_t cols, class Layout>
struct Mkl_expr_decay_trait_impl<Matrix<Value, rows, cols, Layout>>
{
	using Type = Lvalue_block_tag<Value>;
};

template<class Expr, auto begin1, auto size1, auto begin2, auto size2>
struct Mkl_expr_decay_trait_impl<View<Expr, Range<begin1, size1>, Range<begin2, size2>, Lvalue>> :
	Mkl_expr_decay_trait<Expr>
{};

template<class Expr>
struct Mkl_expr_decay_trait_impl<Diag_view<Expr, Lvalue>> : Mkl_expr_decay_trait<Expr>
{};

template<class Expr>
struct Mkl_expr_decay_trait_impl<Transposed_view<Expr, Lvalue>> : Mkl_expr_decay_trait<Expr>
{};

template<class Expr, typename Scalar>
struct Mkl_expr_decay_trait_impl<Scalar_expr<Expr, Scalar, Scalar_mul_left_fn>>
{
	using Type = Scalar_expr_tag<Mkl_expr_decay<Expr>, Scalar, Fn_mul_tag>;
};

template<class Expr, typename Scalar>
struct Mkl_expr_decay_trait_impl<Scalar_expr<Expr, Scalar, Scalar_mul_right_fn>>
{
	using Type = Scalar_expr_tag<Mkl_expr_decay<Expr>, Scalar, Fn_mul_tag>;
};

template<class Expr1, class Expr2>
struct Mkl_expr_decay_trait_impl<Binary_expr<Expr1, Expr2, Mul_fn>>
{
	using Type = Binary_expr_tag<Mkl_expr_decay<Expr1>, Mkl_expr_decay<Expr2>, Fn_mul_tag>;
};
} // namespace esl::internal
