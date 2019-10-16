#pragma once
#include <esf/geometry/rect.hpp>
#include <esf/types.hpp>

#include <esu/algorithm.hpp>
#include <esu/numeric.hpp>

#include <algorithm>

namespace esf
{
namespace internal
{
template<class Triangle>
Rect bounding_rect(const Triangle& triangle, Triangle_tag)
{
	auto v = triangle.vertex_circ();

	auto&& a = v->vertex();
	auto&& b = (++v)->vertex();
	auto&& c = (++v)->vertex();

	const auto x = std::minmax({a.x(), b.x(), c.x()});
	const auto y = std::minmax({a.y(), b.y(), c.y()});

	return {{x.first, y.first}, {x.second, y.second}};
}
} // namespace internal

template<class Geometry>
Rect bounding_rect(const Geometry& geometry)
{
	return internal::bounding_rect(geometry, typename Geometry::Geometry_tag{});
}
} // namespace esf
