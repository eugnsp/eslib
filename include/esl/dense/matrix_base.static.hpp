#pragma once
#include <esl/dense/concepts.hpp>
#include <esl/dense/dense.hpp>
#include <esl/dense/shape.hpp>
#include <esl/dense/storage/static_storage.hpp>
#include <esl/dense/type_traits.hpp>

#include <esu/array.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <type_traits>

namespace esl::internal
{
template<std::size_t ct_rows, std::size_t ct_cols, class Expr, class Layout>
class Matrix_base : public Dense<Expr>, public Shape<ct_rows, ct_cols, Layout>
{
private:
	using Dense_base = Dense<Expr>;
	using Shape_base = Shape<ct_rows, ct_cols, Layout>;

public:
	using Value = Value_type<Expr>;

public:
	////////////////////////////////////////////////////////////////////////////////
	/** Constructors */

	Matrix_base() = default;
	Matrix_base(const Matrix_base&) = default;
	Matrix_base(Matrix_base&&) = default;

	explicit Matrix_base(const Value& value)
	{
		Dense_base::assign_scalar(value);
	}

	#ifdef __cpp_concepts
	template<typename... Values>
	requires sizeof...(Values) == ct_rows * ct_cols && (... && std::is_convertible_v<Values, Value>)
	#else
	template<typename... Values, typename = std::enable_if_t<
				 sizeof...(Values) == ct_rows * ct_cols && (... && std::is_convertible_v<Values, Value>)>>
	#endif
	explicit constexpr Matrix_base(Values&&... values)
		: data_{std::forward<Values>(values)...}
	{}

	template<class Expr2>
	Matrix_base(const Expression<Expr2>& expr)
	{
		Dense_base::assign_expr(expr);
	}

	////////////////////////////////////////////////////////////////////////////////
	/** Assignments */

	using Dense_base::operator=;

	Matrix_base& operator=(const Matrix_base&) = default;
	Matrix_base& operator=(Matrix_base&&) = default;

	////////////////////////////////////////////////////////////////////////////////
	/** Extents */

	using Shape_base::cols;
	using Shape_base::rows;

	static constexpr bool is_empty()
	{
		return rows() == 0 || cols() == 0;
	}

	static constexpr std::size_t size()
	{
		return rows() * cols();
	}

	static constexpr std::size_t capacity()
	{
		return size();
	}

	////////////////////////////////////////////////////////////////////////////////
	/** Element access */

	// Returns the matrix element
	constexpr Value& operator()(const std::size_t row, const std::size_t col)
	{
		return data_[this->linear_index(row, col)];
	}

	// Returns the matrix element
	constexpr const Value& operator()(const std::size_t row, const std::size_t col) const
	{
		return data_[this->linear_index(row, col)];
	}

	constexpr Value& operator[](const std::size_t index)
	#ifdef ESL_CONCEPTS
	requires internal::is_vector<Expr>
	#endif
	{
		#ifndef ESL_CONCEPTS
		static_assert(internal::is_vector<Expr>);
		#endif

		return (*this)(index, 0);
	}

	constexpr const Value& operator[](const std::size_t index) const
	#ifdef ESL_CONCEPTS
	requires internal::is_vector<Expr>
	#endif
	{
		#ifndef ESL_CONCEPTS
		static_assert(internal::is_vector<Expr>);
		#endif

		return (*this)(index, 0);
	}

	constexpr Value& operator()(const std::size_t index)
	{
		return (*this)[index];
	}

	constexpr const Value& operator()(const std::size_t index) const
	{
		return (*this)[index];
	}

	////////////////////////////////////////////////////////////////////////////////
	/** Modifiers */

	void swap(Matrix_base& other) noexcept(std::is_nothrow_swappable_v<Value>)
	{
		data_.swap(other.data_);
	}

	static void resize(const std::size_t rows, const std::size_t cols)
	{
		assert(rows == ct_rows);
		assert(cols == ct_cols);
	}

protected:
	Static_storage<Value, ct_rows * ct_cols> data_;
};
} // namespace esl::internal
