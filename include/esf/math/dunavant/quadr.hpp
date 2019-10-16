#pragma once
#include <esf/math/dunavant/data.hpp>
#include <esf/types.hpp>

#include <esu/array.hpp>

#include <cassert>
#include <tuple>

namespace esf
{
namespace internal
{
// Dunavant quadrature integration
template<Local_index order, class Group_index_sequence>
class Dunavant_quadr;

template<Local_index order, Local_index... group_indices>
class Dunavant_quadr<order, Local_index_sequence<group_indices...>>
{
private:
	static constexpr auto point_groups = Dunavant_data<order>::groups;
	static constexpr auto group_sizes =
		esu::make_array(std::get<group_indices>(point_groups).size()...);

public:
	static constexpr Local_index dim = 2;

	// The total number of quadrature points
	static constexpr Local_index size = esu::array_sum<Local_index>(group_sizes, 0);

public:
	// Computes a weighted sum over quadrature points,
	// sum_i weight(i) * fn(i) for i = 0, ... size() - 1
	template<class Fn>
	static constexpr auto sum(Fn fn)
	{
		return (... + std::get<group_indices>(point_groups).sum(fn, start_index<group_indices>()));
	}

protected:
	// Returns an array of all quadrature points
	static constexpr auto points()
	{
		return esu::array_cat(std::get<group_indices>(point_groups).points()...);
	}

private:
	// Returns the index of the first point in the group with the given index
	template<Local_index group_index>
	static constexpr Local_index start_index()
	{
		return esu::array_sum_n<group_index, Local_index>(group_sizes, 0);
	}
};

template<Local_index order>
using Dunavant_quadr_t = Dunavant_quadr<order,
	Make_local_index_sequence<std::tuple_size_v<decltype(Dunavant_data<order>::groups)>>>;
} // namespace internal

// Quadrature integration
template<Local_index order>
class Quadr<order, 2> : public internal::Dunavant_quadr_t<order>
{
private:
	using Base = internal::Dunavant_quadr_t<order>;

public:
	static constexpr Local_index dim = 2;

public:
	// Returns a quadrature point with the given index in the reference triangle
	static constexpr esl::Vector_2d point(Local_index point)
	{
		constexpr auto points = Base::points();
		assert(point < points.size());
		return points[point];
	}

	// 	// TODO
	// Returns the coordinates of a point with the given index in the given triangle
	// 	template<class Derived, typename Index>
	// 	static geom::Point/*math_la::Vector_2d*/ point(std::size_t point,
	// 		const geom::Triangle<Derived, Index>& triangle)
	// 	{
	// 		const auto& bary_coords = points[point].coords;
	// 		return
	// 			(1 - bary_coords[0] - bary_coords[1]) * triangle.vertex(0) +
	// 			bary_coords[0] * triangle.vertex(1) +
	// 			bary_coords[1] * triangle.vertex(2);
	// 	}
};
} // namespace esf
