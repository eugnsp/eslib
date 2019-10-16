#pragma once
#include <mkl_types.h>

#include <complex>

namespace esl::internal
{
template<class T>
struct Mkl_type_trait;

template<>
struct Mkl_type_trait<float>
{
	using Type = float;
};

template<>
struct Mkl_type_trait<double>
{
	using Type = double;
};

template<>
struct Mkl_type_trait<std::complex<float>>
{
	using Type = ::MKL_Complex8;
};

template<>
struct Mkl_type_trait<std::complex<double>>
{
	using Type = ::MKL_Complex16;
};

template<class T>
using Mkl_type = typename Mkl_type_trait<T>::Type;
} // namespace esl::internal
