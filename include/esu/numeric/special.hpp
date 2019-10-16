#pragma once
#include <esu/numeric/const.hpp>

#include <cmath>

namespace esu
{
// Computes ln(1 + exp(x))
double ln_one_p_exp(double);

//////////////////////////////////////////////////////////////////////////
/** Fermi-Dirac statistics */

// Returns the Fermi function, F(x) = 1 / (1 + exp(x))
double fermi(double);

// Returns the Fermi-Dirac integral of order -1, F(x) = 1 / (1 + exp(-x))
double fd_int_minus_one(double);

// Returns the Fermi-Dirac integral of order 0, F(x) = ln(1 + exp(x))
double fd_int_zero(double);

// Returns the Fermi-Dirac integral of order -3/2 [Fuk15a]
double fd_int_three_halves(double);

// Returns the Fermi-Dirac integral of order -1/2 [Fuk15a]
double fd_int_minus_half(double);

// Returns the Fermi-Dirac integral of order 1/2 [Fuk15a]
double fd_int_half(double);

// Returns the Fermi-Dirac integral of order 1 [Fuk15a]
double fd_int_one(double);

// Returns the inverse Fermi-Dirac integral of order 1 [Fuk15a]
double inverse_fd_int_one(double);

// Returns the logarithm of Fermi-Dirac integral of order 1/2
double log_fd_int_half(double);

// Returns the inverse Fermi-Dirac integral of order 1/2
double inverse_fd_int_half(double);

// Returns the inverse Fermi-Dirac integral of order 1/2 of exponential
double inverseFermiDiracHalfExp(double);

//////////////////////////////////////////////////////////////////////////
/** Bernoulli function B(x) = x / (e^x - 1) */

// Returns the Bernoulli function
double bernoulli(double);

// Returns the derivative of the Bernoulli function
double bernoulli_derivative(double);

//////////////////////////////////////////////////////////////////////////

// Returns the function f(x) = (e^x - 1) / x;
double exp_m1_over_x(double);

/** @returns the function f(x) = coth(x) - 1 / x */
double coth_x_minus_over_over_x(double);

// Returns the natural logarithm of the factorial, ln n!
double ln_fact(unsigned int);

// Returns the natural logarithm of the binomial coefficient, ln C(n, m)
double ln_binom(unsigned int n, unsigned int m);

// Returns the Wigner 3-j symbol (j1 j2 j3; m1 m2 m3),
// the arguments are given in half-integer units: j1x2 = 2j1, etc.
double wigner_3j(unsigned int j1x2, unsigned int j2x2, unsigned int j3x2,
	int m1x2, int m2x2, int m3x2);

double gaunt(unsigned int l1, unsigned int l2, unsigned int l3,
	int m1, int m2, int m3);

double real_gaunt(unsigned int l1, unsigned int l2, unsigned int l3,
	int m1, int m2, int m3);
}
