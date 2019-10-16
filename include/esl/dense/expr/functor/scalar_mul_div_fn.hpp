#pragma once
#include <esl/dense/type_traits.hpp>

#include <cstddef>
#include <utility>

namespace esl::internal
{
template<class Expr, typename Scalar>
class Scalar_mul_left_fn
{
public:
	using Value = decltype(std::declval<Scalar>() * std::declval<Value_type<Expr>>());

public:
	static Value element(const Expr& expr, const Scalar& scalar, std::size_t row, std::size_t col)
	{
		return scalar * expr(row, col);
	}
};

template<class Expr, typename Scalar>
class Scalar_mul_right_fn
{
public:
	using Value = decltype(std::declval<Scalar>() * std::declval<Value_type<Expr>>());

public:
	static Value element(const Expr& expr, const Scalar& scalar, std::size_t row, std::size_t col)
	{
		return scalar * expr(row, col);
	}
};

template<class Expr, typename Scalar>
class Scalar_div_fn
{
public:
	using Value = decltype(std::declval<Value_type<Expr>>() / std::declval<Scalar>());

public:
	static Value element(const Expr& expr, const Scalar& scalar, std::size_t row, std::size_t col)
	{
		return expr(row, col) / scalar;
	}
};
} // namespace esl::internal
