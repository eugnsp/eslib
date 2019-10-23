#pragma once
#include <esl/dense/tags.hpp>

#include <cassert>
#include <cstddef>
#include <utility>

namespace esl::internal
{
template<std::size_t ct_rows, std::size_t ct_cols>
class Shape_base
{
public:
	static constexpr std::size_t rows()
	{
		return rows_;
	}

	static constexpr std::size_t cols()
	{
		return cols_;
	}

protected:
	constexpr Shape_base() = default;

	static void set(std::size_t rows, std::size_t cols)
	{
		assert(rows == ct_rows);
		assert(cols == ct_cols);
	}

private:
	static constexpr std::size_t rows_ = ct_rows;
	static constexpr std::size_t cols_ = ct_cols;
};

template<std::size_t ct_rows>
class Shape_base<ct_rows, dynamic>
{
public:
	static constexpr std::size_t rows()
	{
		return rows_;
	}

	constexpr std::size_t cols() const
	{
		return cols_;
	}

protected:
	constexpr Shape_base(std::size_t rows, std::size_t cols) noexcept : cols_(cols)
	{
		assert(rows == rows_);
	}

	void set(std::size_t rows, std::size_t cols)
	{
		assert(rows == ct_rows);
		cols_ = cols;
	}

	void swap(Shape_base& other)
	{
		std::swap(cols_, other.cols_);
	}

private:
	static constexpr std::size_t rows_ = ct_rows;
	std::size_t cols_;
};

template<std::size_t ct_cols>
class Shape_base<dynamic, ct_cols>
{
public:
	constexpr std::size_t rows() const
	{
		return rows_;
	}

	static constexpr std::size_t cols()
	{
		return cols_;
	}

protected:
	constexpr Shape_base(std::size_t rows, std::size_t cols) noexcept : rows_(rows)
	{
		assert(cols == cols_);
	}

	void set(std::size_t rows, std::size_t cols)
	{
		rows_ = rows;
		assert(cols == ct_cols);
	}

	void swap(Shape_base& other) noexcept
	{
		std::swap(rows_, other.rows_);
	}

private:
	std::size_t rows_;
	static constexpr std::size_t cols_ = ct_cols;
};

template<>
class Shape_base<dynamic, dynamic>
{
public:
	constexpr std::size_t rows() const
	{
		return rows_;
	}

	constexpr std::size_t cols() const
	{
		return cols_;
	}

protected:
	constexpr Shape_base(std::size_t rows, std::size_t cols) noexcept : rows_(rows), cols_(cols)
	{}

	void set(std::size_t rows, std::size_t cols)
	{
		rows_ = rows;
		cols_ = cols;
	}

	void swap(Shape_base& other) noexcept
	{
		std::swap(rows_, other.rows_);
		std::swap(cols_, other.cols_);
	}

private:
	std::size_t rows_;
	std::size_t cols_;
};
} // namespace esl::internal
