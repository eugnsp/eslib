#pragma once
#include <esf/geometry.hpp>
#include <esf/math.hpp>
#include <esf/types.hpp>

#include <esl/dense.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace esf
{
namespace internal
{
template<class System, class T>
class Solution_base
{
public:
	using Value = esl::Value_type<T>;
	using Mesh = typename System::Mesh;

private:
	template<std::size_t var>
	using Element = typename System::template Var_t<var>::Element;

public:
	Solution_base(const System& system, const T& values) : values_(values), system_(system)
	{}

	///////////////////////////////////////////////////////////////////////
	//* Solution values access */

	const T& values() const
	{
		return values_;
	}

	template<class Quadr, std::size_t var = 0>
	auto at_quadr(const typename System::template Var_dofs<var>& dofs) const
	{
		esl::Vector<Value, Quadr::size> vals_at_quadr{};

		for (std::size_t iq = 0; iq < Quadr::size; ++iq)
			for (std::size_t id = 0; id < dofs.size(); ++id)
			{
				const auto v = values_[dofs[id].index];
				vals_at_quadr[iq] += Element_quadr<Element<var>, Quadr>::basis()(iq, id) * v;
			}

		return vals_at_quadr;
	}

	template<class Quadr, std::size_t var = 0>
	auto at_quadr(const typename System::Mesh::Cell_view& cell) const
	{
		const auto dofs = system_.dof_mapper().template dofs<var>(cell);
		return at_quadr<Quadr, var>(dofs);
	}

	template<std::size_t var = 0>
	auto& at(Vertex_index vertex) const
	{
		typename System::template Var_vertex_dofs<var> dofs;
		system_.dof_mapper().template vertex_dofs<var>(vertex, dofs);

		// TODO : return a vector if we have several DoFs
		return values_[dofs[0].index];
	}

	auto& operator[](Vertex_index vertex) const
	{
		return at<0>(vertex);
	}

	template<std::size_t var = 0>
	Value at(const esf::Point2& pt, const typename Mesh::Cell_view& cell) const
	{
		static_assert(Mesh::dim == 2);

		const auto pt_ref = point_to_ref_triangle(pt, cell);
		const auto dofs = system_.dof_mapper().template dofs<var>(cell);
		Value value{};
		for (Local_index id = 0; id < dofs.size(); ++id)
			value += Element<var>::basis(id, pt_ref) * values_[dofs[id].index];
		return value;
	}

	Value operator()(const esf::Point2& pt, const typename Mesh::Cell_view& cell) const
	{
		return at<0>(pt, cell);
	}

	const System& system() const
	{
		return system_;
	}

	const typename System::Mesh& mesh() const
	{
		return system_.mesh();
	}

protected:
	T values_;
	const System& system_;
};
} // namespace internal

template<class Quadr, std::size_t var = 0, class System, class T>
auto at_quadr(const internal::Solution_base<System, T>& solution, const typename System::template Var_dofs<var>& dofs)
{
	return solution.template at_quadr<Quadr, var>(dofs);
}

template<class Quadr, std::size_t var = 0, class System, class T>
auto at_quadr(const internal::Solution_base<System, T>& solution, const typename System::Mesh::Cell_view& cell)
{
	return solution.template at_quadr<Quadr, var>(cell);
}
} // namespace esf
