#pragma once
#include <esf/geometry/point2.hpp>
#include <esf/geometry/points2.hpp>
#include <esf/tags.hpp>

#include <esl/dense/tags.hpp>

#include <cassert>
#include <initializer_list>
#include <utility>
#include <vector>

namespace esf
{
// A linestring (a sequence of points and line segments connecting them)
class Linestring : public Points2<esl::dynamic>
{
public:
	using Geometry_tag = Linestring_tag;

public:
	explicit Linestring(std::initializer_list<Point2> vertices) : Points2(vertices)
	{
		assert(n_vertices() > 1);
	}

	explicit Linestring(std::vector<Point2> vertices) : Points2(std::move(vertices))
	{
		assert(n_vertices() > 1);
	}
};
} // namespace esf
