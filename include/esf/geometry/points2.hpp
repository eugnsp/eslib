#pragma once
#include <esf/geometry/point2.hpp>
#include <esf/types.hpp>

#include <esu/array.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <initializer_list>
#include <utility>
#include <vector>

namespace esf
{
namespace internal
{
template<Local_index n>
class Points2_base
{
public:
	using Geometry_tag = Points2_tag;

public:
	const Point2& vertex(Local_index index) const
	{
		return vertices_[index];
	}

	// Returns the first vertex
	const Point2& first() const
	{
		return vertices_.front();
	}

	// Returns the last vertex
	const Point2& last() const
	{
		return vertices_.back();
	}

	auto begin_vertex() const
	{
		return vertices_.begin();
	}

	auto end_vertex() const
	{
		return vertices_.end();
	}

protected:
	Points2_base() = default;

	template<typename Points>
	Points2_base(Points&& points) : vertices_(std::forward<Points>(points))
	{}

	// TODO
	Points2_base(std::initializer_list<Point2> points)
	{
		if constexpr (n == invalid_local_index)
			vertices_.resize(points.size());
		std::copy(points.begin(), points.end(), vertices_.begin());
	}

private:
	using List =
		std::conditional_t<n == invalid_local_index, std::vector<Point2>, std::array<Point2, n>>;

protected:
	List vertices_;
};
} // namespace internal

// A collection of points
template<Local_index n>
class Points2 : public internal::Points2_base<n>
{
private:
	using Base = internal::Points2_base<n>;

public:
	Points2() = default;

	explicit Points2(const Point2 (&vertices)[n]) : Base(esu::to_array(vertices))
	{}

	explicit Points2(const std::array<Point2, n>& vertices) : Base(vertices)
	{}

	explicit Points2(std::initializer_list<Point2> vertices) : Base(vertices)
	{}

	static constexpr Local_index n_vertices()
	{
		return n;
	}
};

template<>
class Points2<invalid_local_index> : public internal::Points2_base<invalid_local_index>
{
private:
	using Base = internal::Points2_base<invalid_local_index>;

public:
	Points2() = default;

	explicit Points2(std::initializer_list<Point2> vertices) : Base(vertices)
	{}

	explicit Points2(std::vector<Point2> vertices) : Base(std::move(vertices))
	{}

	Local_index n_vertices() const
	{
		return static_cast<Local_index>(vertices_.size());
	}
};
} // namespace esf
