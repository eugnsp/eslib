#pragma once
#include <esl/dense/expr.hpp>
#include <esl/dense/expression.hpp>
#include <esl/dense/forward.hpp>
//#include <esl/dense/functor.hpp>
#include <esl/dense/expr/add.hpp>
#include <esl/dense/functor/type_traits.hpp>
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/dense/view/range.hpp>
#include <esl/dense/view/slice.hpp>

#include <array>
#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace esl
{
template<class Expr, class Category>
class Dense : public Expression<Expr>
{
private:
	using Base = Expression<Expr>;

public:
	using Value = Value_type<Expr>;
	using Layout = Layout_tag<Expr>;

public:
	using Base::self;

	//////////////////////////////////////////////////////////////////////
	//> Extents

	// Returns the number of rows
	using Base::rows;

	// Returns the number of columns
	using Base::cols;

	///////////////////////////////////////////////////////////////////////
	//> Operators

	template<class Scalar, typename = std::enable_if_t<std::is_convertible_v<Scalar, Value>>>
	Dense& operator=(const Scalar& scalar)
	{
		assign_scalar(scalar);
		return *this;
	}

	template<class Expr2>
	Dense& operator=(const Expression<Expr2>& expr)
	{
		assign_expr(expr);
		return *this;
	}

	// template<typename Scalar, typename = std::void_t<decltype(std::declval<Value>() += std::declval<Scalar>())>>
	// Dense& operator+=(const Scalar& expr)
	// {
	// 	static_assert(internal::is_lvalue<Category>, "Expression should be an l-value");

	// 	internal::Add_equal<Expr, Expr2>::run(self(), expr);
	// 	return *this;
	// }

	template<class Expr2>
	Dense& operator+=(const Expression<Expr2>& expr)
	{
		static_assert(internal::is_lvalue<Category>, "Expression should be an l-value");
		static_assert(internal::is_extent_dynamic_or_eq(ct_rows_value<Expr>, ct_rows_value<Expr2>));
		static_assert(internal::is_extent_dynamic_or_eq(ct_cols_value<Expr>, ct_cols_value<Expr2>));
		assert(rows() == expr.rows());
		assert(cols() == expr.cols());

		internal::Fn_expr_add_assign_type<Expr, Expr2>{}(self(), expr.self());
		return *this;
	}

	template<class Expr2>
	Dense& operator-=(const Expression<Expr2>& expr)
	{
		static_assert(internal::is_lvalue<Category>, "Expression should be an l-value");
		static_assert(internal::is_extent_dynamic_or_eq(ct_rows_value<Expr>, ct_rows_value<Expr2>));
		static_assert(internal::is_extent_dynamic_or_eq(ct_cols_value<Expr>, ct_cols_value<Expr2>));
		assert(rows() == expr.rows());
		assert(cols() == expr.cols());

		internal::Fn_expr_sub_assign_type<Expr, Expr2>{}(self(), expr.self());
		return *this;
	}

	template<class Scalar>
	Dense& operator*=(const Scalar& scalar)
	{
		static_assert(internal::is_lvalue<Category>, "Expression should be an l-value");

		internal::Fn_scalar_mul_assign_type<Expr, Scalar>{}(self(), scalar);
		return *this;
	}

	template<class Scalar>
	Dense& operator/=(const Scalar& scalar)
	{
		static_assert(internal::is_lvalue<Category>, "Expression should be an l-value");

		internal::Fn_scalar_div_assign_type<Expr, Scalar>{}(self(), scalar);
		return *this;
	}

	template<class Scalar>
	void assign_scalar(const Scalar& scalar)
	{
		static_assert(internal::is_lvalue<Category>, "Expression should be an l-value");

		internal::Fn_scalar_assign_type<Expr, Scalar>{}(self(), scalar);
	}

	template<class Expr2>
	void assign_expr(const Expression<Expr2>& expr)
	{
		static_assert(internal::is_lvalue<Category>, "Expression should be an l-value");
		static_assert(internal::is_extent_dynamic_or_eq(ct_rows_value<Expr>, ct_rows_value<Expr2>));
		static_assert(internal::is_extent_dynamic_or_eq(ct_cols_value<Expr>, ct_cols_value<Expr2>));
		assert(rows() == expr.rows());
		assert(cols() == expr.cols());

		internal::Fn_expr_assign_type<Expr, Expr2>{}(self(), expr.self());
	}

	///////////////////////////////////////////////////////////////////////
	//> Element access

	// Returns the matrix element (row, col)
	decltype(auto) operator()(const std::size_t row, const std::size_t col)
	{
		return self()(row, col);
	}

	// Returns the matrix element (row, col)
	decltype(auto) operator()(const std::size_t row, const std::size_t col) const
	{
		return self()(row, col);
	}

	// Returns the matrix element (index, 0)
	decltype(auto) operator[](const std::size_t index)
	{
		static_assert(internal::is_vector<Expr>, "Expression should be a vector");
		return self()(index, 0);
	}

	// Returns the matrix element (index, 0)
	decltype(auto) operator[](const std::size_t index) const
	{
		static_assert(internal::is_vector<Expr>, "Expression should be a vector");
		return self()(index, 0);
	}

	// Returns the matrix element (index, 0)
	decltype(auto) operator()(const std::size_t index)
	{
		return (*this)[index];
	}

	// Returns the matrix element (index, 0)
	decltype(auto) operator()(const std::size_t index) const
	{
		return (*this)[index];
	}

	template<class Layout = Col_major>
	auto eval() const
	{
		return Matrix<Value, ct_rows_value<Expr>, ct_cols_value<Expr>, Layout>{self()};
	}

	// 	template<class Other>
	// 	bool operator==(const Expr<Other>& other) const
	// 	{
	// 		if (!ct_size_equal_v<ct_rows, ct_rows_v<Other>> ||
	// 			!ct_size_equal_v<ct_cols, ct_cols_v<Other>> ||
	// 			n_rows() != other.n_rows() ||
	// 			n_cols() != other.n_cols())
	// 		{
	// 			return false;
	// 		}
	//
	// 		for (std::size_t col = 0; col < other.n_cols(); ++col)
	// 			for (std::size_t row = 0; row < other.n_rows(); ++row)
	// 				if ((*this)(row, col) != other.self()(row, col))
	// 					return false;
	//
	// 		return true;
	// 	}
	//
	// 	template<class Other>
	// 	bool operator!=(const Expr<Other>& other) const
	// 	{
	// 		return !(*this == other);
	// 	}

	///////////////////////////////////////////////////////////////////////
	//> Block views

	template<std::size_t start_row, std::size_t start_col, std::size_t rows, std::size_t cols>
	auto view()
	{
		using Rows = internal::Range<start_row, rows>;
		using Cols = internal::Range<start_col, cols>;
		return view_impl(Rows{}, Cols{});
	}

	template<std::size_t start_row, std::size_t start_col, std::size_t rows, std::size_t cols>
	auto view() const
	{
		using Rows = internal::Range<start_row, rows>;
		using Cols = internal::Range<start_col, cols>;
		return view_impl(Rows{}, Cols{});
	}

	template<std::size_t start_row, std::size_t start_col, std::size_t rows, std::size_t cols>
	auto cview() const
	{
		return view<start_row, start_col, rows, cols>();
	}

	///////////////////////////////////////////////////////////////////////

	auto view(const std::size_t start_row, const std::size_t start_col, const std::size_t rows, const std::size_t cols)
	{
		using Range = internal::Range<dynamic, dynamic>;
		return view_impl(Range{start_row, rows}, Range{start_col, cols});
	}

	auto view(
		const std::size_t start_row, const std::size_t start_col, const std::size_t rows, const std::size_t cols) const
	{
		using Range = internal::Range<dynamic, dynamic>;
		return view_impl(Range{start_row, rows}, Range{start_col, cols});
	}

	auto cview(
		const std::size_t start_row, const std::size_t start_col, const std::size_t rows, const std::size_t cols) const
	{
		return view(start_row, start_col, rows, cols);
	}

	///////////////////////////////////////////////////////////////////////
	//> Row views

	template<std::size_t index>
	auto row_view()
	{
		using Rows = internal::Range<index, 1>;
		using Cols = internal::Range<0, ct_cols_value<Expr>>;
		return view_impl(Rows{}, Cols{0, cols()});
	}

	template<std::size_t index>
	auto row_view() const
	{
		using Rows = internal::Range<index, 1>;
		using Cols = internal::Range<0, ct_cols_value<Expr>>;
		return view_impl(Rows{}, Cols{0, cols()});
	}

	template<std::size_t index>
	auto row_cview() const
	{
		return row_view<index>();
	}

	///////////////////////////////////////////////////////////////////////

	auto row_view(std::size_t index)
	{
		using Rows = internal::Range<dynamic, 1>;
		using Cols = internal::Range<0, ct_cols_value<Expr>>;
		return view_impl(Rows{index}, Cols{0, cols()});
	}

	auto row_view(std::size_t index) const
	{
		using Rows = internal::Range<dynamic, 1>;
		using Cols = internal::Range<0, ct_cols_value<Expr>>;
		return view_impl(Rows{index}, Cols{0, cols()});
	}

	auto row_cview(std::size_t index) const
	{
		return row_view(index);
	}

	///////////////////////////////////////////////////////////////////////

	template<std::size_t start_row, std::size_t rows>
	auto rows_view()
	{
		using Rows = internal::Range<start_row, rows>;
		using Cols = internal::Range<0, ct_cols_value<Expr>>;
		return view_impl(Rows{}, Cols{0, cols()});
	}

	template<std::size_t start_row, std::size_t rows>
	auto rows_view() const
	{
		using Rows = internal::Range<start_row, rows>;
		using Cols = internal::Range<0, ct_cols_value<Expr>>;
		return view_impl(Rows{}, Cols{0, cols()});
	}

	template<std::size_t start_row, std::size_t rows>
	auto rows_cview() const
	{
		return rows_view<start_row, rows>();
	}

	///////////////////////////////////////////////////////////////////////

	auto rows_view(std::size_t start_row, std::size_t rows)
	{
		using Rows = internal::Range<dynamic, dynamic>;
		using Cols = internal::Range<0, ct_cols_value<Expr>>;
		return view_impl(Rows{start_row, rows}, Cols{0, cols()});
	}

	auto rows_view(std::size_t start_row, std::size_t rows) const
	{
		using Rows = internal::Range<dynamic, dynamic>;
		using Cols = internal::Range<0, ct_cols_value<Expr>>;
		return view_impl(Rows{start_row, rows}, Cols{0, cols()});
	}

	auto rows_cview(std::size_t start_row, std::size_t rows) const
	{
		return rows_view(start_row, rows);
	}

	///////////////////////////////////////////////////////////////////////
	//> Column views

	template<std::size_t index>
	auto col_view()
	{
		using Rows = internal::Range<0, ct_rows_value<Expr>>;
		using Cols = internal::Range<index, 1>;
		return view_impl(Rows{0, rows()}, Cols{});
	}

	template<std::size_t index>
	auto col_view() const
	{
		using Rows = internal::Range<0, ct_rows_value<Expr>>;
		using Cols = internal::Range<index, 1>;
		return view_impl(Rows{0, rows()}, Cols{});
	}

	template<std::size_t index>
	auto col_cview() const
	{
		return col_view<index>();
	}

	///////////////////////////////////////////////////////////////////////

	auto col_view(std::size_t index)
	{
		using Rows = internal::Range<0, ct_rows_value<Expr>>;
		using Cols = internal::Range<dynamic, 1>;
		return view_impl(Rows{0, rows()}, Cols{index});
	}

	auto col_view(std::size_t index) const
	{
		using Rows = internal::Range<0, ct_rows_value<Expr>>;
		using Cols = internal::Range<dynamic, 1>;
		return view_impl(Rows{0, rows()}, Cols{index});
	}

	auto col_cview(std::size_t index) const
	{
		return col_view(index);
	}

	///////////////////////////////////////////////////////////////////////

	template<std::size_t start_col, std::size_t cols>
	auto cols_view()
	{
		using Rows = internal::Range<0, ct_rows_value<Expr>>;
		using Cols = internal::Range<start_col, cols>;
		return view_impl(Rows{0, rows()}, Cols{});
	}

	template<std::size_t start_col, std::size_t cols>
	auto cols_view() const
	{
		using Rows = internal::Range<0, ct_rows_value<Expr>>;
		using Cols = internal::Range<start_col, cols>;
		return view_impl(Rows{0, rows()}, Cols{});
	}

	template<std::size_t start_col, std::size_t cols>
	auto cols_cview() const
	{
		return cols_view<start_col, cols>();
	}

	///////////////////////////////////////////////////////////////////////

	auto cols_view(std::size_t start_col, std::size_t cols)
	{
		using Rows = internal::Range<0, ct_rows_value<Expr>>;
		using Cols = internal::Range<dynamic, dynamic>;
		return view_impl(Rows{0, rows()}, Cols{start_col, cols});
	}

	auto cols_view(std::size_t start_col, std::size_t cols) const
	{
		using All_rows = internal::Range<0, ct_rows_value<Expr>>;
		using Cols = internal::Range<dynamic, dynamic>;
		return view_impl(All_rows{0, rows()}, Cols{start_col, cols});
	}

	auto cols_cview(std::size_t start_col, std::size_t cols) const
	{
		return cols_view(start_col, cols);
	}

	///////////////////////////////////////////////////////////////////////
	//> Transposed view

	auto tr_view()
	{
		return Transposed_view<Expr, Category>{self()};
	}

	auto tr_view() const
	{
		return Transposed_view<const Expr, Category>{self()};
	}

	auto tr_cview() const
	{
		return tr_view();
	}

	///////////////////////////////////////////////////////////////////////
	//> Diagonal view

	auto diag_view()
	{
		return Diag_view<Expr, Category>{self()};
	}

	auto diag_view() const
	{
		return Diag_view<const Expr, Category>{self()};
	}

	auto diag_cview() const
	{
		return diag_view();
	}

	// template<std::size_t t_size>
	// auto col(Vector<std::size_t, t_size> indices)
	// {
	// 	return col(internal::Slice<t_size>{std::move(indices)});
	// }

	// template<std::size_t t_size>
	// auto row(Vector<std::size_t, t_size> indices)
	// {
	// 	return row(internal::Slice<t_size>{std::move(indices)});
	// }

	// 	template<std::size_t rows, std::size_t cols>
	// 	auto view(Matrix<std::size_t, rows, cols> indices)
	// 	{
	// 		//return col(Matrix_slice<rows, cols>{std::move(indices)});
	// 	}

	// template<std::size_t t_size>
	// auto col(Vector<std::size_t, t_size> indices) const
	// {
	// 	return col(internal::Slice<t_size>{std::move(indices)});
	// }

	// template<std::size_t t_size>
	// auto ccol(Vector<std::size_t, t_size> indices) const
	// {
	// 	return col(std::move(indices));
	// }

	// Row views
	// template<class TRows, typename = std::enable_if_t<internal::is_indexer_v<TRows>>>
	// auto row(TRows rows) const
	// {
	// 	return view(std::move(rows), internal::Range<dynamic, ct_cols_value<Expr>>{0, cols()});
	// }

	// template<std::size_t t_size>
	// auto row(Vector<std::size_t, t_size> indices) const
	// {
	// 	return row(internal::Slice<t_size>{std::move(indices)});
	// }

	// template<std::size_t t_size>
	// auto crow(Vector<std::size_t, t_size> indices) const
	// {
	// 	return row(std::move(indices));
	// }

	// TODO
	// template<typename T_Value, typename = std::enable_if_t<!is_any_expr_v<T_Value>>>
	//                                                                           V !!!
	// template<typename T_Value, typename = std::void_t<decltype(std::declval<Value&>() += std::declval<T_Value&>())>>
	// Dense& operator+=(const T_Value& value)
	// {
	// 	for (std::size_t col = 0; col < cols(); ++col)
	// 		for (std::size_t row = 0; row < rows(); ++row)
	// 			self()(row, col) += value;
	// 	return *this;
	// }

	// template<typename T_Value, typename = std::void_t<decltype(std::declval<Value&>() -= std::declval<T_Value&>())>>
	// Dense& operator-=(const T_Value& value)
	// {
	// 	for (std::size_t col = 0; col < cols(); ++col)
	// 		for (std::size_t row = 0; row < rows(); ++row)
	// 			self()(row, col) -= value;
	// 	return *this;
	// }

private:
	template<class Rows, class Cols>
	auto view_impl(Rows rows, Cols cols)
	{
		return View<Expr, Rows, Cols, Category>{self(), std::move(rows), std::move(cols)};
	}

	template<class Rows, class Cols>
	auto view_impl(Rows rows, Cols cols) const
	{
		return View<const Expr, Rows, Cols, Category>{self(), std::move(rows), std::move(cols)};
	}
};
} // namespace esl
