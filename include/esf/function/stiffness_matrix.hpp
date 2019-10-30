#pragma once
#include <esf/forward.hpp>
#include <esf/function/gradients.hpp>
#include <esf/function/jacobian.hpp>
#include <esf/index.hpp>
#include <esf/index.hpp>
#include <esf/mesh/mesh1.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/quadr/quadr.hpp>
#include <esf/type_traits.hpp>

#include <esl/dense.hpp>
#include <esu/type_traits.hpp>

#include <cstddef>
#include <type_traits>

// TODO : remove
namespace math
{
// A function object that takes arbitrary arguments
// and returns a static constant
template<auto v>
struct Constant
{
	using Value = decltype(v);
	static constexpr auto value = v;

	template<typename... Args>
	constexpr auto operator()(Args...) const noexcept
	{
		return value;
	}
};

// A function object that takes arbitrary arguments
// and always returns zero
template<typename T = int>
struct Zero : Constant<static_cast<T>(0)>
{};

// A function object that takes arbitrary arguments
// and always returns one
template<typename T = int>
struct One : Constant<static_cast<T>(1)>
{};

} // namespace math

namespace esf
{
template<class Element, class Quadr>
using Grad = esl::Matrix<std::conditional_t<internal::is_dim1<Element>, double, esl::Vector_2d>,
						 Quadr::size, Element::total_cell_dofs>;

// Returns a local stiffness matrix
template<class Element, class Quadr, class Func>
auto stiffness_matrix(const Grad<Element, Quadr>& grads, Func func, double scale)
{
	constexpr auto n_dofs = Element::total_cell_dofs;

	esl::Matrix_d<n_dofs, n_dofs> m;
	for (std::size_t i = 0; i < n_dofs; ++i)
		for (std::size_t j = 0; j < n_dofs; ++j)
			m(i, j) = scale * Quadr::sum([i, j, &grads, &func](auto q) {
				return func(q) * esl::dot(grads(q, i), grads(q, j));
			});

	return m;
}

// Returns a local stiffness matrix
template<class Element,
	     class Quadr = esf::Quadr<2 * (Element::order - 1), typename Element::Space_dim>>
auto stiffness_matrix(const Grad<Element, Quadr>& grads, double scale)
{
	return stiffness_matrix<Element, Quadr>(grads, math::One{}, scale);
}

// Returns a local stiffness matrix
template<class Element,
	     class Quadr = esf::Quadr<2 * (Element::order - 1), typename Element::Space_dim>,
		 class Func>
auto stiffness_matrix(const Grad<Element, Quadr>& grads, Func func,
                      esl::Matrix_2d eps, double scale)
{
	constexpr auto n_dofs = Element::total_cell_dofs;

	esl::Matrix_d<n_dofs, n_dofs> m;
	for (std::size_t i = 0; i < n_dofs; ++i)
		for (std::size_t j = 0; j < n_dofs; ++j)
			m(i, j) = scale * Quadr::sum([i, j, &grads, &func, &eps](auto q)
			{
				return func(q) * esl::dot(grads(q, i), eps * grads(q, j));
			});

	return m;
}

// Returns a local stiffness matrix
template<class Element,
	class Quadr = esf::Quadr<2 * (Element::order - 1), typename Element::Space_dim>>
auto stiffness_matrix(const Grad<Element, Quadr>& grads, esl::Matrix_2d eps, double scale)
{
	return stiffness_matrix<Element, Quadr>(grads, math::One{}, eps, scale);
}

template<class Element,
		 class Quadr = esf::Quadr<2 * (Element::order - 1), typename Element::Space_dim>>
auto stiffness_matrix(const typename Mesh<typename Element::Space_dim>::Cell_view& cell,
					  double scale)
{
	const auto grads = gradients<Element, Quadr>(inv_transp_jacobian(cell));
	return stiffness_matrix<Element, Quadr>(grads, scale);
}
} // namespace esf
