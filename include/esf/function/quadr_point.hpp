#pragma once
#include <esf/geometry.hpp>
#include <esf/index.hpp>
#include <esf/tags.hpp>

#include <type_traits>

namespace esf
{
// Returns the coordinates of the quadrature point in the reference triangle
template<class Quadr>
Point2 point(Quadr_point_index<Quadr> i)
{
	return Quadr::point(i.index);
}

// Returns the coordinates of the quadrature point in the triangle
template<class Quadr,
		 class Triangle,
		 typename = std::enable_if_t<std::is_same_v<typename Triangle::Geometry_tag, Triangle_tag>>>
Point2 point(Quadr_point_index<Quadr> i, const Triangle& triangle)
{
	auto v = triangle.vertex_circ();
	auto&& a = v->vertex();
	auto&& b = (++v)->vertex();
	auto&& c = (++v)->vertex();

	const auto pt = Quadr::point(i.index);
	return (1 - pt[0] - pt[1]) * a + pt[0] * b + pt[1] * c;
}
} // namespace esf
