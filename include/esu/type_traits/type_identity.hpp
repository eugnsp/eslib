#pragma once

namespace esu
{
namespace internal
{
template<class T>
struct Type_impl
{
	using Type = T;
};
}

template<class T>
struct Type : internal::Type_impl<T>
{};

template<class T>
using Type_identity = typename internal::Type_impl<T>::Type;
}
