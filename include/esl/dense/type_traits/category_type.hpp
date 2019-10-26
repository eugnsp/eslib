#pragma once
#include <esl/dense/forward.hpp>

#include <esu/type_traits.hpp>

namespace esl
{
template<class Expr>
using Category_type = typename Traits<esu::Remove_cv_ref<Expr>>::Category;
} // namespace esl
