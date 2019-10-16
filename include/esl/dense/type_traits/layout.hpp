#pragma once
#include <esl/dense/forward.hpp>
#include <esl/dense/tags.hpp>

#include <esu/type_traits.hpp>

#include <type_traits>

namespace esl
{
namespace internal
{
template<class Layout>
struct Transpose_layout;

template<>
struct Transpose_layout<No_layout>
{
	using Type = No_layout;
};

template<>
struct Transpose_layout<Col_major>
{
	using Type = Row_major;
};

template<>
struct Transpose_layout<Row_major>
{
	using Type = Col_major;
};

////////////////////////////////////////////////////////////////////////////////////////////////

template<class... Ts>
struct Common_layout
{
	using Type = Col_major;
};

template<class... Ts>
struct Common_layout<No_layout, Ts...> : Common_layout<Ts...>
{};
} // namespace internal

////////////////////////////////////////////////////////////////////////////////////////////////

template<class Expr>
using Layout_tag = typename Traits<esu::Remove_cv_ref<Expr>>::Layout;

template<class... Exprs>
using Common_layout_tag = typename internal::Common_layout<Layout_tag<Exprs>...>::Type;

template<class T>
inline constexpr bool is_row_major = std::is_same_v<Layout_tag<T>, Row_major>;

template<>
inline constexpr bool is_row_major<Row_major> = true;

template<>
inline constexpr bool is_row_major<Col_major> = false;

template<class T>
inline constexpr bool is_col_major = std::is_same_v<Layout_tag<T>, Col_major>;

template<>
inline constexpr bool is_col_major<Row_major> = false;

template<>
inline constexpr bool is_col_major<Col_major> = true;

namespace internal
{
template<class... Exprs>
inline constexpr bool have_same_layouts = esu::are_same<Layout_tag<Exprs>...>;
}
} // namespace esl
