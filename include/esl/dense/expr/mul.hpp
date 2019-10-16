#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/type_traits.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace esl::internal
{
template<class Expr, typename Scalar /* , typename = std::enable_if_t<is_scalar<Scalar>> */>
void mul(Dense<Expr, Lvalue>& expr, Scalar scalar)
{
	for (std::size_t col = 0; col < expr.cols(); ++col)
		for (std::size_t row = 0; row < expr.rows(); ++row)
			expr(row, col) *= scalar;
}
} // namespace esl::internal
