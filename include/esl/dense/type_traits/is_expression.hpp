#pragma once
#include <esl/dense/forward.hpp>

#include <type_traits>
#include <utility>

namespace esl
{
namespace internal
{
template<class Expr>
void expression_match(const Expression<Expr>&);

int expression_match(...);
} // namespace internal

template<class Expr>
struct Is_expression_trait : std::is_void<decltype(internal::expression_match(std::declval<Expr>()))>
{};

template<class Expr>
inline constexpr bool is_expression = Is_expression_trait<Expr>::value;
} // namespace esl
