#pragma once
#include <esf/mesh/iterator/iterator_base.hpp>
#include <esf/types.hpp>

#include <cassert>
#include <type_traits>

namespace esf
{
template<class Element_tag, class Circ_tag>
class Circulator_vertex_face : public internal::Iterator_base<Element_tag, Mesh<2>>
{
private:
	static_assert(std::is_same_v<Element_tag, Vertex_tag> || std::is_same_v<Element_tag, Face_tag>);

	using Base = internal::Iterator_base<Element_tag, Mesh<2>>;

public:
	Circulator_vertex_face(const Mesh<2>& mesh, Halfedge_index halfedge) :
		Base(mesh), halfedge_(halfedge)
	{
		assert(is_valid(halfedge));
		set_view_index();
	}

	Circulator_vertex_face& operator=(const Circulator_vertex_face& other)
	{
		Base::operator=(other);
		halfedge_ = other.halfedge_;
		return *this;
	}

	Circulator_vertex_face& operator++()
	{
		this->next();
		set_view_index();
		return *this;
	}

	Circulator_vertex_face operator++(int)
	{
		auto old_iterator = *this;
		++(*this);
		return old_iterator;
	}

private:
	void next()
	{
		halfedge_ = view_.mesh().next(halfedge_, Circ_tag{});
	}

	void set_view_index()
	{
		if constexpr (std::is_same_v<Element_tag, Vertex_tag>)
			view_.set_index(view_.mesh().vertex_index(halfedge_));
		else if constexpr (std::is_same_v<Element_tag, Face_tag>)
		{
			// Skip invalid faces ("outside" the mesh)
			auto face = get_face_index();
			while (!is_valid(face))
			{
				next();
				face = get_face_index();
			}
			view_.set_index(face);
		}
	}

	auto get_face_index() const
	{
		return view_.mesh().face_index(
			std::is_same_v<Circ_tag, Face_circ_tag> ? twin(halfedge_) : halfedge_);
	}

private:
	using Base::view_;

	Halfedge_index halfedge_;
};
} // namespace esf
