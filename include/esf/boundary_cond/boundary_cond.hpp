#pragma once
#include <esf/boundary_cond/boundary_cond_base1.hpp>
#include <esf/boundary_cond/boundary_cond_base2.hpp>

namespace esf
{
template<class Element>
class Boundary_cond : public internal::Boundary_cond_base<typename Element::Space_dim, Element>
{
private:
	using Base = internal::Boundary_cond_base<typename Element::Space_dim, Element>;

public:
	using Space_dim = typename Element::Space_dim;

	static constexpr bool is_strong  = true;
	static constexpr bool is_uniform = false;

public:
	using Base::Base;
};
} // namespace esf
