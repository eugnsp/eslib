#pragma once
#include "sparsity_pattern.hpp"
#include <esl/sparse/forward.hpp>

//#include <esl/base/extended.hpp>
#include <esl/dense/matrix.hpp>
#include <esl/dense/tags.hpp>

#include <algorithm>
#include <type_traits>
#include <vector>

namespace esl
{
template<class Symmetry_tag_, typename Index_>
class Csr_pattern /*: public internal::Extended<Sparse_matrix<T_Value>, 0, 0>*/
{
public:
	using Symmetry_tag = Symmetry_tag_;
	using Index = Index_;

public:
	// Csr_pattern() = default;

	// Csr_pattern(Index rows, Index cols) : rows_(rows), cols_(cols)
	// {}

	// void resize(Index rows, Index cols)
	// {
	// 	rows_ = rows;
	// 	cols_ = cols;
	// }

	// 	void set_sparsity_pattern(const Sparsity_pattern<Symmetry_tag>& pattern)
	// 	{
	// 		assert(pattern.n_rows() == rows_);

	// 		row_indices_.clear();
	// 		col_indices_.clear();

	// 		row_indices_.reserve(rows_ + 1);
	// 		col_indices_.reserve(pattern.nnz());

	// 		std::size_t index = 0;
	// 		for (auto& row : pattern.rows())
	// 		{
	// 			assert(std::is_sorted(row.begin(), row.end()));

	// 			col_indices_.insert(col_indices_.end(), row.begin(), row.end());
	// 			row_indices_.push_back(index);

	// 			index += row.size();
	// 		}

	// 		col_indices_.shrink_to_fit();
	// 		row_indices_.push_back(index);
	// 		values_.resize(col_indices_.size());

	// 		assert(row_indices_.size() == rows_ + 1);

	// #ifndef NDEBUG
	// 		//		checkStructure();
	// #endif
	// 	}

	// template<typename Value2>
	// void set_sparsity_pattern(const Csr_matrix<Value2, Symmetry_tag, Index>& matrix)
	// {
	// 	rows_ = matrix.rows();
	// 	cols_ = matrix.cols();

	// 	row_indices_.assign(matrix.row_indices(), matrix.row_indices() + rows_ + 1);
	// 	col_indices_.assign(matrix.col_indices(), matrix.col_indices() + matrix.nnz());
	// 	values_.resize(matrix.nnz());
	// }

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
		return col_indices_.size();
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
		return (row_indices_.capacity() + col_indices_.capacity()) * sizeof(Index);
	}

protected:
	std::vector<Index> row_indices_;
	std::vector<Index> col_indices_;

	Index rows_ = 0;
	Index cols_ = 0;
};
} // namespace esl
