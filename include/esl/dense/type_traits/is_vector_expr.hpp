#pragma once
#include <esl/dense/type_traits/ct_extent.hpp>

#include <esu/type_traits.hpp>

#include <type_traits>

namespace esl
{
namespace internal
{
template<class Expr>
inline constexpr bool is_vector_expr_impl = ct_cols_value<Expr> == 1;
}

template<class Expr>
inline constexpr bool is_vector_expr = internal::is_vector_expr_impl<esu::Remove_cv_ref<Expr>>;
} // namespace esl
