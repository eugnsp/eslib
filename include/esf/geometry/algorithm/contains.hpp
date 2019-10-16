#pragma once
#include <esf/geometry/compare.hpp>
#include <esf/geometry/geometry.hpp>
#include <esf/geometry/point2.hpp>
#include <esf/types.hpp>

#include <esu/algorithm.hpp>

#include <type_traits>

namespace esf
{
// TODO : remove
bool is_point_on_line_segment(const Point2&, const Point2& a, const Point2& b);

namespace internal
{
// TODO : optimize
template<class Triangle, class Point>
bool contains(const Triangle& triangle, const Point& point, Triangle_tag, Point2_tag)
{
	auto v = triangle.vertex_circ();
	auto&& a = v->vertex();
	auto&& b = (++v)->vertex();
	auto&& c = (++v)->vertex();

	return which_side(point, a, b) != Side::ON_THE_RIGHT &&
		   which_side(point, b, c) != Side::ON_THE_RIGHT &&
		   which_side(point, c, a) != Side::ON_THE_RIGHT;
}

template<class Rect, class Point>
bool contains(const Rect& rect, const Point& point, Rect_tag, Point2_tag)
{
	return is_geom_greater_equal(point.x(), rect.left()) &&
		   is_geom_greater_equal(point.y(), rect.bottom()) &&
		   is_geom_less_equal(point.x(), rect.right()) && is_geom_less_equal(point.y(), rect.top());
}

template<class Linestring, class Point>
bool contains(const Linestring& linestring, const Point& point, Linestring_tag, Point2_tag)
{
	return esu::any_of_pairs(linestring.begin_vertex(), linestring.end_vertex(),
		[&point](auto& v1, auto& v2) { return is_point_on_line_segment(point, v1, v2); });
}

template<class Polygon, class Point>
bool contains(const Polygon& poly, const Point& point, Polygon_tag, Point2_tag)
{
	bool inside = false;

	const auto x = point.x();
	const auto y = point.y();

	const auto n = poly.n_vertices();
	for (Local_index i = 0, j = n - 1; i < n; j = i++)
	{
		auto&& vi = poly.vertex(i);
		auto&& vj = poly.vertex(j);
		if (((vi.y() > y) != (vj.y() > y)) &&
			(x < (vj.x() - vi.x()) * (y - vi.y()) / (vj.y() - vi.y()) + vi.x()))
		{
			inside = !inside;
		}
	}
	return inside;
}
} // namespace internal

template<class Geometry1, class Geometry2>
bool contains(const Geometry1& geometry1, const Geometry2& geometry2)
{
	return internal::contains(geometry1, geometry2, typename Geometry1::Geometry_tag{},
		typename Geometry2::Geometry_tag{});
}
} // namespace esf
