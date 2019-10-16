#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/function/for_each_element.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/ct_extent.hpp>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace esl
{
template<typename T>
auto norm_sup(T v) -> decltype(std::abs(std::declval<T>()))
{
	return std::abs(v);
}

template<class Expr, class Category>
auto norm_sup(const Dense<Expr, Category>& expr)
{
	using Ret = decltype(std::abs(std::declval<Value_type<Expr>>()));
	Ret norm{};

	for_each_element([&norm](auto&& value) { norm = std::max(norm, std::abs(value)); }, expr);
	return norm;
}
} // namespace esl
