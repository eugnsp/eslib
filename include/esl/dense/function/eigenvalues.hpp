#pragma once
#include <esl/dense/expression.hpp>
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/ct_extent.hpp>
#include <esl/dense/utility/mkl_overloads.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>

namespace esl
{
namespace internal
{
template<class Expr1, class Mkl_tag1, class Expr2, class Mkl_tag2, typename = void>
struct Fn_eigenvalues;

template<class Expr1, class Expr2, typename Value>
struct Fn_eigenvalues<Expr1, Lvalue_block_tag<Value>, Expr2, Lvalue_block_tag<Value>,
	std::enable_if_t<is_fd<Value> && is_vector<Expr2>>>
{
	void operator()(Expr1& mat, Expr2& eigenvalues)
	{
		assert(eigenvalues.row_stride() == 1);

		const auto n = mat.rows();
		Vector_x<Value> tau(n), off_diag(n - 1);

		internal::mkl_lapack_sytrd<Layout_tag<Expr1>>(
			n, mat.data(), mat.lead_dim(), eigenvalues.data(), off_diag.data(), tau.data());
		internal::mkl_lapack_sterf(n, eigenvalues.data(), off_diag.data());
	}
};
} // namespace internal

template<class Expr1, class Expr2>
void eigenvalues(Dense<Expr1, Lvalue>& mat, Dense<Expr2, Lvalue>& eigenvalues)
{
	static_assert(internal::is_extent_dynamic_or_eq(ct_rows_value<Expr1>, ct_cols_value<Expr1>));
	static_assert(internal::is_extent_dynamic_or_eq(ct_rows_value<Expr1>, ct_rows_value<Expr2>));
	assert(mat.rows() == mat.cols());
	assert(mat.rows() == eigenvalues.size());

	internal::Fn_eigenvalues<Expr1, internal::Mkl_expr_decay<Expr1>, Expr2, internal::Mkl_expr_decay<Expr2>>{}(
		mat.self(), eigenvalues.self());
}
} // namespace esl
