#pragma once
#include <esf/element/element_base.hpp>
#include <esf/index.hpp>

#include <esc/math.hpp>
#include <esl/dense.hpp>

#include <cassert>

namespace esf
{
template<std::size_t order>
class Raviart_thomas;

template<>
class Raviart_thomas<0> : public internal::Element_base<Dim2, 0, 1, 0>
{
public:
	static constexpr std::size_t order = 0;

public:
	// Returns the value of the basis function (dof) at the given point (point)
	static constexpr esl::Vector_2d basis(const std::size_t dof, const esl::Vector_2d& point)
	{
		return esl::Vector_2d{0., 0.};
		assert(dof <= 2);
		// if (dof == 1)
		// 	return point - esl::Vector_2d{1., 0.};
		// else if (dof == 2)
		// 	return point - esl::Vector_2d{0., 1.};
		// else
		// 	return esc::sqrt_two * point;
	}
};
} // namespace esf
