#pragma once
#include <esf/index.hpp>
#include <esf/quadr/quadr.hpp>

#include <esl/dense.hpp>

namespace esf
{
namespace internal
{
template<
	class Element,
	class Quadr,
	class Grads,
	class Inv_tr_jacobian>
auto gradients(
	Grads& 				   grads,
	const Inv_tr_jacobian& j)
{
	constexpr auto basis_grads = esf::Element_quadr<Element, Quadr>::basis_grad();
	for (esf::Local_index i = 0; i < grads.rows(); ++i)
		for (esf::Local_index r = 0; r < grads.cols(); ++r)
			grads(i, r) = j * basis_grads(i, r);
}
} // namespace internal

template<
	class Element,
	class Quadr>
auto gradients(const double inv_j)
{
	esl::Matrix<double, Quadr::size, Element::total_edge_dofs> grads;
	internal::gradients<Element, Quadr>(grads, inv_j);
	return grads;
}

template<
	class Element,
	class Quadr>
auto gradients(const esl::Matrix_2d& inv_tr_j)
{
	esl::Matrix<esl::Vector_2d, Quadr::size, Element::total_face_dofs> grads;
	internal::gradients<Element, Quadr>(grads, inv_tr_j);
	return grads;
}
} // namespace esf
