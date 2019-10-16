#include <esf/geometry/geometry.hpp>
#include <esf/geometry/point2.hpp>

#include <esl/dense.hpp>

//#include <esu/numeric.hpp>
#include <esf/geometry/algorithm.hpp>
//#include "math/LA/VectorFunction.h"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace esf
{
/** Checks whether four points form a rectangle.
 *  Precondition: points should be ordered anti-clockwise. */
// bool isAlignedRect(const Point2& p1, const Point2& p2, const Point2& p3, const Point2& p4)
// {
// 	return ((is_phys_equal(p1.y(), p2.y()) && is_equal(p3.y(), p4.y()) &&
// 		is_phys_equal(p1.x(), p4.x()) && is_equal(p2.x(), p3.x())) ||
// 		(is_phys_equal(p1.x(), p2.x()) && is_equal(p3.x(), p4.x()) &&
// 			is_phys_equal(p1.y(), p4.y()) && is_equal(p2.y(), p3.y())));
// }

// Checks if the point lies on the line segment [a, b]
bool is_point_on_line_segment(const Point2& point, const Point2& a, const Point2& b)
{
	if (a == point || b == point)
		return true;

	const auto pa = point - a;
	const auto ba = b - a;
	const double d = dot(ba, ba);

	const double cross_ba_pa = cross(ba, pa);
	if (std::abs(cross_ba_pa) > delta * std::sqrt(d))
		return false;

	const double dot_d_pa = dot(ba, pa);
	if (dot_d_pa < 0 || dot_d_pa > d)
		return false;

	return true;
}

double GetDistanceToLineSegment(const Point2& point, const Point2& a, const Point2& b)
{
	if (a == point || b == point)
		return 0;

	if (a == b)
		return distance(point, a);

	// const auto pa = point - a;
	// const auto d = b - a;
	// const double dd = norm2Squared(d);

	const double dx = b.x() - a.x();
	const double dy = b.y() - a.y();
	const double dd = dx * dx + dy * dy;

	const double dot = dx * (point.x() - a.x()) + dy * (point.y() - a.y());
	const double t = dot / dd;

	if (t <= 0)
		return distance(point, a);

	if (t >= 1)
		return distance(point, b);

	return distance(point, Point2{a.x() + t * dx, a.y() + t * dy});
}

// Computes which side of the line (a -> b) the point is located at
Side which_side(const Point2& point, const Point2& a, const Point2& b)
{
	if (point == a || point == b)
		return Side::ON_THE_LINE;

	// TODO : evaluate d
	const auto ba = b - a;
	const double cross_ba_pa = cross(ba, point - a);

	if (std::abs(cross_ba_pa) < delta * dot(ba, ba))
		return Side::ON_THE_LINE;

	return cross_ba_pa > 0 ? Side::ON_THE_LEFT : Side::ON_THE_RIGHT;
}

// Computes the intersection point between the line segment [x1, x2] and
// the line passing through points (y1) and (y2)
Point2 GetSegmentAndLineIntersectionPoint2(
	const Point2& x1, const Point2& x2, const Point2& y1, const Point2& y2)
{
	const double dXx = x2.x() - x1.x();
	const double dXy = x2.y() - x1.y();

	const double dYx = y2.x() - y1.x();
	const double dYy = y2.y() - y1.y();

	const double dXYx = y1.x() - x1.x();
	const double dXYy = y1.y() - x1.y();

	const double crossXY = dXx * dYy - dXy * dYx;

	if (std::abs(crossXY) <
		delta * std::sqrt(std::max(dXx * dXx + dXy * dXy, dYx * dYx + dYy * dYy)))
	{
		assert(std::abs(dXYx * dXy - dXYy * dXx) > .00001);

		return x2; // The intersection is the last point (by agreement)
	}
	else
	{
		const double cross = dXYx * dYy - dXYy * dYx;
		const double t = cross / crossXY;

		assert(t >= -.000001 || t <= 1.000001);

		return {x1.x() + t * dXx, x1.y() + t * dXy};
	}
}

// Clips simple by the line passing through points (y1) and (y2)
// void ClipSimpleByLine(SimplePolygon& out, const SimplePolygon& in, const Point2& y1, const Point2&
// y2)
//{
//	assert(!in.IsEmpty());
//
//	out.Clear();
//
//	const unsigned int sizeOfIn = in.GetNumberOfVertices();
//	const Point2* a = &in.GetVertex(sizeOfIn - 1);
//
//	for (unsigned int i = 0; i < sizeOfIn; ++i)
//	{
//		const Point2* b = &in.GetVertex(i);
//
//		switch (GetWhichSide(*b, y1, y2))
//		{
//			case SIDE::ON_THE_LEFT:
//				if (GetWhichSide(*a, y1, y2) == SIDE::ON_THE_RIGHT)
//					out.AddVertex(GetSegmentAndLineIntersectionPoint2(*a, *b, y1, y2));
//				// no break!
//
//			case SIDE::ON_THE_LINE:
//				out.AddVertex(*b);
//				break;
//
//			case SIDE::ON_THE_RIGHT:
//				if (GetWhichSide(*a, y1, y2) == SIDE::ON_THE_LEFT)
//					out.AddVertex(GetSegmentAndLineIntersectionPoint2(*a, *b, y1, y2));
//				break;
//		}
//
//		a = b;
//	}
//}

double circumradius(const Point2& , const Point2& , const Point2& )
{
	throw 0;
	// return distance(a, b) * distance(b, c) * distance(c, a) / (4 * area(a, b, c));
}
} // namespace esf
