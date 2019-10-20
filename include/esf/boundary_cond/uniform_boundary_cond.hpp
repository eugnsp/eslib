#pragma once
#include <esf/boundary_cond/boundary_cond.hpp>

namespace esf
{
template<class Element, typename T = double, bool is_strong_ = true>
class Uniform_boundary_cond : public Boundary_cond<Element>
{
public:
	static constexpr bool is_strong = is_strong_;
	static constexpr bool is_uniform = true;

private:
	using Base = Boundary_cond<Element>;

public:
	using Base::Base;

	Uniform_boundary_cond(const Mesh<Element::dim>& mesh, double value) : Base(mesh), value_(value)
	{}

	Uniform_boundary_cond(
		const Mesh<Element::dim>& mesh, const typename Base::Boundary& boundary, double value) :
		Base(mesh, boundary), value_(value)
	{}

	template<class Element_index>
	T value(Element_index, std::size_t) const
	{
		return value_;
	}

	void set_value(T value)
	{
		value_ = value;
	}

private:
	T value_;
};
} // namespace esf
