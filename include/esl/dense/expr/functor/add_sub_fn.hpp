#pragma once
#include <esl/dense/expr/functor/elementwise_fn.hpp>
#include <esl/dense/type_traits.hpp>

#include <utility>

namespace esl::internal
{
template<class Expr1, class Expr2>
class Add_fn : public Elementwise_fn<Expr1, Expr2>
{
public:
	using Value = decltype(std::declval<Value_type<Expr1>>() + std::declval<Value_type<Expr2>>());

public:
	static Value element(const Expr1& l_expr, const Expr2& r_expr, std::size_t row, std::size_t col)
	{
		return l_expr(row, col) + r_expr(row, col);
	}
};

template<class Expr1, class Expr2>
class Sub_fn : public Elementwise_fn<Expr1, Expr2>
{
public:
	using Value = decltype(std::declval<Value_type<Expr1>>() - std::declval<Value_type<Expr2>>());

public:
	static Value element(const Expr1& left, const Expr2& right, std::size_t row, std::size_t col)
	{
		return left(row, col) - right(row, col);
	}
};
} // namespace esl::internal
