#pragma once
#include <esl/dense/matrix_base.dynamic.hpp>
#include <esl/dense/matrix_base.static.hpp>
#include <esl/dense/type_traits.hpp>

#include <esu/type_traits.hpp>

#include <array>
#include <cstddef>
#include <type_traits>

namespace esl
{
template<typename Value, std::size_t ct_rows, std::size_t ct_cols, class Layout>
class Matrix : public internal::Matrix_base<ct_rows, ct_cols, Matrix<Value, ct_rows, ct_cols, Layout>, Layout>
{
	static_assert(!std::is_const_v<Value>);
	static_assert(!std::is_reference_v<Value>);

private:
	using Base = internal::Matrix_base<ct_rows, ct_cols, Matrix<Value, ct_rows, ct_cols, Layout>, Layout>;

public:
	//> Constructors

	using Base::Base;

	Matrix() = default;
	Matrix(const Matrix&) = default;
	Matrix(Matrix&&) = default;

	// template<class Expr>
	// Matrix(const Expression<Expr>& expr)
	// {
	// 	Base::assign_expr(expr);
	// }

	///////////////////////////////////////////////////////////////////////
	//> Assignments

	using Base::operator=;

	Matrix& operator=(const Matrix&) = default;
	Matrix& operator=(Matrix&&) = default;

	////////////////////////////////////////////////////////////////////////
	//> Extents

	using Base::cols;
	using Base::rows;

	// Returns the increment used to determine the starting
	// position of the matrix element in the next row of the same column
	std::size_t row_stride() const
	{
		return is_col_major<Layout> ? 1 : cols();
	}

	// Returns the increment used to determine the starting
	// position of the matrix element in the next column of the same row
	std::size_t col_stride() const
	{
		return is_row_major<Layout> ? 1 : rows();
	}

	// Returns the leading dimension of a matrix
	std::size_t lead_dim() const
	{
		return is_col_major<Layout> ? rows() : cols();
	}

	std::size_t memory_size() const
	{
		return Base::capacity() * sizeof(Value);
	}

	//////////////////////////////////////////////////////////////////////
	//> Element access

	// Returns the pointer to the underlying array of matrix element storage
	Value* data() noexcept
	{
		return data_.data();
	}

	// Returns the pointer to the underlying array of matrix element storage
	const Value* data() const noexcept
	{
		return data_.data();
	}

private:
	using Base::data_;
};

template<class Expr>
Matrix(const Expression<Expr>&)->Matrix<Value_type<Expr>, ct_rows_value<Expr>, ct_cols_value<Expr>>;

///////////////////////////////////////////////////////////////////////
//> Type traits

template<typename Value_, std::size_t rows_, std::size_t cols_, class Layout_>
struct Traits<Matrix<Value_, rows_, cols_, Layout_>>
{
	using Value = Value_;
	using Layout = Layout_;

	static constexpr std::size_t rows = rows_;
	static constexpr std::size_t cols = cols_;

	static constexpr std::size_t row_stride = is_col_major<Layout> ? 1 : cols;
	static constexpr std::size_t col_stride = is_row_major<Layout> ? 1 : rows;
	static constexpr std::size_t lead_dim = is_col_major<Layout> ? rows : cols;
};
} // namespace esl
