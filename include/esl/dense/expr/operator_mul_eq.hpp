#pragma once
#include <esl/dense/dense.hpp>

#include <cstddef>

namespace esl
{
template<
	class Expr,
	typename Scalar>
auto operator*=(
	Dense<Expr, Lvalue>& expr,
	const Scalar& 		 scalar)
-> Dense<Expr, Lvalue>&
{
	for (std::size_t col = 0; col < expr.cols(); ++col)
		for (std::size_t row = 0; row < expr.rows(); ++row)
			expr.self()(row, col) *= scalar;

	return expr;
}
} // namespace esl
