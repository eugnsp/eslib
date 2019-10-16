#pragma once
#include <esf/types.hpp>

#include <cmath>

namespace esf
{
namespace internal
{
template<class Point1, class Point2>
double distance(const Point1& point1, const Point2& point2, Point1_tag, Point1_tag)
{
	return std::abs(point1.x() - point2.x());
}

template<class Point1, class Point2>
double distance(const Point1& point1, const Point2& point2, Point2_tag, Point2_tag)
{
	return std::hypot(point1.x() - point2.x(), point1.y() - point2.y());
}
} // namespace internal

template<class Geometry1, class Geometry2>
double distance(const Geometry1& geometry1, const Geometry2& geometry2)
{
	return internal::distance(geometry1, geometry2, typename Geometry1::Geometry_tag{},
		typename Geometry2::Geometry_tag{});
}
} // namespace esf
