#pragma once
#include <esf/geometry/point1.hpp>
#include <esf/mesh/mesh1.hpp>
#include <esf/mesh/index.hpp>

#include <esu/iterator.hpp>

#include <array>

namespace esf::internal
{
template<class Element>
class Boundary_cond_base<1, Element>
{
	static_assert(Element::dim == 1);
	static_assert(Element::has_vertex_dofs);

public:
	static constexpr bool is_strong = true;
	static constexpr bool is_uniform = false;

	using Boundary = Point1;

public:
	Boundary_cond_base(const Mesh<1>& mesh, const Boundary& point) :
		vertex_(mesh.find_vertex(point))
	{}

	template<class... Args>
	auto vertices(const Args&...) const
	{
		static_assert(Element::has_vertex_dofs);
		return esu::Iterable{&vertex_, &vertex_ + 1};
	}

	template<class... Args>
	Vertex_index vertex(const Args&...) const
	{
		return vertex_;
	}

protected:
	Vertex_index vertex_;
};
} // namespace esf::internal
