#pragma once
#include <esl/dense/tags.hpp>
#include <esl/sparse/forward.hpp>

#include <esu/type_traits.hpp>

#include <type_traits>

namespace esl::internal
{
template<class T>
inline constexpr bool is_symmetry_tag =
	esu::is_one_of<T, Not_symmetric, Structural_symmetric, Symmetric_upper, Symmetric_lower>;

//////////////////////////////////////////////////////////////////////
//* Structural_symmetric  */

template<class T, typename = void>
struct Is_not_symmetric;

template<class T>
struct Is_not_symmetric<T, std::enable_if_t<is_symmetry_tag<T>>> : std::is_same<T, Not_symmetric>
{};

template<class Symmetry_tag, typename Index>
struct Is_not_symmetric<Csr_pattern<Symmetry_tag, Index>> : std::is_same<Symmetry_tag, Not_symmetric>
{};

template<typename Value, class Symmetry_tag, typename Index>
struct Is_not_symmetric<Csr_matrix<Value, Symmetry_tag, Index>> : std::is_same<Symmetry_tag, Not_symmetric>
{};

template<class Matrix>
struct Is_not_symmetric<Mkl_sparse_matrix<Matrix>> : Is_not_symmetric<esu::Remove_cv_ref<Matrix>>
{};

template<class T>
inline constexpr bool is_not_symmetric = Is_not_symmetric<T>::value;

//////////////////////////////////////////////////////////////////////
//* Structural_symmetric  */

template<class T, typename = void>
struct Is_structural_symmetric;

template<class T>
struct Is_structural_symmetric<T, std::enable_if_t<is_symmetry_tag<T>>> : std::is_same<T, Structural_symmetric>
{};

template<class Symmetry_tag, typename Index>
struct Is_structural_symmetric<Csr_pattern<Symmetry_tag, Index>> : std::is_same<Symmetry_tag, Structural_symmetric>
{};

template<typename Value, class Symmetry_tag, typename Index>
struct Is_structural_symmetric<Csr_matrix<Value, Symmetry_tag, Index>> :
	std::is_same<Symmetry_tag, Structural_symmetric>
{};

template<class Matrix>
struct Is_structural_symmetric<Mkl_sparse_matrix<Matrix>> : Is_structural_symmetric<esu::Remove_cv_ref<Matrix>>
{};

template<class T>
inline constexpr bool is_structural_symmetric = Is_structural_symmetric<T>::value;

//////////////////////////////////////////////////////////////////////
//* Symmetric_upper */

template<class T, typename = void>
struct Is_symmetric_upper;

template<class T>
struct Is_symmetric_upper<T, std::enable_if_t<is_symmetry_tag<T>>> : std::is_same<T, Symmetric_upper>
{};

template<class Symmetry_tag, typename Index>
struct Is_symmetric_upper<Csr_pattern<Symmetry_tag, Index>> : std::is_same<Symmetry_tag, Symmetric_upper>
{};

template<typename Value, class Symmetry_tag, typename Index>
struct Is_symmetric_upper<Csr_matrix<Value, Symmetry_tag, Index>> : std::is_same<Symmetry_tag, Symmetric_upper>
{};

template<class Matrix>
struct Is_symmetric_upper<Mkl_sparse_matrix<Matrix>> : Is_symmetric_upper<esu::Remove_cv_ref<Matrix>>
{};

template<class T>
inline constexpr bool is_symmetric_upper = Is_symmetric_upper<T>::value;

//////////////////////////////////////////////////////////////////////
//* Symmetric_upper */

template<class T, typename = void>
struct Is_symmetric_lower;

template<class T>
struct Is_symmetric_lower<T, std::enable_if_t<is_symmetry_tag<T>>> : std::is_same<T, Symmetric_lower>
{};

template<class Symmetry_tag, typename Index>
struct Is_symmetric_lower<Csr_pattern<Symmetry_tag, Index>> : std::is_same<Symmetry_tag, Symmetric_lower>
{};

template<typename Value, class Symmetry_tag, typename Index>
struct Is_symmetric_lower<Csr_matrix<Value, Symmetry_tag, Index>> : std::is_same<Symmetry_tag, Symmetric_lower>
{};

template<class Matrix>
struct Is_symmetric_lower<Mkl_sparse_matrix<Matrix>> : Is_symmetric_lower<esu::Remove_cv_ref<Matrix>>
{};

template<class T>
inline constexpr bool is_symmetric_lower = Is_symmetric_lower<T>::value;

//////////////////////////////////////////////////////////////////////
//* Symmetric = Symmetric_upper OR Symmetric_lower */

template<class T>
struct Is_symmetric : std::disjunction<Is_symmetric_upper<T>, Is_symmetric_lower<T>>
{};

template<class T>
inline constexpr bool is_symmetric = Is_symmetric<T>::value;
} // namespace esl::internal
