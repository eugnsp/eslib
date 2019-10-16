#pragma once
#include <esl/dense/type_traits/expr_storage.hpp>
#include <esl/sparse/forward.hpp>

#include <type_traits>

namespace esl::internal
{
template<typename Value, class Symmetry, typename Index>
inline constexpr bool store_expr_as_reference<Csr_matrix<Value, Symmetry, Index>> = true;

template<class Matrix>
inline constexpr bool store_expr_as_reference<Mkl_sparse_matrix<Matrix>> = true;
} // namespace esl::internal
