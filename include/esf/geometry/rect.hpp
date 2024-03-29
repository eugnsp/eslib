#pragma once
#include <esf/geometry/point2.hpp>
#include <esf/tags.hpp>

#include <cassert>

namespace esf
{
// An axis aligned rectangle
class Rect
{
public:
	using Geometry_tag = Rect_tag;

public:
	Rect() = default;

	Rect(const Point2& bottom_left, const double width, const double height)
		: bottom_left_(bottom_left), width_(width), height_(height)
	{
		assert(width_ >= 0 && height_ >= 0);
	}

	Rect(const Point2& bottom_left, const Point2& top_right)
		: Rect(bottom_left, top_right.x() - bottom_left.x(), top_right.y() - bottom_left.y())
	{}

	double top() const
	{
		return bottom_left_.y() + height_;
	}

	double bottom() const
	{
		return bottom_left_.y();
	}

	double left() const
	{
		return bottom_left_.x();
	}

	double right() const
	{
		return bottom_left_.x() + width_;
	}

	double width() const
	{
		return width_;
	}

	double height() const
	{
		return height_;
	}

	Point2 bottom_left() const
	{
		return bottom_left_;
	}

	Point2 bottom_right() const
	{
		return {bottom(), right()};
	}

	Point2 top_left() const
	{
		return {top(), left()};
	}

	Point2 top_right() const
	{
		return {top(), right()};
	}

	// 	bool hasPointInside(const Point& point) const
	// 	{
	// 		return point.x() >= left() - PHYS_MIN_POINT_DISTANCE
	// 			&& point.x() <= right() + PHYS_MIN_POINT_DISTANCE
	// 			&& point.y() >= bottom() - PHYS_MIN_POINT_DISTANCE
	// 			&& point.y() <= top() + PHYS_MIN_POINT_DISTANCE;
	// 	}
	//
	// 	bool isPointOnBoundary(const Point& point) const
	// 	{
	// 		if (point.x() < left() - PHYS_MIN_POINT_DISTANCE
	// 			|| point.x() > right() + PHYS_MIN_POINT_DISTANCE
	// 			|| point.y() < bottom() - PHYS_MIN_POINT_DISTANCE
	// 			|| point.y() > top() + PHYS_MIN_POINT_DISTANCE)
	// 		{
	// 			return false;
	// 		}
	//
	// 		if (point.x() > left() + PHYS_MIN_POINT_DISTANCE
	// 			&& point.x() < right() - PHYS_MIN_POINT_DISTANCE
	// 			&& point.y() > bottom() + PHYS_MIN_POINT_DISTANCE
	// 			&& point.y() < top() - PHYS_MIN_POINT_DISTANCE)
	// 		{
	// 			return false;
	// 		}
	//
	// 		return true;
	// 	}
	//
	// 	bool intersectsWith(const Rect& other) const
	// 	{
	// 		return right() >= other.left() - PHYS_MIN_POINT_DISTANCE
	// 			&& left() <= other.right() + PHYS_MIN_POINT_DISTANCE
	// 			&& top() >= other.bottom() - PHYS_MIN_POINT_DISTANCE
	// 			&& bottom() <= other.top() + PHYS_MIN_POINT_DISTANCE;
	// 	}
	//
	// 	bool isInRect(const Rect& other) const
	// 	{
	// 		return left() >= other.left() - PHYS_MIN_POINT_DISTANCE
	// 			&& right() <= other.right() + PHYS_MIN_POINT_DISTANCE
	// 			&& top() <= other.top() + PHYS_MIN_POINT_DISTANCE
	// 			&& bottom() >= other.bottom() - PHYS_MIN_POINT_DISTANCE;
	// 	}

private:
	Point2 bottom_left_{0, 0};
	double width_  = 0;
	double height_ = 0;
};
} // namespace esf
