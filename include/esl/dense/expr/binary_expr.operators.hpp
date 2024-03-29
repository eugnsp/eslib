#pragma once
#include <esl/dense/expr/functor.hpp>
#include <esl/dense/expression.hpp>
#include <esl/dense/forward.hpp>

namespace esl
{
template<class Expr1, class Expr2>
auto operator+(const Expression<Expr1>& expr1, const Expression<Expr2>& expr2)
	-> Binary_expr<Expr1, Expr2, internal::Add_fn>
{
	return {expr1.self(), expr2.self()};
}

template<class Expr1, class Expr2>
auto operator-(const Expression<Expr1>& expr1, const Expression<Expr2>& expr2)
	-> Binary_expr<Expr1, Expr2, internal::Sub_fn>
{
	return {expr1.self(), expr2.self()};
}

template<class Expr1, class Expr2>
auto operator*(const Expression<Expr1>& expr1, const Expression<Expr2>& expr2)
	-> Binary_expr<Expr1, Expr2, internal::Mul_fn>
{
	return {expr1.self(), expr2.self()};
}
} // namespace esl
