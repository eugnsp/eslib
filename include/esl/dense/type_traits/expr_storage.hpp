#pragma once
#include <esl/dense/forward.hpp>

#include <esu/type_traits.hpp>

#include <type_traits>

namespace esl::internal
{
template<class Expr>
inline constexpr bool store_expr_as_reference = false;

template<typename Value, auto rows, auto cols, class Layout>
inline constexpr bool store_expr_as_reference<Matrix<Value, rows, cols, Layout>> = true;

template<class Expr>
using Expr_storage_type = std::conditional_t<store_expr_as_reference<esu::Remove_cv_ref<Expr>>,
	std::add_lvalue_reference_t<Expr>, std::remove_reference_t<Expr>>;
} // namespace esl::internal
