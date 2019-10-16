#pragma once
#include <esl/dense/tags.hpp>

#include <type_traits>

namespace esl::internal
{
template<class Category>
inline constexpr bool is_lvalue = std::is_same_v<Category, Lvalue>;
}
