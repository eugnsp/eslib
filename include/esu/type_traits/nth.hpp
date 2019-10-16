#pragma once
#include <cstddef>

namespace esu
{
namespace internal
{
// Returns the n-th type in the pack
template<std::size_t n, typename T, typename... Ts>
struct Nth_impl
{
	using Type = typename Nth_impl<n - 1, Ts...>::Type;
};

template<typename T, typename... Ts>
struct Nth_impl<0, T, Ts...>
{
	using Type = T;
};

template<typename T, typename... Ts>
struct Head_impl
{
	using Type = T;
};
} // namespace internal

// Returns the n-th type in the pack
template<std::size_t n, class... Ts>
using Nth = typename internal::Nth_impl<n, Ts...>::Type;

// Returns the first type in the pack
template<typename... Ts>
using Head = typename internal::Head_impl<Ts...>::Type;
} // namespace esu
