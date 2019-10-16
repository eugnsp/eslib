#pragma once
#include "point.hpp"
#include "Rect.h"
#include <cstddef>
#include <vector>
#include <algorithm>

	namespace geom
	{
		class Boundary		
		{
		public:
			Boundary(std::vector<Point> points = { }, bool isClosed = false)
				: points_(points), isClosed_(isClosed) { }

			std::size_t numberOfPoints() const
			{
				return points_.size();
			}

			double length() const;

			bool contains(const Point&) const;

			double GetDistance(const Point&) const;

			bool isInRect(const Rect& rect) const
			{
				return std::all_of(points_.begin(), points_.end(),
								   [&rect](const Point& p) { return rect.hasPointInside(p); });
			}

			const Point& first() const
			{
				return points_.front();
			}

			const Point& last() const
			{
				return points_.back();
			}

			bool is_closed() const
			{
				return isClosed_;
			}
		
		private:
			std::vector<Point> points_;
			const bool isClosed_;
		};
	}
