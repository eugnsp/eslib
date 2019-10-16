#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/ct_extent.hpp>

#include <esu/numeric/det.hpp>

#include <type_traits>

namespace esl
{
// clang-format off
template<class Expr, class Category,
	std::enable_if_t<internal::is_extent_static_and_eq(ct_rows_value<Expr>, 2) &&
					 internal::is_extent_static_and_eq(ct_cols_value<Expr>, 2), int> = 0>
auto det(const Dense<Expr, Category>& expr)
{
	return esu::det(
		expr(0, 0), expr(1, 0),
		expr(0, 1), expr(1, 1));
}

template<class Expr, class Category,
	std::enable_if_t<internal::is_extent_static_and_eq(ct_rows_value<Expr>, 3) &&
					 internal::is_extent_static_and_eq(ct_cols_value<Expr>, 3), int> = 0>
auto det(const Dense<Expr, Category>& expr)
{
	return esu::det(
		expr(0, 0), expr(1, 0), expr(2, 0),
		expr(0, 1), expr(1, 1), expr(2, 1),
		expr(0, 2), expr(1, 2), expr(2, 2));
}
// clang-format on
} // namespace esl
