#pragma once
#include <esf/geometry/point2.hpp>
#include <esf/geometry/points2.hpp>
#include <esf/types.hpp>

#include <esu/algorithm.hpp>
#include <esu/array.hpp>

#include <cassert>
#include <utility>
#include <vector>

namespace esf
{
template<unsigned int n>
auto turtle(const Point2 (&points)[n])
{
	auto pts = esu::to_array(points);
	esu::for_each_pair(pts.begin(), pts.end(), [](const auto& p1, auto& p2) { p2 += p1; });

	return pts;
}

// A simple polygon (a sequence of points defining an exterior bounding
// ring with non-intersecting sides) with a CCW orientation
template<unsigned int n>
class Polygon : public Points2<n>
{
public:
	using Geometry_tag = Polygon_tag;

private:
	static_assert(n > 2);
	using Base = Points2<n>;

public:
	using Base::Base;
};

/*
template<>
class Polygon<dynamic> : public Points<dynamic>
{
public:
	using Geometry_tag = Polygon_tag;

private:
	using Base = Points<dynamic>;

public:
	explicit Polygon(std::initializer_list<Point> vertices) : Base(vertices)
	{
		assert(n_vertices() > 2);
	}

	explicit Polygon(std::vector<Point> vertices) : Base(std::move(vertices))
	{
		assert(n_vertices() > 2);
	}
};
*/
} // namespace esf
