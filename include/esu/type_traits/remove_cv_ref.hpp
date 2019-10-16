#pragma once
#include <type_traits>

namespace esu
{
namespace internal
{
template<class T>
struct Remove_cv_ref_impl
{
	using Type = std::remove_cv_t<std::remove_reference_t<T>>;
};
}

template<class T>
using Remove_cv_ref = typename internal::Remove_cv_ref_impl<T>::Type;
}
