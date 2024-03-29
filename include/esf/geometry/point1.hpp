#pragma once
#include <esf/geometry/compare.hpp>
#include <esf/geometry/function/equals.hpp>
#include <esf/tags.hpp>

#include <esl/dense.hpp>
#include <esu/numeric.hpp>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <iosfwd>

namespace esf
{
class Point1 final
{
public:
	using Geometry_tag = Point1_tag;

public:
	Point1() = default;

	constexpr explicit Point1(double x) : x_(x)
	{}

	constexpr double& x()
	{
		return x_;
	}

	constexpr const double& x() const
	{
		return x_;
	}

private:
	double x_;
};

inline bool operator<(const Point1& lhs, const Point1& rhs)
{
	return is_geom_less(lhs.x(), rhs.x());
}

inline bool operator==(const Point1& a, const Point1& b)
{
	return equals(a, b);
}

inline bool operator!=(const Point1& a, const Point1& b)
{
	return !(a == b);
}

std::ostream& operator<<(std::ostream& out, const Point1& p);
} // namespace esf
