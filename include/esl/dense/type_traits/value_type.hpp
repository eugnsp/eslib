#pragma once
#include <esl/dense/forward.hpp>

#include <esu/type_traits.hpp>

namespace esl
{
template<class Expr>
using Value_type = typename Traits<esu::Remove_cv_ref<Expr>>::Value;

namespace internal
{
template<class... Exprs>
inline constexpr bool have_same_value_types = esu::are_same<Value_type<Exprs>...>;
}
} // namespace esl
