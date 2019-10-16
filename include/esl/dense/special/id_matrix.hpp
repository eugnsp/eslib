#pragma once
#include <esl/dense/function.hpp>
#include <esl/dense/tags.hpp>

#include <cstddef>

namespace esl
{
// Returns the identity matrix
template<typename T, class Layout = Col_major>
constexpr auto id_matrix(const std::size_t size)
{
	return make_matrix<Layout>(size, size, [](auto row, auto col) { return static_cast<T>(row == col ? 1 : 0); });
}

// Returns the identity matrix
template<typename T, std::size_t size, class Layout = Col_major>
constexpr auto id_matrix()
{
	return make_matrix<size, size, Layout>([](auto row, auto col) { return static_cast<T>(row == col ? 1 : 0); });
}
} // namespace esl
