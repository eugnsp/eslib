#pragma once
//#include "util/Exception.h"
//#include <es/la/sparse/sparse_matrix.hpp>
#include <esl/dense/tags.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <esu/error.hpp>
#include <esu/function.hpp>
#include <esu/iterator.hpp>
#include <esu/type_traits.hpp>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace esl
{

template<typename T>
class Coord_sparsity_pattern
{
public:
	bool is_empty() const
	{
		return row_indices_.empty();
	}

	T size() const
	{
		return static_cast<T>(row_indices_.size());
	}

	void insert(T row, T col)
	{
		row_indices_.push_back(row);
		col_indices_.push_back(col);
	}

	void reserve(T size)
	{
		row_indices_.reserve(size);
		col_indices_.reserve(size);
	}

	void remove_duplicates()
	{
		std::sort(begin(), end());
		const auto new_length = std::unique(begin(), end()) - begin();
		row_indices_.resize(new_length);
		col_indices_.resize(new_length);
	}

	auto begin()
	{
		return esu::Zip_iterator(row_indices_.begin(), col_indices_.begin());
	}

	auto end()
	{
		return esu::Zip_iterator(row_indices_.end(), col_indices_.end());
	}

private:
	std::vector<T> row_indices_, col_indices_;
};

template<typename T>
class Csr_sparsity_pattern
{
public:
	bool is_empty() const
	{
		return row_indices_.empty();
	}

	T size() const
	{
		return static_cast<T>(row_indices_.size());
	}

	void insert(T row, T col)
	{
		row_indices_.push_back(row);
		col_indices_.push_back(col);
	}

	void reserve(T size)
	{
		row_indices_.reserve(size);
		col_indices_.reserve(size);
	}

private:
	std::vector<T> row_indices_, col_indices_;
};

template<class Symmetry_tag>
class Sparsity_pattern
{
private:
	using Row = std::vector<std::size_t>;

public:
	explicit Sparsity_pattern(std::size_t n_rows)
	{
		rows_.resize(n_rows);
	}

	std::size_t nnz() const
	{
		std::size_t nnz = 0;
		for (auto& row : rows_)
			nnz += row.size();
		return nnz;
	}

	std::size_t n_rows() const
	{
		return rows_.size();
	}

	auto& rows()
	{
		return rows_;
	}

	const auto& rows() const
	{
		return rows_;
	}

	Row& row(std::size_t index)
	{
		return rows_[index];
	}

	const Row& row(std::size_t index) const
	{
		return rows_[index];
	}

	void insert(std::size_t row, std::size_t col)
	{
		debug_check_index(row, col);

		auto& r = rows_[row];
		r.insert(std::lower_bound(r.begin(), r.end(), col), col);
	}

	template<class Random_access_it>
	void insert(std::size_t row, Random_access_it cols_first, Random_access_it cols_last)
	{
		debug_check_indices(row, cols_first, cols_last);

		if (cols_first == cols_last)
			return;

		Row& r = rows_[row];
		if (r.empty())
			r.insert(r.end(), cols_first, cols_last);
		else
		{
			const auto lower_pos = std::lower_bound(r.begin(), r.end(), *cols_first);
			auto lower = lower_pos - r.begin();
			const auto upper = std::upper_bound(lower_pos, r.end(), *std::prev(cols_last)) - r.begin();

			const auto middle = r.size();
			while (cols_first != cols_last)
			{
				const auto pos = std::equal_range(r.begin() + lower, r.begin() + upper, *cols_first);
				lower = pos.second - r.begin();
				if (pos.first == pos.second)
					r.push_back(*cols_first);

				++cols_first;
			}

			// TODO : Optimize for no merge?
			std::inplace_merge(r.begin(), r.begin() + middle, r.end());
		}
	}

	esu::Error check() const;

	bool is_each_row_not_empty() const
	{
		return std::all_of(rows_.begin(), rows_.end(), [](auto& row) { return !row.empty(); });
	}

	// TODO
	bool is_symmetric() const
	{
		for (std::size_t row = 0; row < n_rows(); ++row)
			for (auto col : rows_[row])
			{
				assert(col < n_rows());
				auto& row_tr = rows_[col];

				if (std::find(row_tr.begin(), row_tr.end(), row) == row_tr.end())
					return false;
			}

		return true;
	}

private:
	static void debug_check_index([[maybe_unused]] std::size_t row, [[maybe_unused]] std::size_t col)
	{
		if constexpr (std::is_same_v<Symmetry_tag, Symmetric_upper>)
			assert(row <= col);
	}

	template<class Forward_it>
	static void debug_check_indices(std::size_t row, Forward_it cols_first, Forward_it cols_last)
	{
		assert(std::is_sorted(cols_first, cols_last));
		assert(std::adjacent_find(cols_first, cols_last) == cols_last);

		std::for_each(cols_first, cols_last, [row](std::size_t col) { debug_check_index(row, col); });
	}

private:
	std::vector<Row> rows_;
};

//////////////////////////////////////////////////////////////////////////

template<class Symmetry_tag>
esu::Error Sparsity_pattern<Symmetry_tag>::check() const
{
	esu::Error err;

	unsigned int n_errors = 0;
	for (std::size_t i = 0; i < rows_.size(); ++i)
	{
		auto& row = rows_[i];
		if (!std::is_sorted(row.begin(), row.end()))
		{
			++n_errors;
			err.append("\n* Error: not sorted row.\n", "The row #", i, " is not sorted.\n",
				"The indices are: ", esu::join_as_string(row.begin(), row.end()));
		}

		auto pos = std::adjacent_find(row.begin(), row.end());
		if (pos != row.end())
		{
			++n_errors;
			err.append("\n* Error: same indices in the row.\n", "The row #", i, " contains two or more same indices: ");

			for (;;)
			{
				err.append(*pos++);
				pos = std::adjacent_find(pos, row.end());
				if (pos != row.end())
					err.append(", ");
				else
					break;
			}

			err.append("\nThe indices are: ", esu::join_as_string(row.begin(), row.end()));
		}

		if (n_errors >= 10)
		{
			err.append("\nToo many errors.");
			break;
		}
	}

	return err;
}
} // namespace esl
