#pragma once
#include <esl/dense/forward.hpp>

#include <esu/type_traits.hpp>

#include <type_traits>

namespace esl::internal
{
template<class Expr>
inline constexpr bool is_view_impl = false;

template<class Expr, class Rows, class Cols, class Category>
inline constexpr bool is_view_impl<View<Expr, Rows, Cols, Category>> = true;

template<class Expr>
inline constexpr bool is_view = is_view_impl<esu::Remove_cv_ref<Expr>>;
} // namespace esl::internal
