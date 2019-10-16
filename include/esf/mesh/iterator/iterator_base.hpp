#pragma once
#include <esf/types.hpp>

#include <cassert>

namespace esf::internal
{
template<class Element_tag, class Mesh>
class Iterator_base
{
public:
	using View = Element_view<Element_tag, Mesh>;
	using Element_index = typename View::Element_index;
	using Internal_index = typename View::Internal_index;

public:
	explicit Iterator_base(const Mesh& mesh) : view_(mesh, Internal_index::invalid)
	{}

	Iterator_base(const Mesh& mesh, Internal_index index) : view_(mesh, index)
	{}

	Iterator_base& operator=(const Iterator_base& other)
	{
		view_ = other.view_;
		return *this;
	}

	const View& operator*() const
	{
		return view_;
	}

	const View* operator->() const
	{
		return &view_;
	}

protected:
	View view_;
};

template<class Element_tag, class Mesh>
bool operator==(
	const Iterator_base<Element_tag, Mesh>& it1, const Iterator_base<Element_tag, Mesh>& it2)
{
	assert(&it1->mesh() == &it2->mesh());
	return **it1 == **it2;
}

template<class Element_tag, class Mesh>
bool operator!=(
	const Iterator_base<Element_tag, Mesh>& it1, const Iterator_base<Element_tag, Mesh>& it2)
{
	return !(it1 == it2);
}
} // namespace esf::internal
