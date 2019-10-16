#pragma once
#include <esl/dense/shape_base.hpp>
#include <esl/dense/type_traits.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace esl::internal
{
template<std::size_t ct_rows, std::size_t ct_cols, class Layout>
class Shape : public Shape_base<ct_rows, ct_cols>
{
private:
	using Base = Shape_base<ct_rows, ct_cols>;

public:
	//////////////////////////////////////////////////////////////////////
	//* Extents */

	using Base::cols;
	using Base::rows;

	constexpr std::size_t l_dim() const
	{
		if constexpr (is_col_major<Layout>)
			return rows();
		else if constexpr (is_row_major<Layout>)
			return cols();
	}

protected:
	using Base::Base;

	constexpr std::size_t linear_index(std::size_t row, std::size_t col) const
	{
		assert(row < rows());
		assert(col < cols());

		if constexpr (is_col_major<Layout>)
			return row + col * l_dim();
		else if constexpr (is_row_major<Layout>)
			return col + row * l_dim();
	}
};
} // namespace esl::internal
