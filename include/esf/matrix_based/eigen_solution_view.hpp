#pragma once
#include <esf/types.hpp>

#include <esl/dense.hpp>

#include <cstddef>
#include <utility>

namespace esf
{
template<class System, typename Value_>
class Eigen_solution_view
{
public:
	using Value = Value_;
	using Mesh = typename System::Mesh;

private:
	template<std::size_t var>
	using Element = typename System::template Var_t<var>::Element;

	using Vector = esl::Vector_x<Value>;
	using Matrix = esl::Matrix_x<Value>;

public:
	Eigen_solution_view(const System& system, const Vector& eigen_values, const Matrix& eigen_vectors) :
		eigen_values_(eigen_values), eigen_vectors_(eigen_vectors), system_(system)
	{}

	std::size_t size() const
	{
		return eigen_values_.size();
	}

	Value operator[](std::size_t index) const
	{
		return eigen_values_[index];
	}

	Value operator()(std::size_t i, std::size_t j) const
	{
		return eigen_vectors_(i, j);
	}

	template<std::size_t var = 0>
	Value at(Vertex_index vertex, std::size_t eigen_pair) const
	{
		typename System::template Var_vertex_dofs<var> dofs;
		system_.dof_mapper().template vertex_dofs<var>(vertex, dofs);
		if (dofs[0].is_free)
			return eigen_vectors_(dofs[0].index, eigen_pair);
		else
			return 0;
	}

	template<class Quadr, std::size_t var = 0>
	auto at_quadr(std::size_t index, const typename System::template Var_dofs<var>& dofs) const
	{
		assert(index < size());
		esl::Vector<Value, Quadr::size> vals_at_quadr{};

		for (std::size_t iq = 0; iq < Quadr::size; ++iq)
			for (std::size_t id = 0; id < dofs.size(); ++id)
				if (dofs[id].is_free)
				{
					const auto v = eigen_vectors_(dofs[id].index, index);
					vals_at_quadr[iq] += esf::Element_quadr<Element<var>, Quadr>::basis()(iq, id) * v;
				}

		return vals_at_quadr;
	}

	template<class Quadr, std::size_t var = 0>
	auto at_quadr(std::size_t index, const typename Mesh::Cell_view& cell) const
	{
		const auto dofs = system_.dof_mapper().template dofs<var>(cell);
		return at_quadr<Quadr>(index, dofs);
	}

	const Mesh& mesh() const
	{
		return system_.mesh();
	}

private:
	const Vector& eigen_values_;
	const Matrix& eigen_vectors_;

	const System& system_;
};

template<class Quadr, std::size_t var = 0, class System, class Value>
auto at_quadr(const Eigen_solution_view<System, Value>& solution, std::size_t index,
	const typename System::template Var_dofs<var>& dofs)
{
	return solution.template at_quadr<Quadr, var>(index, dofs);
}

template<class Quadr, std::size_t var = 0, class System, class Value>
auto at_quadr(
	const Eigen_solution_view<System, Value>& solution, std::size_t index, const typename System::Mesh::Cell_view& cell)
{
	return solution.template at_quadr<Quadr, var>(index, cell);
}
} // namespace esf
