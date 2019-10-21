#pragma once
#include <esf/boundary_cond/boundary_cond_base1.hpp>
#include <esf/boundary_cond/boundary_cond_base2.hpp>

namespace esf
{
template<class Element>
using Boundary_cond = internal::Boundary_cond_base<typename Element::Space_dim, Element>;
} // namespace esf
