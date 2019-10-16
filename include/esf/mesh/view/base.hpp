#pragma once
#include <esf/type_traits.hpp>

#include <cassert>
#include <type_traits>

namespace esf::internal
{
template<class Element_tag, class Mesh>
class Element_view_base
{
public:
	using Element_index = Element_index_by_tag<Element_tag>;

	// For all elements of 2D meshes, except edges, store index of the corresponding
	// type, for edges store halfedge index (of either corresponding halfedge)
	using Internal_index = Decay_element_index_in_view<Element_index, Mesh>;

public:
	Element_view_base(const Mesh& mesh, Internal_index index) : index_(index), mesh_(mesh)
	{}

	Element_view_base& operator=(const Element_view_base& view)
	{
		assert(&mesh_ == &view.mesh_);
		set_index(*view);
		return *this;
	}

	Internal_index operator*() const
	{
		return index_;
	}

	Internal_index internal_index() const
	{
		return index_;
	}

	void set_index(Internal_index index)
	{
		index_ = index;
	}

	bool is_valid() const
	{
		return esf::is_valid(index_);
	}

	bool is_boundary() const
	{
		return mesh_.is_boundary(index_);
	}

	const Mesh& mesh() const
	{
		return mesh_;
	}

protected:
	Internal_index index_;
	const Mesh& mesh_;
};
} // namespace esf::internal
