#pragma once
#include <esf/function/gradients.hpp>
#include <esf/function/jacobian.hpp>
#include <esf/quadr/quadr.hpp>
#include <esf/quadr/quadr_point_index.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/index.hpp>

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
// TODO : use make_matrix ?
// Returns a local mass matrix
template<class Element, class Quadr = esf::Quadr<2 * Element::order, Element::dim>, class Fn>
auto mass_matrix(Fn fn, double scale)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	esl::Matrix_d<n_dofs, n_dofs> m;
	for (Local_index s = 0; s < n_dofs; ++s)
		for (Local_index r = 0; r < n_dofs; ++r)
			m(r, s) = scale * Quadr::sum([r, s, &fn](auto iq) {
				auto constexpr basis = Element_quadr<Element, Quadr>::basis();
				return fn(iq) * basis(iq, r) * basis(iq, s);
			});

	return m;
}

// Returns a local mass matrix
template<class Element, class Quadr = esf::Quadr<2 * Element::order, Element::dim>>
constexpr auto mass_matrix(double scale)
{
	const auto mat_fn = [scale](std::size_t i, std::size_t j)
	{
		return scale * Quadr::sum([i, j](std::size_t iq)
		{
			auto constexpr basis = Element_quadr<Element, Quadr>::basis();
			return basis(iq, i) * basis(iq, j);
		});
	};

	constexpr auto n_dofs = Element::n_total_cell_dofs;
	return esl::make_matrix<n_dofs, n_dofs>(mat_fn);
}

////////////////////////////////////////////////////////////////////////////////////////////////

template<class Element, class Quadr>
using Grad = esl::Matrix<std::conditional_t<Element::dim == 1, double, esl::Vector_2d>, Quadr::size,
	Element::n_total_cell_dofs>;

// Returns a local stiffness matrix
template<class Element, class Quadr, class Func>
auto stiffness_matrix(const Grad<Element, Quadr>& grads, Func func, double scale)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	esl::Matrix_d<n_dofs, n_dofs> m;
	for (Local_index i = 0; i < n_dofs; ++i)
		for (Local_index j = 0; j < n_dofs; ++j)
			m(i, j) = scale * Quadr::sum([i, j, &grads, &func](auto q) {
				return func(q) * esl::dot(grads(q, i), grads(q, j));
			});

	return m;
}

// Returns a local stiffness matrix
template<class Element, class Quadr = esf::Quadr<2 * (Element::order - 1), Element::dim>>
auto stiffness_matrix(const Grad<Element, Quadr>& grads, double scale)
{
	return stiffness_matrix<Element, Quadr>(grads, math::One{}, scale);
}

// Returns a local stiffness matrix
template<class Element, class Quadr = esf::Quadr<2 * (Element::order - 1), Element::dim>, class Func>
auto stiffness_matrix(
	const Grad<Element, Quadr>& grads, Func func, esl::Matrix_2d eps, double scale)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	esl::Matrix_d<n_dofs, n_dofs> m;
	for (Local_index i = 0; i < n_dofs; ++i)
		for (Local_index j = 0; j < n_dofs; ++j)
			m(i, j) = scale * Quadr::sum([i, j, &grads, &func, &eps](auto q) {
				return func(q) * esl::dot(grads(q, i), eps * grads(q, j));
			});

	return m;
}

// Returns a local stiffness matrix
template<class Element, class Quadr = esf::Quadr<2 * (Element::order - 1), Element::dim>>
auto stiffness_matrix(const Grad<Element, Quadr>& grads, esl::Matrix_2d eps, double scale)
{
	return stiffness_matrix<Element, Quadr>(grads, math::One{}, eps, scale);
}

template<class Element, class Quadr = esf::Quadr<2 * (Element::order - 1), Element::dim>, class Func>
auto stiffness_matrix(const Mesh2::Face_view& face, Func func, double scale)
{
	const auto grads = gradients<Element, Quadr>(inv_transp_jacobian(face));
	return stiffness_matrix<Element, Quadr>(grads, func, scale);
}

template<class Element, class Quadr = esf::Quadr<2 * (Element::order - 1), Element::dim>>
auto stiffness_matrix(const Mesh2::Cell_view& cell, double scale)
{
	const auto grads = gradients<Element, Quadr>(inv_transp_jacobian(cell));
	return stiffness_matrix<Element, Quadr>(grads, scale);
}

///////////////////////////////////////////////////////////////////////////////////////////////

// Returns a local load vector
template<class Element, class Quadr = esf::Quadr<Element::order, Element::dim>, class Fn>
auto load_vector(Fn fn, double scale)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	return esl::make_vector<n_dofs>([&fn, scale](auto i) {
		return scale * Quadr::sum([i, &fn](Index iq) {
			constexpr auto basis = Element_quadr<Element, Quadr>::basis();
			return fn(Quadr_point_index<Quadr>{iq}) * basis(iq, i);
		});
	});
}

// Returns a local load vector
template<class Element, class Quadr = esf::Quadr<Element::order, Element::dim>>
auto load_vector(double scale)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	return esl::make_vector<n_dofs>([scale](auto i) {
		return scale * Quadr::sum([i](Local_index q) {
			constexpr auto basis = Element_quadr<Element, Quadr>::basis();
			return basis(q, i);
		});
	});
}
} // namespace esf
