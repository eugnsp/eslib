#pragma once
#include <esf/geometry/compare.hpp>
#include <esf/geometry/function/equals.hpp>
#include <esf/geometry/point1.hpp>
#include <esf/tags.hpp>

#include <esl/dense.hpp>
#include <esu/numeric.hpp>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <iosfwd>
#include <string>

namespace esf
{
class Point2 final : public esl::Vector_2d
{
public:
	using Geometry_tag = Point2_tag;

private:
	using Base = esl::Vector_2d;

public:
	using Base::Base;

	template<typename X, typename Y>
	Point2(X x, Y y)
		: Base{static_cast<double>(x), static_cast<double>(y)}
	{}

	Point2(Point1 x, Point1 y)
		: Base{x.x(), y.x()}
	{}

	double& x()
	{
		return (*this)[0];
	}

	double& y()
	{
		return (*this)[1];
	}

	const double& x() const
	{
		return (*this)[0];
	}

	const double& y() const
	{
		return (*this)[1];
	}

	std::string to_string() const;
};

// inline Point operator*(Point p, double scalar)
// {
// 	p *= scalar;
// 	return p;
// }
//
// inline Point operator*(double scalar, const Point& p)
// {
// 	return p * scalar;
// }
//
// inline Point operator/(Point p, double scalar)
// {
// 	p /= scalar;
// 	return p;
// }

// Lexicographical ordering of two points
inline bool operator<(const Point2& lhs, const Point2& rhs)
{
	if (is_geom_less(lhs.x(), rhs.x()))
		return true;

	if (is_geom_greater(lhs.x(), rhs.x()))
		return false;

	if (is_geom_less(lhs.y(), rhs.y()))
		return true;

	return false;
}

// inline bool isPointInRefTriangle(const Point& p)
// {
// 	return p.x() > -PHYS_MIN_POINT_DISTANCE
// 		&& p.y() > -PHYS_MIN_POINT_DISTANCE
// 		&& p.x() + p.y() < 1 + PHYS_MIN_POINT_DISTANCE;
// }

inline bool operator==(const esf::Point2& a, const esf::Point2& b)
{
	return equals(a, b);
}

/** Returns the dot product of two points @f$ a_x b_x + a_y b_y @f$. */
inline double operator*(const esf::Point2& a, const esf::Point2& b)
{
	return a.x() * b.x() + a.y() * b.y();
}

// Returns the cross product of two points, ax * by - ay * bx
inline double operator^(const esf::Point2& a, const esf::Point2& b)
{
	return esu::det(a.x(), a.y(), b.x(), b.y());
}

std::ostream& operator<<(std::ostream& out, const esf::Point2& p);
} // namespace esf
