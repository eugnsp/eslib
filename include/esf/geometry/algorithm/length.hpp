#pragma once
#include <esf/geometry/algorithm/distance.hpp>
#include <esf/types.hpp>

#include <esu/algorithm.hpp>

namespace esf
{
namespace internal
{
template<class Segment>
double length(const Segment& segment, Segment_tag)
{
	return distance(segment.vertex(0), segment.vertex(1));
}

template<class Linestring>
double length(const Linestring& linestring, Linestring_tag)
{
	double l = 0;
	esu::for_each_pair(linestring.begin_vertex(), linestring.end_vertex(),
		[&l](const auto& v1, const auto& v2) { l += distance(v1, v2); });

	return l;
}
} // namespace internal

template<class Geometry>
double length(const Geometry& geometry)
{
	return internal::length(geometry, typename Geometry::Geometry_tag{});
}
} // namespace esf
