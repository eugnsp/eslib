#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/ct_extent.hpp>
#include <esl/dense/utility/mkl_overloads.hpp>

#include <cmath>
#include <complex>
#include <cstddef>
#include <type_traits>

namespace esl
{
namespace internal
{
template<class Expr, class Mkl_tag, typename = void>
class Fn_norm1
{
public:
	auto operator()(const Expr& expr) const
	{
		using Ret = decltype(std::norm(expr(0, 0)));
		Ret nrm{};

		for (std::size_t col = 0; col < expr.cols(); ++col)
			for (std::size_t row = 0; row < expr.rows(); ++row)
				nrm += std::abs(expr(row, col));

		return nrm;
	}
};
} // namespace internal

template<typename T>
auto norm1(T v) -> decltype(std::abs(T{}))
{
	return std::abs(v);
}

template<class Expr, class Category>
auto norm1(const Dense<Expr, Category>& expr)
{
	return internal::Fn_norm1<Expr, internal::Mkl_expr_decay<Expr>>{}(expr);
}
} // namespace esl
