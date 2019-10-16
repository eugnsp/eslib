#pragma once
#include <esl/dense/dense.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/ct_extent.hpp>

#include <algorithm>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace esl
{
template<class Expr, class Category>
class Diag_view : public Dense<Diag_view<Expr, Category>, Category>
{
public:
	using Traversal_order = Col_major;

private:
	using Base = Dense<Diag_view<Expr, Category>, Category>;

public:
	template<class Expr_f>
	Diag_view(Expr_f&& expr) : expr_(std::forward<Expr_f>(expr))
	{}

	Diag_view(const Diag_view&) = default;
	Diag_view(Diag_view&&) = default;

	using Base::operator=;

	Diag_view& operator=(const Diag_view& other)
	{
		static_assert(std::is_same_v<Category, Lvalue>, "Expression should be an l-value");
		this->assign_expr(other);
		return *this;
	}

	// template<class Other>
	// Diag_view& operator=(const Expression<Other>& other)
	// {
	// 	static_assert(std::is_same_v<Category, Lvalue>, "Expression should be an l-value");
	// 	this->assign_expr(other);
	// 	return *this;
	// }

	//////////////////////////////////////////////////////////////////////
	//* Extents */

	std::size_t rows() const
	{
		return std::min(expr_.rows(), expr_.cols());
	}

	static constexpr std::size_t cols()
	{
		return 1;
	}

	std::size_t lead_dim() const
	{
		static_assert(is_lvalue_block<Diag_view>, "View should be an l-value block");
		return expr_.lead_dim() + 1;
	}

	std::size_t row_stride() const
	{
		return lead_dim();
	}

	std::size_t col_stride() const
	{
		return 0;
	}

	//////////////////////////////////////////////////////////////////////
	//* Element access */

	decltype(auto) operator()(std::size_t row, std::size_t col)
	{
		assert(col == 0);
		return expr_(row, row);
	}

	decltype(auto) operator()(std::size_t row, std::size_t col) const
	{
		assert(col == 0);
		if constexpr (std::is_same_v<Category, Lvalue>)
			return std::as_const(expr_(row, row));
		else
			return expr_(row, row);
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
//> Traits

template<class Expr, class Category>
struct Traits<Diag_view<Expr, Category>>
{
	using Value = Value_type<Expr>;
	using Layout = Col_major;

	static constexpr std::size_t rows = std::min(ct_rows_value<Expr>, ct_cols_value<Expr>);
	static constexpr std::size_t cols = 1;
};
} // namespace esl
