#pragma once
#include <esf/tags.hpp>

#include <type_traits>

namespace esf::internal
{
template<class T>
struct Is_dim1 : Is_dim1<typename T::Space_dim> {};

template<>
struct Is_dim1<Dim1> : std::true_type {};

template<>
struct Is_dim1<Dim2> : std::false_type {};

template<class T>
struct Is_dim2 : Is_dim2<typename T::Space_dim> {};

template<>
struct Is_dim2<Dim1> : std::false_type {};

template<>
struct Is_dim2<Dim2> : std::true_type {};

template<class T>
inline constexpr bool is_dim1 = Is_dim1<T>::value;

template<class T>
inline constexpr bool is_dim2 = Is_dim2<T>::value;
} // namespace esf::internal
