#pragma once
//#include "math/la/Vector.h"
//#include "element_quadr.hpp"
#include <cstddef>

namespace esf
{
namespace internal
{
template<class T_Mesh, class T_Var, class T_Derived>
class Field_base
{
	static_assert(T_Mesh::dim == T_Var::dim, "Incompatible mesh and variable dimensions");

public:
	// 	using Mesh = T_Mesh;
	// 	using Var = T_Var;
	using Element = typename T_Var::Element;
	//
	// 	static constexpr std::size_t degree = Var::degree;

public:
	// 	template<class T_Quadr>
	// 	void at_quadr(const typename T_Mesh::Cell_view& cell,
	// 		LA::Vector<double, T_Quadr::n_points>& values) const
	// 	{
	// 		values.zero();
	//
	//  		for (std::size_t i = 0; i < values.size(); ++i)
	//  			for (std::size_t j = 0; j < Element::dofs(); ++j)
	//  				values[i] += Element_quadr<Element, T_Quadr>::basis[i][j] * self().value(j);
	// 	}

	const T_Derived& self() const
	{
		return static_cast<const T_Derived&>(*this);
	}
};
} // namespace internal
} // namespace esf
