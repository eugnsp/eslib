#pragma once
#include <esl/dense/utility/mkl_overloads.hpp>

namespace esl::internal
{
// Tag for the X += Y operation
struct Fn_expr_mkl_vec_axpy_add_assign_tag
{};

template<class Tag>
class Fn_expr_mkl_vec_axpy
{
public:
	template<class Expr_dst, class Expr_src>
	void operator()(Expr_dst& dst, const Expr_src& src) const
	{
		(*this)(dst, src, Tag{});
	}

private:
	// template<class Expr1, class Expr2>
	// using Transp_tag = std::conditional_t<have_same_layout<Expr1, Expr2>, No_transpose, Transpose>;

	template<class Expr_dst, class Expr_src>
	void operator()(Expr_dst& dst, const Expr_src& src, Fn_expr_mkl_vec_axpy_add_assign_tag) const
	{
		(*this)(dst, src, 1);
	}

	template<class Expr_dst, class Expr_src, typename Alpha>
	void operator()(Expr_dst& dst, const Expr_src& src, Alpha alpha) const
	{
		mkl_axpy(dst.size(), alpha, src.data(), src.row_stride(), dst.data(), dst.row_stride());
	}
};
} // namespace esl::internal
