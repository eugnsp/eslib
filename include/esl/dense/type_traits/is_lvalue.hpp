#pragma once
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits/category_type.hpp>

#include <type_traits>

namespace esl::internal
{
template<class Expr>
inline constexpr bool is_lvalue = std::is_same_v<Category_type<Expr>, Lvalue>;
}
