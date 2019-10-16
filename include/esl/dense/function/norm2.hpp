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
class Fn_norm2
{
public:
	auto operator()(const Expr& expr) const
	{
		using Ret = decltype(std::norm(expr[0]));
		Ret nrm_sq{};

		for (std::size_t i = 0; i < expr.size(); ++i)
			nrm_sq += std::norm(expr[i]);

		return std::sqrt(nrm_sq);
	}
};

template<class Expr, typename Value>
class Fn_norm2<Expr, Lvalue_block_tag<Value>, std::enable_if_t<is_fd_or_cfd<Value> && is_dynamic<Expr>>>
{
public:
	auto operator()(const Expr& expr) const
	{
		return internal::mkl_nrm2(expr.size(), expr.data(), expr.row_stride());
	}
};
} // namespace internal

template<typename T>
auto norm2(T v) -> decltype(std::abs(T{}))
{
	return std::abs(v);
}

template<class Expr, class Category>
auto norm2(const Dense<Expr, Category>& expr)
{
	static_assert(internal::is_vector<Expr>);
	return internal::Fn_norm2<Expr, internal::Mkl_expr_decay<Expr>>{}(expr);
}
} // namespace esl
