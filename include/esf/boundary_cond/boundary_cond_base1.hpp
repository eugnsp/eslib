#pragma once
#include <esf/geometry/point1.hpp>
#include <esf/mesh/mesh1.hpp>
#include <esf/types.hpp>

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
	Boundary_cond_base(const Mesh<1>& mesh, esf::Point1 point)
	{
		vertex_[0] = mesh.find_vertex(point);
	}

	template<class... Args>
	auto vertices(const Args&...) const
	{
		static_assert(Element::has_vertex_dofs);
		return esu::Iterable{vertex_};
	}

	template<class... Args>
	Vertex_index vertex(const Args&...) const
	{
		return vertex_[0];
	}

protected:
	std::array<Vertex_index, 1> vertex_;
};
} // namespace esf::internal
