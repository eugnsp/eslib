#pragma once
#include <esf/geometry/function/distance.hpp>
#include <esf/tags.hpp>

#include <algorithm>
#include <cmath>

namespace esf
{
namespace internal
{
template<class Triangle>
double diam(const Triangle& triangle, Triangle_tag)
{
	auto v = triangle.vertex_circ();
	auto&& a = v->vertex();
	auto&& b = (++v)->vertex();
	auto&& c = (++v)->vertex();

	return std::max({distance(a, b), distance(b, c), distance(c, a)});
}

template<class Rect>
double diam(const Rect& rect, Rect_tag)
{
	return std::hypot(rect.width(), rect.height());
}
} // namespace internal

template<class Geometry>
double diam(const Geometry& geometry)
{
	return internal::diam(geometry, typename Geometry::Geometry_tag{});
}
} // namespace esf
