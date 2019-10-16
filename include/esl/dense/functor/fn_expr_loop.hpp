#pragma once
#include <esl/dense/functor/fn.hpp>
#include <esl/dense/type_traits.hpp>

#include <cstddef>

namespace esl::internal
{
// A functor for performing an element-wise operation on dense expressions;
// applies the given elemental functor to all pairs of matrix elements
template<class Fn>
class Fn_expr_element_wise
{
public:
	template<class Expr_dst, class Expr_src>
	void operator()(Expr_dst& dst, const Expr_src& src) const
	{
		// Prefer contiguous writes to contiguous reads
		(*this)(dst, src, Layout_tag<Expr_dst>{});
	}

private:
	template<class Expr_dst, class Expr_src>
	void operator()(Expr_dst& dst, const Expr_src& src, Col_major) const
	{
		for (std::size_t col = 0; col < dst.cols(); ++col)
			for (std::size_t row = 0; row < dst.rows(); ++row)
				Fn{}(dst(row, col), src(row, col));
	}

	template<class Expr_dst, class Expr_src>
	void operator()(Expr_dst& dst, const Expr_src& src, Row_major) const
	{
		for (std::size_t row = 0; row < dst.rows(); ++row)
			for (std::size_t col = 0; col < dst.cols(); ++col)
				Fn{}(dst(row, col), src(row, col));
	}
};

// A functor that assigns the second dense expression to the first one using nested loops
using Fn_expr_assign_loop = Fn_expr_element_wise<Fn_assign>;

// A functor that adds the second dense expression to the first one using nested loops
using Fn_expr_add_assign_loop = Fn_expr_element_wise<Fn_add_assign>;

// A functor that subtracts the second dense expression from the first one using nested loops
using Fn_expr_sub_assign_loop = Fn_expr_element_wise<Fn_sub_assign>;
} // namespace esl::internal
