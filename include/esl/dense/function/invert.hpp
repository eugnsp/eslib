#pragma once
#include <esl/dense/concepts.hpp>
#include <esl/dense/forward.hpp>
#include <esl/dense/function/det.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/ct_extent.hpp>

#include <type_traits>
#include <utility>

namespace esl
{
#ifdef ESL_CONCEPTS
template<class Expr>
requires concepts::dense<std::remove_reference_t<Expr>> &&
		 internal::is_lvalue<Expr> && ct_rows_value<Expr> == 2 && ct_cols_value<Expr> == 2
#else
template<class Expr, typename = std::enable_if_t<
	internal::is_lvalue<Expr> && ct_rows_value<Expr> == 2 && ct_cols_value<Expr> == 2>>
#endif
void invert(Expr&& expr)
{
	std::swap(expr(0, 0), expr(1, 1));
	const auto one_over_d = 1 / det(expr);
	expr(0, 0) *=  one_over_d;
	expr(1, 1) *=  one_over_d;
	expr(1, 0) *= -one_over_d;
	expr(0, 1) *= -one_over_d;
}

#ifdef ESL_CONCEPTS
template<class Expr>
requires concepts::dense<std::remove_reference_t<Expr>> &&
		 internal::is_lvalue<Expr> && ct_rows_value<Expr> == 2 && ct_cols_value<Expr> == 2
#else
template<class Expr, typename = std::enable_if_t<
	internal::is_lvalue<Expr> && ct_rows_value<Expr> == 2 && ct_cols_value<Expr> == 2>>
#endif
void invert_transpose(Expr&& expr)
{
	invert(expr);
	std::swap(expr(0, 1), expr(1, 0));
}
} // namespace esl
