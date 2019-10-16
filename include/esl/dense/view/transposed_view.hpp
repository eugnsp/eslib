#pragma once
#include <esl/dense/dense.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/ct_extent.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace esl
{
template<class Expr, class Category>
class Transposed_view : public Dense<Transposed_view<Expr, Category>, Category>
{
public:
	template<class Expr_f>
	Transposed_view(Expr_f&& expr) : expr_(std::forward<Expr_f>(expr))
	{}

	Transposed_view(const Transposed_view&) = default;
	Transposed_view(Transposed_view&&) = default;

	Transposed_view& operator=(const Transposed_view& other)
	{
		static_assert(std::is_same_v<Category, Lvalue>, "Expression should be an l-value");
		this->assign_expr(other);
		return *this;
	}

	template<class Other>
	Transposed_view& operator=(const Expression<Other>& other)
	{
		static_assert(std::is_same_v<Category, Lvalue>, "Expression should be an l-value");
		this->assign_expr(other);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////
	//* Extents */

	std::size_t rows() const
	{
		return expr_.cols();
	}

	std::size_t cols() const
	{
		return expr_.rows();
	}

	std::size_t lead_dim() const
	{
		static_assert(is_lvalue_block<Transposed_view>, "View should be an l-value block");
		return expr_.lead_dim();
	}

	std::size_t row_stride() const
	{
		static_assert(is_lvalue_block<Transposed_view>, "View should be an l-value block");
		return expr_.col_stride();
	}

	std::size_t col_stride() const
	{
		static_assert(is_lvalue_block<Transposed_view>, "View should be an l-value block");
		return expr_.row_stride();
	}

	//////////////////////////////////////////////////////////////////////
	//* Element access */

	decltype(auto) operator()(std::size_t row, std::size_t col)
	{
		return expr_(col, row);
	}

	decltype(auto) operator()(std::size_t row, std::size_t col) const
	{
		if constexpr (std::is_same_v<Category, Lvalue>)
			return std::as_const(expr_(col, row));
		else
			return expr_(col, row);
	}

	auto data()
	{
		return expr_.data();
	}

	auto data() const
	{
		return &std::as_const(*(expr_.data()));
	}

private:
	internal::Expr_storage_type<Expr> expr_;
};

///////////////////////////////////////////////////////////////////////
//> Type traits

template<class Expr, class Category>
struct Traits<Transposed_view<Expr, Category>>
{
	using Value = Value_type<Expr>;
	using Layout = typename internal::Transpose_layout<Layout_tag<Expr>>::Type;

	static constexpr std::size_t rows = ct_cols_value<Expr>;
	static constexpr std::size_t cols = ct_rows_value<Expr>;

	static constexpr std::size_t row_stride = ct_col_stride_value<Expr>;
	static constexpr std::size_t col_stride = ct_row_stride_value<Expr>;
};
} // namespace esl
