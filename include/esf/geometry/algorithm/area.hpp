#pragma once
#include <esf/types.hpp>

#include <esu/algorithm.hpp>
#include <esu/numeric.hpp>

namespace esf
{
namespace internal
{
template<class Triangle>
double area(const Triangle& triangle, Triangle_tag)
{
	auto v = triangle.vertex_circ();
	auto&& a = v->vertex();
	auto&& b = (++v)->vertex();
	auto&& c = (++v)->vertex();

	return .5 * esu::det(a.x(), a.y(), 1., b.x(), b.y(), 1., c.x(), c.y(), 1.);
}

template<class Rect>
double area(const Rect& rect, Rect_tag)
{
	return rect.width() * rect.height();
}

template<class Polygon>
double area(const Polygon& poly, Polygon_tag)
{
	double area = 0;
	const auto add_to_area = [&area](auto& v1, auto& v2) { area += v1 ^ v2; }; // TODO : cross

	const auto first = poly.vertex_circ();
	const auto last = esu::for_each_pair_n(first, poly.n_vertices() - 1, add_to_area);
	add_to_area(*last, *first);

	return area / 2;
}
} // namespace internal

template<class Geometry>
double area(const Geometry& geometry)
{
	return internal::area(geometry, typename Geometry::Geometry_tag{});
}
} // namespace esf
