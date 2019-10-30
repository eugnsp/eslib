#pragma once
#include <esl/dense/concepts.hpp>
#include <esl/dense/forward.hpp>
#include <esl/dense/type_traits.hpp>

#include <esu/numeric/det.hpp>

#include <type_traits>

namespace esl
{
#ifdef ESL_CONCEPTS
template<class Expr>
requires concepts::dense<Expr> && ct_rows_value<Expr> == 2 && ct_cols_value<Expr> == 2
#else
template<class Expr, std::enable_if_t<(is_dense<Expr> &&
		     ct_rows_value<Expr> == 2 && ct_cols_value<Expr> == 2), int> = 0>
#endif
auto det(const Expr& expr)
{
	return esu::det(expr(0, 0), expr(1, 0),
				    expr(0, 1), expr(1, 1));
}

#ifdef ESL_CONCEPTS
template<class Expr>
requires concepts::dense<Expr> && ct_rows_value<Expr> == 3 && ct_cols_value<Expr> == 3
#else
template<class Expr, std::enable_if_t<(is_dense<Expr> &&
		     ct_rows_value<Expr> == 3 && ct_cols_value<Expr> == 3), int> = 0>
#endif
auto det(const Expr& expr)
{
	return esu::det(expr(0, 0), expr(1, 0), expr(2, 0),
					expr(0, 1), expr(1, 1), expr(2, 1),
					expr(0, 2), expr(1, 2), expr(2, 2));
}
} // namespace esl
