#pragma once
#include <esf/geometry/point1.hpp>
#include <esf/index.hpp>
#include <esf/quadr/gauss/data.hpp>

#include <cassert>
#include <cstddef>
#include <utility>

namespace esf
{
template<std::size_t order>
class Quadr<order, Dim1>
{
private:
	static constexpr auto points = internal::Gauss_data<order>::points;

public:
	using Space_dim = Dim1;

	// The total number of quadrature points
	static constexpr std::size_t size = points.size();

public:
	// Returns the quadrature point with the given index
	// on the reference interval [0, 1]
	static constexpr Point1 point(std::size_t point)
	{
		assert(point < size);

		// Map the interval [-1, 1], which is used in data, to [0, 1]
		return Point1{(1 + points[point].x) / 2};
	}

	// Computes a weighted sum over quadrature points,
	// sum_i weight(i) * fn(i) for i = 0, ... size() - 1
	template<class Fn>
	static constexpr auto sum(Fn fn)
	{
		return sum_impl(fn, std::make_index_sequence<size>{});
	}

private:
	template<class Fn, std::size_t... indices>
	static constexpr auto sum_impl(Fn fn, std::index_sequence<indices...>)
	{
		return (... + (points[indices].weight * fn(indices)));
	}
};
} // namespace esf
