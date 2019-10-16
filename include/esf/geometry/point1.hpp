#pragma once
#include <esf/geometry/algorithm/equals.hpp>
#include <esf/geometry/compare.hpp>
#include <esf/types.hpp>

#include <esl/dense.hpp>
#include <esu/numeric.hpp>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <iosfwd>

namespace esf
{
class Point1
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

	constexpr double x() const
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

std::ostream& operator<<(std::ostream& out, const Point1& p);
} // namespace esf
