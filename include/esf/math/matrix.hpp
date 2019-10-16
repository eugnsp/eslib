#pragma once
#include <esf/math/quadr.hpp>
#include <esf/types.hpp>

#include <esl/dense.hpp>

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
template<class Element, class Quadr, class Func>
auto mass_matrix(Func func, double scale = 1)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	esl::Matrix_d<n_dofs, n_dofs> m;
	for (Local_index s = 0; s < n_dofs; ++s)
		for (Local_index r = 0; r < n_dofs; ++r)
			m(r, s) = scale * Quadr::sum([r, s, &func](auto iq) {
				// TODO : constexpr, GCC internal compiler error
				auto const /*expr*/ basis = Element_quadr<Element, Quadr>::basis();
				return func(iq) * basis(iq, r) * basis(iq, s);
			});

	return m;
}

// Returns a local mass matrix
template<class Element, class Quadr>
constexpr auto mass_matrix(double scale = 1)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	return esl::make_matrix<n_dofs, n_dofs>([scale](auto i, auto j) {
		return scale * Quadr::sum([i, j](auto q) {
			// TODO : constexpr, GCC internal compiler error
			auto const /*expr*/ basis = Element_quadr<Element, Quadr>::basis();
			return basis(q, i) * basis(q, j);
		});
	});
}

// Returns a local stiffness matrix
template<class Element, class Quadr, class Func, class Grads>
auto stiffness_matrix(const Grads& grads, Func func, double scale = 1)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	static_assert(Grads::rows() == Quadr::size);
	static_assert(Grads::cols() == n_dofs);

	esl::Matrix_d<n_dofs, n_dofs> m;
	for (Local_index i = 0; i < n_dofs; ++i)
		for (Local_index j = 0; j < n_dofs; ++j)
			m(i, j) = scale * Quadr::sum([i, j, &grads, &func](auto q) {
				return func(q) * esl::dot(grads(q, i), grads(q, j));
			});

	return m;
}

// Returns a local stiffness matrix
template<class Element, class Quadr, class Func, class Grads>
auto stiffness_matrix(const Grads& grads, Func func, esl::Matrix_2d eps, double scale = 1)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	static_assert(grads.rows() == Quadr::size());
	static_assert(grads.cols() == n_dofs);

	esl::Matrix_d<n_dofs, n_dofs> m;
	for (Local_index i = 0; i < n_dofs; ++i)
		for (Local_index j = 0; j < n_dofs; ++j)
			m(i, j) = scale * Quadr::sum([i, j, &grads, &func, &eps](auto q) {
				return func(q) * esl::dot(grads(q, i), eps * grads(q, j));
			});

	return m;
}

// Returns a local stiffness matrix
template<class Element, class Quadr, class Grads>
auto stiffness_matrix(const Grads& grads, double scale = 1)
{
	return stiffness_matrix<Element, Quadr>(grads, math::One{}, scale);
}

// Returns a local stiffness matrix
template<class Element, class Quadr, class Grads>
auto stiffness_matrix(const Grads& grads, esl::Matrix_2d eps, double scale = 1)
{
	return stiffness_matrix<Element, Quadr>(grads, math::One{}, eps, scale);
}

// Returns a local load vector
template<class Element, class Quadr, class Func>
auto load_vector(Func func, double scale)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	return esl::make_vector<n_dofs>([&func, scale](auto i) {
		return scale * Quadr::sum([i, &func](Local_index q) {
			// TODO : constexpr, GCC internal compiler error
			const /*expr*/ auto basis = Element_quadr<Element, Quadr>::basis();
			return func(q) * basis(q, i);
		});
	});
}

// Returns a local load vector
template<class Element, class Quadr>
auto load_vector(double scale = 1)
{
	constexpr auto n_dofs = Element::n_total_cell_dofs;

	return esl::make_vector<n_dofs>([scale](auto i) {
		return scale * Quadr::sum([i](Local_index q) {
			// TODO : constexpr, GCC internal compiler error
			const /*expr*/ auto basis = Element_quadr<Element, Quadr>::basis();
			return basis(q, i);
		});
	});
}
} // namespace esf
