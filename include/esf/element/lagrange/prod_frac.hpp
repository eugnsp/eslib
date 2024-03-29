#pragma once
#include <esf/index.hpp>

namespace esf::internal
{
// Returns the product prod_{k=0}^{n-1} (x-k) / (n-k),
// returns 1 if the product contains no terms (e.g., if n = 0).
template<typename N>
constexpr double prod_frac_xmk_nmk(double x, N n)
{
	double value = 1;
	for (N k = 0; k < n; ++k)
		value *= (x - k) / (n - k);

	return value;
}

// Returns the product prod_{k=0}^{n-1} (x-k) / (n-k)
// excluding (i.e. making 1) numerator of the term with k = k_exclude,
// returns 1 if the product contains no terms (e.g., if n = 0).
template<typename N>
constexpr double prod_frac_xmk_nmk(double x, N n, N k_exclude)
{
	double value = 1;
	for (N k = 0; k < n; ++k)
		value *= (k != k_exclude ? x - k : 1) / (n - k);

	return value;
}
} // namespace esf::internal
