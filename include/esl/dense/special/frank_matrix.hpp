#pragma once
#include <esl/dense/function.hpp>
#include <esl/dense/tags.hpp>

#include <algorithm>
#include <cstddef>

namespace esl
{
// Returns the Frank matrix
template<typename T, class Layout = Col_major>
constexpr auto frank_matrix(const std::size_t size)
{
	return make_matrix<Layout>(size, size, [](auto row, auto col) { return static_cast<T>(1 + std::min(row, col)); });
}

// Returns the Frank matrix
template<typename T, std::size_t size, class Layout = Col_major>
constexpr auto frank_matrix()
{
	return make_matrix<size, size, Layout>([](auto row, auto col) { return static_cast<T>(1 + std::min(row, col)); });
}
} // namespace esl
