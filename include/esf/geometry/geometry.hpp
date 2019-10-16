#pragma once
#include <esf/geometry/algorithm/distance.hpp>
#include <esf/geometry/point2.hpp>

#include <esu/numeric.hpp>
//#include "math/LA/VectorFunction.h"

#include <algorithm>
#include <cmath>

namespace esf
{
// Basic geometric functions

inline double distance_squared(const Point2& a, const Point2& b)
{
	const auto dx = a.x() - b.x();
	const auto dy = a.y() - b.y();
	return dx * dx + dy * dy;
}

/** Checks whether four points form a rectangle.
 *  Precondition: points should be order anti-clockwise. */
bool isAlignedRect(const Point2&, const Point2&, const Point2&, const Point2&);

// Checks if the point lies on the line segment (a, b)
bool is_point_on_line_segment(const Point2&, const Point2& a, const Point2& b);

double GetDistanceToLineSegment(const Point2& point, const Point2& a, const Point2& b);

enum class Side
{
	ON_THE_LINE,
	ON_THE_LEFT,
	ON_THE_RIGHT
};

// Computes which side of the directed line (a -> b) the point is located at
Side which_side(const esf::Point2&, const esf::Point2& a, const esf::Point2& b);

// Computes the intersection point between the line segment [x1, x2] and
// the line passing through points (y1) and (y2)
esf::Point2 GetSegmentAndLineIntersectionPoint(const esf::Point2& _x1, const esf::Point2& _x2,
	const esf::Point2& _y1, const esf::Point2& _y2);

// Clips simple by the line passing through points (y1) and (y2)
// void ClipSimpleByLine(SimplePolygon& _out, const SimplePolygon& _in, const Point2& _y1, const
// Point2& _y2);

// Functions related to triangles

// Point circumcenter(const Point2&, const Point2&, const Point2&);
// double circumradius(const Point2&, const Point2&, const Point2&);

// 	inline double signed_area(const Point2& a, const Point2& b, const Point2& c)
// 	{
// 		return math::det(a.x(), a.y(), 1., b.x(), b.y(), 1., c.x(), c.y(), 1.) / 2;
// 	}
//
// 	inline double area(const Point2& a, const Point2& b, const Point2& c)
// 	{
// 		return std::abs(signed_area(a, b, c));
// 	}

// Diameter of a triangle is the maximum distance between any two vertices
inline double diameter(const Point2& a, const Point2& b, const Point2& c)
{
	return std::max({distance(b, c), distance(c, a), distance(a, b)});
}
} // namespace esf
