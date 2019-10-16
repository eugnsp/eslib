#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/function/det.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/ct_extent.hpp>

#include <type_traits>
#include <utility>

namespace esl
{
// clang-format off
template<class Expr, std::enable_if_t<internal::is_extent_static_and_eq(ct_rows_value<Expr>, 2) &&
									  internal::is_extent_static_and_eq(ct_cols_value<Expr>, 2), int> = 0>
void invert(Dense<Expr, Lvalue>& expr)
{
	const auto d = det(expr);

	using std::swap;
	swap(expr(0, 0), expr(1, 1));

	expr(0, 0) /= d;
	expr(1, 1) /= d;
	expr(1, 0) /= -d;
	expr(0, 1) /= -d;
}

template<class Expr, std::enable_if_t<internal::is_extent_static_and_eq(ct_rows_value<Expr>, 2) &&
									  internal::is_extent_static_and_eq(ct_cols_value<Expr>, 2), int> = 0>
void invert_transpose(Dense<Expr, Lvalue>& expr)
{
	invert(expr);

	using std::swap;
	swap(expr(0, 1), expr(1, 0));
}
// clang-format on
} // namespace esl
