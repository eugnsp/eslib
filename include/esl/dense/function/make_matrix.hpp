#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>

#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace esl
{
namespace internal
{
template<std::size_t... ii, class Fn>
constexpr auto make_vector_impl(std::index_sequence<ii...>, Fn fn)
{
	using Value = std::invoke_result_t<Fn, std::size_t>;
	return Vector<Value, sizeof...(ii)>{fn(ii)...};
}

template<std::size_t rows, std::size_t cols, class Layout, std::size_t... ii, class Fn>
constexpr auto make_matrix_impl(std::index_sequence<ii...>, Fn fn)
{
	using Value = std::invoke_result_t<Fn, std::size_t, std::size_t>;
	constexpr auto stride = is_col_major<Layout> ? rows : cols;
	return Matrix<Value, rows, cols, Layout>{fn(ii % stride, ii / stride)...};
}
} // namespace internal

template<std::size_t size, class Fn>
constexpr auto make_vector(Fn fn)
{
	return internal::make_vector_impl(std::make_index_sequence<size>{}, fn);
}

template<class Fn>
constexpr auto make_vector(std::size_t size, Fn fn)
{
	using Value = std::invoke_result_t<Fn, std::size_t>;
	esl::Vector_x<Value> vec(size);

	for (std::size_t i = 0; i < vec.size(); ++i)
		vec[i] = fn(i);
	return vec;
}

template<std::size_t rows, std::size_t cols, class Layout = Col_major, class Fn>
constexpr auto make_matrix(Fn fn)
{
	return internal::make_matrix_impl<rows, cols, Layout>(std::make_index_sequence<rows * cols>{}, fn);
}

template<class Layout = Col_major, class Fn>
constexpr auto make_matrix(std::size_t rows, std::size_t cols, Fn fn)
{
	using Value = std::invoke_result_t<Fn, std::size_t, std::size_t>;
	esl::Matrix_x<Value, Layout> mat(rows, cols);

	if (is_col_major<Layout>)
		for (std::size_t col = 0; col < cols; ++col)
			for (std::size_t row = 0; row < rows; ++row)
				mat(row, col) = fn(row, col);
	else
		for (std::size_t row = 0; row < rows; ++row)
			for (std::size_t col = 0; col < cols; ++col)
				mat(row, col) = fn(row, col);
	return mat;
}
} // namespace esl
