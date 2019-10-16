#pragma once
#include <esl/dense/expression.hpp>
#include <esl/dense/function/for_each_element.hpp>
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace esl
{
namespace internal
{
template<class Expr, class Mkl_tag, typename = void>
class Fn_sum
{
public:
	auto operator()(const Expr& expr) const
	{
		Value_type<Expr> sum{};
		for_each_element([&sum](auto&& value) { sum += value; }, expr);

		return sum;
	}
};
} // namespace internal

template<class Expr, class Category>
auto sum(const Dense<Expr, Category>& expr)
{
	return internal::Fn_sum<Expr, internal::Mkl_expr_decay<Expr>>{}(expr.self());
}
} // namespace esl
