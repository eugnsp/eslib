#pragma once
#include <cstddef>
#include <utility>

namespace esl
{
namespace internal
{
struct Internal
{};

// Dynamic extent
struct Dynamic : std::integral_constant<std::size_t, static_cast<std::size_t>(-1)>
{};

///////////////////////////////////////////////////////////////////////
//> Expression tags

template<typename Value>
struct Lvalue_block_tag
{};

struct Fn_mul_tag
{};

template<class Expr, typename Scalar, class Fn>
struct Scalar_expr_tag
{};

template<class Expr1, class Expr2, class Fn>
struct Binary_expr_tag
{};
} // namespace internal

// The constant used to denote dynamic extents of expressions.
inline constexpr auto dynamic = internal::Dynamic::value;

//////////////////////////////////////////////////////////////////////
//> Value categories of expressions

struct Rvalue
{};

struct Lvalue
{};

//////////////////////////////////////////////////////////////////////
//> Expressions symmetry tags

struct Not_symmetric
{};

struct Structural_symmetric
{};

struct Symmetric_upper
{};

struct Symmetric_lower
{};

// struct Hermitian
// {};

//////////////////////////////////////////////////////////////////////
//> Dense matrix memory layouts and traversal orders

// The column-major layout: a(0, 0), a(1, 0), a(2, 0), ..., a(0, 1), a(1, 1), ....
struct Col_major
{};

// The column-major layout: `a(0, 0), a(1, 0), a(2, 0), ..., a(0, 1), a(1, 1), ...`.
struct Row_major
{};

// Tag that specifies that any traversal order can be used
struct No_layout
{};

///////////////////////////////////////////////////////////////////////
//> Matrix transposition operations

struct No_transpose
{};

struct Transpose
{};

struct Conj_transpose
{};
} // namespace esl
