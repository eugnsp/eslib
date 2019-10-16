#pragma once
#include <esl/dense/dense.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/ct_extent.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace esl
{
template<class Expr, class Rows, class Cols, class Category>
class View : public Dense<View<Expr, Rows, Cols, Category>, Category>
{
public:
	template<class Expr_f>
	View(Expr_f&& expr, Rows rows, Cols cols) :
		expr_(std::forward<Expr_f>(expr)), rows_(std::move(rows)), cols_(std::move(cols))
	{
		// static_assert(is_ct_extent_dynamic_or_less_equal(ct_size_value<Rows>, ct_rows_value<Expr>), "Incompatible extents");
		// static_assert(is_ct_extent_dynamic_or_less_equal(ct_size_value<Cols>, ct_cols_value<Expr>), "Incompatible extents");

		// assert(rows.are_all_less(expr.rows()));
		// assert(cols.are_all_less(expr.cols()));
	}

	View(const View&) = default;
	View(View&&) = default;

	View& operator=(const View& other)
	{
		static_assert(std::is_same_v<Category, Lvalue>, "Expression should be an l-value");
		this->assign_expr(other);
		return *this;
	}

	template<class Other>
	View& operator=(const Expression<Other>& other)
	{
		static_assert(std::is_same_v<Category, Lvalue>, "Expression should be an l-value");
		this->assign_expr(other);
		return *this;
	}

	//////////////////////////////////////////////////////////////////////
	//* Extents */

	std::size_t rows() const
	{
		return rows_.size();
	}

	std::size_t cols() const
	{
		return cols_.size();
	}

	std::size_t lead_dim() const
	{
		static_assert(is_lvalue_block<View>, "View should be an l-value block");
		return expr_.lead_dim();
	}

	std::size_t row_stride() const
	{
		static_assert(is_lvalue_block<View>, "View should be an l-value block");
		return expr_.row_stride();
	}

	std::size_t col_stride() const
	{
		static_assert(is_lvalue_block<View>, "View should be an l-value block");
		return expr_.col_stride();
	}

	//////////////////////////////////////////////////////////////////////
	//* Element access */

	decltype(auto) operator()(std::size_t row, std::size_t col)
	{
		return expr_(rows_[row], cols_[col]);
	}

	decltype(auto) operator()(std::size_t row, std::size_t col) const
	{
		if constexpr (std::is_same_v<Category, Lvalue>)
			return std::as_const(expr_(rows_[row], cols_[col]));
		else
			return expr_(rows_[row], cols_[col]);
	}

	auto data()
	{
		static_assert(is_lvalue_block<View>, "View should be an l-value block");
		if constexpr (is_col_major<View>)
			return expr_.data() + cols_.start() * lead_dim() + rows_.start();
		else
			return expr_.data() + rows_.start() * lead_dim() + cols_.start();
	}

	auto data() const
	{
		static_assert(is_lvalue_block<View>, "View should be an l-value block");
		if constexpr (is_col_major<View>)
			return &std::as_const(*(expr_.data() + cols_.start() * lead_dim() + rows_.start()));
		else
			return &std::as_const(*(expr_.data() + rows_.start() * lead_dim() + cols_.start()));
	}

private:
	internal::Expr_storage_type<Expr> expr_;

	const Rows rows_;
	const Cols cols_;
};

///////////////////////////////////////////////////////////////////////
//> Type traits

template<class Expr, class Rows, class Cols, class Category>
struct Traits<View<Expr, Rows, Cols, Category>>
{
	using Value = Value_type<Expr>;
	using Layout = Layout_tag<Expr>;

	static constexpr std::size_t rows = ct_size_value<Rows>;
	static constexpr std::size_t cols = ct_size_value<Cols>;
};
} // namespace esl
