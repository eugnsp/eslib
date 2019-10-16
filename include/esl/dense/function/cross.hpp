#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/type_traits.hpp>

#include <esu/numeric/det.hpp>

namespace esl
{
template<class Expr1, class Value_category1, class Expr2, class Value_category2>
auto cross(const Dense<Expr1, Value_category1>& expr1, const Dense<Expr2, Value_category2>& expr2)
{
	static_assert(internal::is_vector<Expr1> && internal::is_vector<Expr2> && ct_rows_value<Expr1> == 2 &&
					  ct_rows_value<Expr2> == 2,
		"Expressions should be vectors of size 2");

	return esu::det(expr1[0], expr1[1], expr2[0], expr2[1]);
}
} // namespace esl
