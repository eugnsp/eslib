#pragma once
#include <esf/forward.hpp>
#include <esf/function/gradients.hpp>
#include <esf/function/jacobian.hpp>
#include <esf/index.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/quadr/quadr.hpp>
#include <esf/type_traits.hpp>

#include <esl/dense.hpp>
#include <esu/type_traits.hpp>

#include <cstddef>
#include <type_traits>

namespace esf
{
// Returns a local load vector
template<class Element, class Quadr = esf::Quadr<Element::order, typename Element::Space_dim>,
	     class Fn>
auto load_vector(Fn fn, double scale)
{
	constexpr auto n_dofs = Element::total_cell_dofs;

	return esl::make_vector<n_dofs>([&fn, scale](std::size_t i)
		{
			return scale * Quadr::sum([i, &fn](std::size_t iq)
				{
					constexpr auto basis = Element_quadr<Element, Quadr>::basis();
					if constexpr (std::is_invocable_v<Fn, Quadr_point_index<Quadr>>)
						return fn(Quadr_point_index<Quadr>{iq}) * basis(iq, i);
					else
						return fn(iq) * basis(iq, i);
				});
		});
}

// Returns a local load vector
template<class Element, class Quadr = esf::Quadr<Element::order, typename Element::Space_dim>>
auto load_vector(double scale)
{
	constexpr auto n_dofs = Element::total_cell_dofs;

	return esl::make_vector<n_dofs>([scale](std::size_t i)
		{
			return scale * Quadr::sum([i](std::size_t iq)
				{
					constexpr auto basis = Element_quadr<Element, Quadr>::basis();
					return basis(iq, i);
				});
		});
}
} // namespace esf
