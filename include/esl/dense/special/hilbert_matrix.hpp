#pragma once
#include <esl/dense/function.hpp>
#include <esl/dense/tags.hpp>

#include <cstddef>

namespace esl
{
// Returns the Hilbert matrix
template<typename T, class Layout = Col_major>
constexpr auto hilbert_matrix(const std::size_t size)
{
	return make_matrix<Layout>(size, size, [](auto row, auto col) { return static_cast<T>(T{1} / (1 + row + col)); });
}

// Returns the Hilbert matrix
template<typename T, std::size_t size, class Layout = Col_major>
constexpr auto hilbert_matrix()
{
	return make_matrix<size, size, Layout>([](auto row, auto col) { return static_cast<T>(T{1} / (1 + row + col)); });
}
} // namespace esl
