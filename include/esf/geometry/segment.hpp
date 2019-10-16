#pragma once
#include <cassert>
#include <esf/geometry/point2.hpp>
#include <esf/types.hpp>

namespace fe
{
class Segment
{
public:
	using Geometry_tag = Segment2_tag;

public:
	Segment() = default;

	Segment(const Point2& p1, const Point2& p2) : points_{p1, p2}
	{
		// TODO
		assert(p1 != p2);
	}

	template<Index index>
	const Point2& vertex() const
	{
		static_assert(index < 2, "Index out of bounds");
		return points_[index];
	}

	const Point2& vertex(Index index) const
	{
		assert(index < 2);
		return points_[index];
	}

private:
	Point2 points_[2];
};
} // namespace fe
