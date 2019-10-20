#pragma once
#include <esl/dense/type_traits.hpp>
#include <esl/sparse/utility/mkl_helpers.hpp>
#include <esl/sparse/utility/mkl_overloads.hpp>

#include <type_traits>

namespace esl::internal
{
class Fn_expr_mkl_sparse_mat_mul
{
public:
	template<class Expr_dst, class Expr_src>
	void operator()(Expr_dst& dst, const Expr_src& src) const
	{
		(*this)(dst, src.expr1(), src.expr2());
	}

private:
	template<class Expr_dst, class Expr1, class Expr2>
	void operator()(Expr_dst& dst, const Expr1& expr1, const Expr2& expr2) const
	{
		static_assert(have_same_layouts<Expr_dst, Expr2>, "Incompatible layouts");

		const auto descr = mkl_matrix_descr<Expr1>();
		constexpr auto transp = transp_op_as_mkl_enum_sparse(No_transpose{});
		constexpr auto layout = layout_as_mkl_enum_sparse(Layout_tag<Expr_dst>{});
		mkl_sparse_mm(transp, 1., expr1.handle(), descr, layout, expr2.data(), expr2.cols(),
			expr2.lead_dim(), 0., dst.data(), dst.lead_dim());
	}
};
} // namespace esl::internal
