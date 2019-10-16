#pragma once
#include <esf/types.hpp>

#include <esu/array.hpp>
#include <esu/numeric.hpp>

namespace esf::internal
{
struct Gauss_point
{
	double weight;
	double x;
};

template<Local_index t_order>
struct Gauss_data
{};

template<>
struct Gauss_data<0>
{
	static constexpr auto points = esu::make_array(Gauss_point{1, 0});
};

template<>
struct Gauss_data<1> : Gauss_data<0>
{};

template<>
struct Gauss_data<2>
{
	static constexpr auto points =
		esu::make_array(Gauss_point{.5, -esu::math::sqrt_three / 3},
			Gauss_point{.5, esu::math::sqrt_three / 3});
};

template<>
struct Gauss_data<3>
{
	static constexpr auto points = esu::make_array(Gauss_point{5 / 18, -7.745966692414834e-1},
		Gauss_point{4. / 9, 0}, Gauss_point{5. / 18, 7.745966692414834e-1});
};

template<>
struct Gauss_data<4>
{
	static constexpr auto points =
		esu::make_array(Gauss_point{1.7392742256872693e-1, -8.6113631159405302e-1},
			Gauss_point{3.2607257743127307e-1, -3.3998104358485626e-1},
			Gauss_point{3.2607257743127307e-1, 3.3998104358485626e-1},
			Gauss_point{1.7392742256872693e-1, 8.6113631159405291e-1});
};

template<>
struct Gauss_data<5>
{
	static constexpr auto points =
		esu::make_array(Gauss_point{1.1846344252809454e-1, -9.0617984593866396e-1},
			Gauss_point{2.3931433524968324e-1, -5.3846931010568289e-1}, Gauss_point{128. / 450, 0},
			Gauss_point{2.3931433524968324e-1, 5.3846931010568300e-1},
			Gauss_point{1.1846344252809454e-1, 9.0617984593866385e-1});
};
} // namespace esf::internal
