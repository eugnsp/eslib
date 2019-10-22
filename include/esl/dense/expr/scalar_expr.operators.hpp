#pragma once
#include <esl/dense/expr/functor.hpp>
#include <esl/dense/expression.hpp>
#include <esl/dense/forward.hpp>
#include <esl/dense/type_traits.hpp>

#include <type_traits>

namespace esl
{
template<
	class 	 Expr,
	typename Scalar,
	typename = std::enable_if_t<!is_expression<Scalar>>>
auto operator*(
	const Scalar&           scalar,
	const Expression<Expr>& expr)
-> Scalar_expr<Expr, Scalar, internal::Scalar_mul_left_fn>
{
	return {expr.self(), scalar};
}

template<
	class 	 Expr,
	typename Scalar,
	typename = std::enable_if_t<!is_expression<Scalar>>>
auto operator*(
	const Expression<Expr>& expr,
	const Scalar&           scalar)
-> Scalar_expr<Expr, Scalar, internal::Scalar_mul_right_fn>
{
	return {expr.self(), scalar};
}

template<
	class 	 Expr,
	typename Scalar,
	typename = std::enable_if_t<!is_expression<Scalar>>>
auto operator/(
	const Expression<Expr>& expr,
	const Scalar& 	        scalar)
-> Scalar_expr<Expr, Scalar, internal::Scalar_div_fn>
{
	return {expr.self(), scalar};
}
} // namespace esl
