#pragma once
#include <esl/dense/functor/fn_scalar_loop.hpp>
#include <esl/dense/type_traits/mkl_decay.hpp>

namespace esl::internal
{
// Returns a functor for the X = a operation (the default one uses two nested loops)
template<class Expr, typename Scalar, class Mkl_expr, typename = void>
struct Fn_scalar_assign_trait
{
	using Type = Fn_scalar_assign_loop;
};

// Returns a functor for the X += a operation (the default one uses two nested loops)
template<class Expr, typename Scalar, class Mkl_expr, typename = void>
struct Fn_scalar_add_assign_trait
{
	using Type = Fn_scalar_add_assign_loop;
};

// Returns a functor for the X -= a operation (the default one uses two nested loops)
template<class Expr, typename Scalar, class Mkl_expr, typename = void>
struct Fn_scalar_sub_assign_trait
{
	using Type = Fn_scalar_sub_assign_loop;
};

// Returns a functor for the X *= a operation (the default one uses two nested loops)
template<class Expr, typename Scalar, class Mkl_expr, typename = void>
struct Fn_scalar_mul_assign_trait
{
	using Type = Fn_scalar_mul_assign_loop;
};

// Returns a functor for the X /= a operation (the default one uses two nested loops)
template<class Expr, typename Scalar, class Mkl_expr, typename = void>
struct Fn_scalar_div_assign_trait
{
	using Type = Fn_scalar_div_assign_loop;
};

template<class Expr, typename Scalar, template<class, typename, class, typename = void> class Trait>
using Make_fn_scalar_type = typename Trait<Expr, Scalar, Mkl_expr_decay<Expr>>::Type;

//////////////////////////////////////////////////////////////////////

// Returns a functor for the X = a operation
template<class Expr1, class Expr2>
using Fn_scalar_assign_type = Make_fn_scalar_type<Expr1, Expr2, Fn_scalar_assign_trait>;

// Returns a functor for the X += a operation
template<class Expr1, class Expr2>
using Fn_scalar_add_assign_type = Make_fn_scalar_type<Expr1, Expr2, Fn_scalar_add_assign_trait>;

// Returns a functor for the X -= a operation
template<class Expr1, class Expr2>
using Fn_scalar_sub_assign_type = Make_fn_scalar_type<Expr1, Expr2, Fn_scalar_sub_assign_trait>;

// Returns a functor for the X *= a operation
template<class Expr1, class Expr2>
using Fn_scalar_mul_assign_type = Make_fn_scalar_type<Expr1, Expr2, Fn_scalar_mul_assign_trait>;

// Returns a functor for the X /= a operation
template<class Expr1, class Expr2>
using Fn_scalar_div_assign_type = Make_fn_scalar_type<Expr1, Expr2, Fn_scalar_div_assign_trait>;
} // namespace esl::internal
