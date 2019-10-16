#pragma once
#include <esf/math/dunavant/points_group.hpp>
#include <esf/types.hpp>

namespace esf::internal
{
// Dunavant quadrature rules data - weights and barycentric coordinates
template<Local_index order>
struct Dunavant_data;

template<>
struct Dunavant_data<0>
{
	static constexpr auto groups = std::make_tuple(Dunavant_points_group{1., 1. / 3});
};

template<>
struct Dunavant_data<1> : Dunavant_data<0>
{};

template<>
struct Dunavant_data<2>
{
	static constexpr auto groups = std::make_tuple(Dunavant_points_group{1. / 3, 2. / 3, 1. / 6});
};

template<>
struct Dunavant_data<3>
{
	static constexpr auto groups = std::make_tuple(
		Dunavant_points_group{-9. / 16, 1. / 3}, Dunavant_points_group{25. / 48, .6, .2});
};

template<>
struct Dunavant_data<4>
{
	static constexpr auto groups =
		std::make_tuple(Dunavant_points_group{.223381589678011, .108103018168070, .445948490915965},
			Dunavant_points_group{.109951743655322, .816847572980459, .091576213509771});
};

template<>
struct Dunavant_data<5>
{
	static constexpr auto groups = std::make_tuple(Dunavant_points_group{9. / 40, 1. / 3},
		Dunavant_points_group{.132394152788506, .059715871789770, .470142064105115},
		Dunavant_points_group{.125939180544827, .797426985353087, .101286507323456});
};

template<>
struct Dunavant_data<6>
{
	static constexpr auto groups =
		std::make_tuple(Dunavant_points_group{.116786275726379, .501426509658179, .249286745170910},
			Dunavant_points_group{.050844906370207, .873821971016996, .063089014491502},
			Dunavant_points_group{
				.082851075618374, .053145049844817, .310352451033784, .636502499121399});
};

template<>
struct Dunavant_data<7>
{
	static constexpr auto groups = std::make_tuple(Dunavant_points_group{-.149570044467682, 1. / 3},
		Dunavant_points_group{.175615257433208, .479308067841920, .260345966079040},
		Dunavant_points_group{.053347235608838, .869739794195568, .065130102902216},
		Dunavant_points_group{
			.077113760890257, .048690315425316, .312865496004874, .638444188569810});
};

template<>
struct Dunavant_data<8>
{
	static constexpr auto groups = std::make_tuple(Dunavant_points_group{.144315607677787, 1. / 3},
		Dunavant_points_group{.095091634267285, .081414823414554, .459292588292723},
		Dunavant_points_group{.103217370534718, .658861384496480, .170569307751760},
		Dunavant_points_group{.032458497623198, .898905543365938, .050547228317031},
		Dunavant_points_group{
			.027230314174435, .008394777409958, .263112829634638, .728492392955404});
};

template<>
struct Dunavant_data<9>
{
	static constexpr auto groups = std::make_tuple(Dunavant_points_group{.097135796282799, 1. / 3},
		Dunavant_points_group{.031334700227139, .020634961602525, .489682519198738},
		Dunavant_points_group{.077827541004774, .125820817014127, .437089591492937},
		Dunavant_points_group{.079647738927210, .623592928761935, .188203535619033},
		Dunavant_points_group{.025577675658698, .910540973211095, .044729513394453},
		Dunavant_points_group{
			.043283539377289, .036838412054736, .221962989160766, .741198598784498});
};
} // namespace esf::internal
