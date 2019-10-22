#pragma once
#include <limits>

namespace esc
{
inline constexpr double e 				= 2.71828182845904523536;
inline constexpr double ln2 			= 6.93147180559945309417e-1;

inline constexpr double pi 				= 3.141592653589793238463;
inline constexpr double two_pi 			= 2 * pi;
inline constexpr double four_pi 		= 4 * pi;
inline constexpr double pi_over_2 		= pi / 2;
inline constexpr double pi_over_4 		= pi / 4;
inline constexpr double one_over_pi 	= 1 / pi;
inline constexpr double two_over_pi 	= 2 / pi;
inline constexpr double sqrt_pi 		= 1.7724538509055160272982;
inline constexpr double sqrt_two_pi 	= 2.5066282746310005024158;
inline constexpr double sqrt_pi_cubed 	= 5.5683279968317078452848;
inline constexpr double pi_squared 		= pi * pi;
inline constexpr double pi_cubed 		= pi * pi * pi;

inline constexpr double sqrt_two 		= 1.414213562373095048802;
inline constexpr double sqrt_three 		= 1.732050807568877293527;
inline constexpr double sqrt_five 		= 2.236067977499789696410;
inline constexpr double sqrt_fifteen 	= 3.872983346207416885179;

inline constexpr double sqrt_half 		= 1 / sqrt_two;
inline constexpr double sqrt_third 		= 1 / sqrt_three;

inline constexpr double dbl_nan 			= std::numeric_limits<double>::quiet_NaN();
inline constexpr double dbl_epsilon 		= std::numeric_limits<double>::epsilon();
inline constexpr double dbl_min 			= std::numeric_limits<double>::min();
inline constexpr double dbl_max 			= std::numeric_limits<double>::max();
inline constexpr double sqrt_dbl_epsilon 	=  1.490116119384765625000e-8;
inline constexpr double log_dbl_epsilon 	= -3.604365338911715608970e1;
inline constexpr double log_dbl_max 		=  7.097827128933839967322e2;
inline constexpr double log_dbl_min 		= -7.083964185322641062244e2;
} // namespace esc
