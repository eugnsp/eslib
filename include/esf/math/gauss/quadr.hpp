#pragma once
#include <esf/geometry/point1.hpp>
#include <esf/math/gauss/data.hpp>
#include <esf/types.hpp>

#include <cassert>

namespace esf
{
template<Local_index order>
class Quadr<order, 1>
{
private:
	static constexpr auto points = internal::Gauss_data<order>::points;

public:
	static constexpr Local_index dim = 1;

	// The total number of quadrature points
	static constexpr Local_index size = points.size();

public:
	// Returns the quadrature point with the given index
	// on the reference interval [0, 1]
	static constexpr esf::Point1 point(Local_index point)
	{
		assert(point < size);

		// Map the interval [-1, 1], which is used in data, to [0, 1]
		return esf::Point1{(1 + points[point].x) / 2};
	}

	// Computes a weighted sum over quadrature points,
	// sum_i weight(i) * fn(i) for i = 0, ... size() - 1
	template<class Fn>
	static constexpr auto sum(Fn fn)
	{
		return sum_impl(fn, Make_local_index_sequence<size>{});
	}

private:
	template<class Fn, Local_index... indices>
	static constexpr auto sum_impl(Fn fn, Local_index_sequence<indices...>)
	{
		return (... + (points[indices].weight * fn(indices)));
	}
};
} // namespace esf
