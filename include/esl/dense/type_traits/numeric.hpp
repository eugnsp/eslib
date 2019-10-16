#pragma once
#include <esu/type_traits.hpp>

#include <mkl_types.h>

#include <complex>
#include <type_traits>

namespace esl::internal
{
template<typename T>
struct Is_fd : esu::Is_one_of<T, float, double>
{};

template<typename T>
inline constexpr bool is_fd = Is_fd<T>::value;

template<typename T>
struct Is_cfd : esu::Is_one_of<T, std::complex<float>, std::complex<double>, ::MKL_Complex8, ::MKL_Complex16>
{};

template<typename T>
inline constexpr bool is_cfd = Is_cfd<T>::value;

template<typename T>
struct Is_fd_or_cfd : std::disjunction<Is_fd<T>, Is_cfd<T>>
{};

template<typename T>
inline constexpr bool is_fd_or_cfd = Is_fd_or_cfd<T>::value;

template<class T>
struct Add_complex_trait
{
	using Type = std::complex<T>;
};

template<class T>
struct Add_complex_trait<std::complex<T>>
{
	using Type = std::complex<T>;
};

template<class T>
using Add_complex = typename Add_complex_trait<T>::Type;

template<class T>
struct Remove_complex_trait
{
	using Type = T;
};

template<class T>
struct Remove_complex_trait<std::complex<T>>
{
	using Type = T;
};

template<class T>
using Remove_complex = typename Remove_complex_trait<T>::Type;

template<class T, typename = void>
struct Make_floating_point_trait;

template<class T>
struct Make_floating_point_trait<T, std::enable_if_t<std::is_floating_point_v<T>>>
{
	using Type = T;
};

template<class T>
struct Make_floating_point_trait<T, std::enable_if_t<std::is_integral_v<T>>>
{
	using Type = double;
};

template<typename T>
using Make_floating_point = typename Make_floating_point_trait<T>::Type;
} // namespace esl::internal
