#pragma once
#include <esl/dense/functor/fn.hpp>
#include <esl/dense/type_traits.hpp>

#include <cstddef>

namespace esl::internal
{
// A functor for performing an element-wise operation on a dense expression;
// applies the given elemental functor to all matrix elements and a scalar
template<class Fn>
class Fn_scalar_element_wise
{
public:
	template<class Expr, typename Scalar>
	void operator()(Expr& dst, const Scalar scalar) const
	{
		// Prefer contiguous writes to contiguous reads
		(*this)(dst, scalar, Layout_tag<Expr>{});
	}

private:
	template<class Expr, typename Scalar>
	void operator()(Expr& dst, const Scalar scalar, Col_major) const
	{
		for (std::size_t col = 0; col < dst.cols(); ++col)
			for (std::size_t row = 0; row < dst.rows(); ++row)
				Fn{}(dst(row, col), scalar);
	}

	template<class Expr, typename Scalar>
	void operator()(Expr& dst, const Scalar scalar, Row_major) const
	{
		for (std::size_t row = 0; row < dst.rows(); ++row)
			for (std::size_t col = 0; col < dst.cols(); ++col)
				Fn{}(dst(row, col), scalar);
	}
};

// A functor that assigns a scalar to all matrix elements of a dense expression
using Fn_scalar_assign_loop = Fn_scalar_element_wise<Fn_assign>;

// A functor that adds a scalar to all matrix elements of a dense expression
using Fn_scalar_add_assign_loop = Fn_scalar_element_wise<Fn_add_assign>;

// A functor that subtracts a scalar from all matrix elements of a dense expression
using Fn_scalar_sub_assign_loop = Fn_scalar_element_wise<Fn_sub_assign>;

// A functor that multiplies all matrix elements of a dense expression by a scalar
using Fn_scalar_mul_assign_loop = Fn_scalar_element_wise<Fn_mul_assign>;

// A functor that divides all matrix elements of a dense expression by a scalar
using Fn_scalar_div_assign_loop = Fn_scalar_element_wise<Fn_div_assign>;
} // namespace esl::internal
