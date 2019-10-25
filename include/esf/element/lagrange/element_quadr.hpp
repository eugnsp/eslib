#pragma once
#include <esf/element/lagrange/lagrange.hpp>
#include <esf/type_traits.hpp>

#include <esl/dense.hpp>

#include <cstddef>

namespace esf
{
template<class Element_, class Quadr_>
class Element_quadr_lagrange
{
private:
	using Element = Element_;
	using Quadr   = Quadr_;
	static constexpr std::size_t n_points = Quadr::size;
	static constexpr std::size_t n_dofs   = Element::total_cell_dofs;

	static_assert(internal::is_dim1<Element> == internal::is_dim1<Quadr>);

public:
	// Returns the values of basis functions at the quadrature points
	// as the matrix of the size (# quadr. points) x (# dofs)
	static constexpr auto basis()
	{
		return esl::make_matrix<n_points, n_dofs>(
			[](std::size_t quadr, std::size_t dof) constexpr
			{
				return Element::basis(dof, Quadr::point(quadr));
			});
	}

	// Returns the values of gradients of basis functions at the quadrature points
	// as the matrix of the size (# quadr. points) x (# dofs)
	static constexpr auto basis_grad()
	{
		return esl::make_matrix<n_points, n_dofs>([](auto quadr, auto dof) constexpr
		{
			return Element::basis_grad(dof, Quadr::point(quadr));
		});
	}
};

template<std::size_t order, class Space_dim, class Quadr>
class Element_quadr<Lagrange<order, Space_dim>, Quadr> :
	public Element_quadr_lagrange<Lagrange<order, Space_dim>, Quadr> {};

template<std::size_t order, class Space_dim, class Quadr>
class Element_quadr<Discontinuous_lagrange<order, Space_dim>, Quadr> :
	public Element_quadr_lagrange<Discontinuous_lagrange<order, Space_dim>, Quadr> {};
} // namespace esf
