#pragma once
#include <esl/dense/forward.hpp>

#include <esu/type_traits.hpp>

#include <type_traits>

namespace esl::internal
{
template<class Expr>
inline constexpr bool is_view_impl = false;

template<class Expr, class Rows, class Cols>
inline constexpr bool is_view_impl<View<Expr, Rows, Cols>> = true;

template<class Expr>
inline constexpr bool is_view = is_view_impl<esu::Remove_cv_ref<Expr>>;
} // namespace esl::internal
