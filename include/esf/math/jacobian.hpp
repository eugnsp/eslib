#pragma once
#include <esf/types/tags.hpp>

#include <esl/dense.hpp>

namespace esf
{
namespace internal
{
// Returns the Jacobian of the transformation
// from a given segment to the reference one
template<typename Segment>
double jacobian(const Segment& segment, Segment1_tag)
{
	return segment.vertex(1).x() - segment.vertex(0).x();
}

template<typename Segment>
double inv_jacobian(const Segment& segment, Segment1_tag tag)
{
	return 1 / jacobian(segment, tag);
}

// Returns the Jacobian of the transformation
// from a given triangle to the reference one
template<typename Triangle>
esl::Matrix_2d jacobian(const Triangle& triangle, Triangle_tag)
{
	auto v = triangle.vertex_circ();
	auto&& a = v->vertex();
	auto&& b = (++v)->vertex();
	auto&& c = (++v)->vertex();

	esl::Matrix_2d j;

	j.col_view<0>() = b - a;
	j.col_view<1>() = c - a;
	return j;
}

// Returns the inverted Jacobian of the transformation
// from a given triangle to the reference one
template<typename Triangle>
esl::Matrix_2d inv_jacobian(const Triangle& triangle, Triangle_tag tag)
{
	auto j = jacobian(triangle, tag);
	invert(j);
	return j;
}

// Returns the inverted transposed Jacobian of the transformation
// from a given triangle to the reference one
template<typename Triangle>
esl::Matrix_2d inv_transp_jacobian(const Triangle& triangle, Triangle_tag)
{
	auto j = jacobian(triangle);
	invert_transpose(j);
	return j;
}
} // namespace internal

template<class Geometry>
auto jacobian(const Geometry& geometry)
{
	return internal::jacobian(geometry, typename Geometry::Geometry_tag{});
}

template<class Geometry>
auto inv_jacobian(const Geometry& geometry)
{
	return internal::inv_jacobian(geometry, typename Geometry::Geometry_tag{});
}

template<class Geometry>
auto inv_transp_jacobian(const Geometry& geometry)
{
	return internal::inv_transp_jacobian(geometry, typename Geometry::Geometry_tag{});
}
} // namespace esf
