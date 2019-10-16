#pragma once
#include <esf/mesh/iterator/iterator_base.hpp>
#include <esf/types.hpp>

#include <cassert>
#include <type_traits>

namespace esf
{
template<class Element_tag, class Circ_tag>
class Circulator_halfedge_edge : public internal::Iterator_base<Element_tag, Mesh<2>>
{
	static_assert(
		std::is_same_v<Element_tag, Halfedge_tag> || std::is_same_v<Element_tag, Edge_tag>);

private:
	using Base = internal::Iterator_base<Element_tag, Mesh<2>>;

public:
	Circulator_halfedge_edge(const Mesh<2>& mesh, Halfedge_index halfedge) : Base(mesh, halfedge)
	{
		assert(is_valid(halfedge));
	}

	Circulator_halfedge_edge& operator++()
	{
		const auto halfedge = view_.mesh().next(view_.internal_index(), Circ_tag{});
		view_.set_index(halfedge);
		return *this;
	}

	Circulator_halfedge_edge operator++(int)
	{
		auto old_iterator = *this;
		++(*this);
		return old_iterator;
	}

private:
	using Base::view_;
};
} // namespace esf
