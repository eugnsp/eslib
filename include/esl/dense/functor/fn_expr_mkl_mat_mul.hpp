#pragma once
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/mkl_overloads.hpp>

#include <type_traits>

namespace esl::internal
{
// Tag for the X = Y * Z operation
struct Fn_expr_mkl_mat_mul_assign_tag
{};

// Tag for the X += Y * Z operation
struct Fn_expr_mkl_mat_mul_add_assign_tag
{};

template<class Tag>
class Fn_expr_mkl_mat_mul
{
public:
	template<class Expr_dst, class Expr_src>
	void operator()(Expr_dst& dst, const Expr_src& src) const
	{
		(*this)(dst, src, Tag{});
	}

private:
	template<class Expr1, class Expr2>
	using Transp_tag = std::conditional_t<have_same_layouts<Expr1, Expr2>, No_transpose, Transpose>;

	template<class Expr_dst, class Expr_src>
	void operator()(Expr_dst& dst, const Expr_src& src, Fn_expr_mkl_mat_mul_assign_tag) const
	{
		(*this)(dst, src.expr1(), src.expr2(), 0);
	}

	template<class Expr_dst, class Expr_src>
	void operator()(Expr_dst& dst, const Expr_src& src, Fn_expr_mkl_mat_mul_add_assign_tag) const
	{
		(*this)(dst, src.expr1(), src.expr2(), 1);
	}

	template<class Expr_dst, class Expr1, class Expr2, typename Beta>
	void operator()(Expr_dst& dst, const Expr1& expr1, const Expr2& expr2, const Beta beta) const
	{
		using Layout = Layout_tag<Expr_dst>;
		using Transp_a = Transp_tag<Expr_dst, Expr1>;
		using Transp_b = Transp_tag<Expr_dst, Expr2>;

		constexpr auto alpha = 1;

		if constexpr (is_vector<Expr_dst> || is_vector<Expr2>)
			mkl_gemv<Layout_tag<Expr1>, No_transpose>(expr1.rows(), expr1.cols(), alpha, expr1.data(), expr1.lead_dim(),
				expr2.data(), expr2.row_stride(), beta, dst.data(), dst.row_stride());
		else
			mkl_gemm<Layout, Transp_a, Transp_b>(dst.rows(), dst.cols(), expr1.cols(), alpha, expr1.data(),
				expr1.lead_dim(), expr2.data(), expr2.lead_dim(), beta, dst.data(), dst.lead_dim());
	}
};
} // namespace esl::internal
