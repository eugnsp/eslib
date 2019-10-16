#pragma once
#include "sparsity_pattern.hpp"
#include <esl/dense/type_traits.hpp>
#include <esl/sparse/csr_pattern.hpp>
#include <esl/sparse/forward.hpp>

//#include <esl/base/extended.hpp>
#include <esl/dense/matrix.hpp>
#include <esl/dense/tags.hpp>

#include <algorithm>
#include <type_traits>
#include <vector>

namespace esl
{
template<typename Value_, class Symmetry_, typename Index_>
class Csr_matrix : public Csr_pattern<Symmetry_, Index_>
{
	static_assert(std::is_integral_v<Index_>, "Bad index type");

private:
	using Base = Csr_pattern<Symmetry_, Index_>;

public:
	using Value = Value_;
	using Symmetry = Symmetry_;
	using Index = Index_;

public:
	//Csr_matrix() = default;

	// Csr_matrix(Index rows, Index cols) : rows_(rows), cols_(cols)
	// {}

	// // HACK
	// template<class T1, class T2, class T3>
	// Csr_matrix(std::size_t rows, std::size_t cols, const T1& values, const T2& row_indices,
	// 	const T3& col_indices) :
	// 	rows_(rows),
	// 	cols_(cols), values_(values.size())
	// {
	// 	//		assert(row_indices.size() == rows);
	// 	assert(col_indices.size() == values.size());

	// 	for (std::size_t i = 0; i < values.size(); ++i)
	// 		values_[i] = values[i];

	// 	row_indices_.resize(row_indices.size());
	// 	col_indices_.resize(col_indices.size());
	// 	std::copy(row_indices.begin(), row_indices.end(), row_indices_.begin());
	// 	std::copy(col_indices.begin(), col_indices.end(), col_indices_.begin());
	// }

	Value& operator[](Index index)
	{
		assert(index < nnz());
		return values_[index];
	}

	const Value& operator[](Index index) const
	{
		assert(index < nnz());
		return values_[index];
	}

	Value& operator()(Index row, Index col)
	{
		assert(row < rows_ && col < cols_);
		// 		if (isSymmetric())
		// 			assert(col >= row);

		auto start = row_indices_[row];
		auto end = row_indices_[row + 1];
		assert(start < end);

		const auto begin = col_indices_.begin();
		const auto pos = std::lower_bound(begin + start, begin + end, col);
		assert(pos != begin + end);
		assert(*pos == col);

		return values_[pos - begin];
	}

	Value at(Index row, Index col)
	{
		assert(row < rows_ && col < cols_);

		auto start = row_indices_[row];
		auto end = row_indices_[row + 1];
		assert(start < end);

		const auto begin = col_indices_.begin();
		const auto pos = std::lower_bound(begin + start, begin + end, col);

		if (pos == begin + end || *pos != col)
			return 0;
		else
			return values_[pos - begin];
	}

	// 	T_Value operator()(std::size_t row, std::size_t col) const
	// 	{
	// 		throw;
	// 	}
	//
	// void zero_row(Index row)
	// {
	// 	assert(row < rows_);

	// 	auto start = row_indices_[row];
	// 	auto end = row_indices_[row + 1];
	// 	assert(start < end);

	// 	for (auto i = start; i < end; ++i)
	// 		values_[i] = 0;
	// }

	void zero()
	{
		values_ = 0;
	}

	void resize(Index rows, Index cols)
	{
		rows_ = rows;
		cols_ = cols;
	}

	void set_sparsity_pattern(const Sparsity_pattern<Symmetry>& pattern)
	{
		assert(pattern.n_rows() == rows_);

		row_indices_.clear();
		col_indices_.clear();

		row_indices_.reserve(rows_ + 1);
		col_indices_.reserve(pattern.nnz());

		std::size_t index = 0;
		for (auto& row : pattern.rows())
		{
			assert(std::is_sorted(row.begin(), row.end()));

			col_indices_.insert(col_indices_.end(), row.begin(), row.end());
			row_indices_.push_back(index);

			index += row.size();
		}

		col_indices_.shrink_to_fit();
		row_indices_.push_back(index);
		values_.resize(col_indices_.size());

		assert(row_indices_.size() == rows_ + 1);

#ifndef NDEBUG
		//		checkStructure();
#endif
	}

	void assign_pattern(const Csr_pattern<Symmetry, Index>& pattern)
	{
		Base::operator=(pattern);
		values_.resize(pattern.nnz());
	}

	Index rows() const
	{
		return rows_;
	}

	Index cols() const
	{
		return cols_;
	}

	Index nnz() const
	{
		return values_.size();
	}

	const Value* values() const
	{
		return values_.data();
	}

	const Index* row_indices() const
	{
		return row_indices_.data();
	}

	const Index* col_indices() const
	{
		return col_indices_.data();
	}

	std::size_t memory_size() const
	{
		return values_.capacity() * sizeof(Value) + Base::memory_size();
	}

private:
	using Base::col_indices_;
	using Base::cols_;
	using Base::row_indices_;
	using Base::rows_;

	Vector_x<Value> values_;
};

//////////////////////////////////////////////////////////////////////
//> Type traits

template<typename Value_, class Symmetry_, typename Index_>
struct Traits<Csr_matrix<Value_, Symmetry_, Index_>>
{
	using Value = Value_;
	using Layout = No_layout;
};
} // namespace esl
